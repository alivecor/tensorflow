//
// Created by Alexander Valys on 10/28/16.
//

#include "tensorflow/contrib/android/identity_model.h"
#include <algorithm>
#include <cstring>

namespace ac {

const long SAMPLE_RATE_HZ = 300;
const long EVALUATION_SECONDS = 30;
const long ID_STRIDE = 40;

IdentityModel::Status IdentityModel::CalculateIdentityProbability(const std::vector<float> &ecg_signal_mv,
                                                                  IdentityResult *result,
                                                                  float confidence_threshold) {
    if (ecg_signal_mv.size() < min_ecg_length()) {
        error_string_ = "ECG too short";
        return Status::kInvalidInput;
    }

    // We will only process the first EVALUATION_SECONDS of data through the ID network
    long signal_length_for_eval = std::min(SAMPLE_RATE_HZ * EVALUATION_SECONDS, (long)ecg_signal_mv.size());

    // ECG is divided into multiple overlapping intervals with stride ID_STRIDE and length min_ecg_length()
    // Each interval is input to the network as an independent signal
    long num_strided_intervals = (signal_length_for_eval - min_ecg_length()) / ID_STRIDE;
    tensorflow::Tensor ecgTensor(tensorflow::DT_FLOAT, tensorflow::TensorShape({num_strided_intervals, min_ecg_length(), 1, 1}));
    for (int i = 0; i < num_strided_intervals; i++) {
        memcpy(ecgTensor.Slice(i,i+1).flat<float>().data(), &ecg_signal_mv.data()[i * ID_STRIDE], sizeof(float[min_ecg_length()]));
    }

    tensorflow::Tensor dropout(tensorflow::DT_FLOAT, tensorflow::TensorShape({}));
    dropout.scalar<float>()(0) = 1.0;

    std::vector<tensorflow::Tensor> outputs;
    tensorflow::Status run_status = session_->Run({{ECG_NAME, ecgTensor}, {DROPOUT_NAME, dropout}},
                                                 {OUTPUT_NAME}, {}, &outputs);

    if (!run_status.ok()) {
        error_string_ = "Failed to run model: " + run_status.ToString();
        return Status::kFailedRun;
    }

    tensorflow::Tensor* interval_softmax = &outputs[0];
    auto vec = interval_softmax->flat_inner_dims<float>();

    // Record the maximum ID probability across all intervals
    result->identity_probability = 0;
    for (int i = 0; i < num_strided_intervals; i++) {
        float match_prob = vec(i,1);
        result->identity_probability = std::max(result->identity_probability, match_prob);
    }

    result->identity_matched = result->identity_probability > confidence_threshold;

    return Status::kSuccess;
}

}