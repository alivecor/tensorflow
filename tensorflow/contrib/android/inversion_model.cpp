//
// Created by Alexander Valys on 10/28/16.
//

#include "tensorflow/contrib/android/inversion_model.h"
#include <cstring>

namespace ac {

InversionModel::Status InversionModel::CalculateInvertedProbability(const std::vector<float>& ecg_signal_mv,
                                                                    InversionResult* result,
                                                                    float confidence_threshold) {

    if (ecg_signal_mv.size() < min_ecg_length()) {
        error_string_ = "ECG too short";
        return InversionModel::Status::kInvalidInput;
    }

    tensorflow::Tensor ecgTensor(tensorflow::DT_FLOAT, tensorflow::TensorShape({1, min_ecg_length(), 1, 1}));
    auto ecg_mapped = ecgTensor.tensor<float, 4>();
    float* data = ecg_mapped.data();

    memcpy(data, ecg_signal_mv.data(), sizeof(float[min_ecg_length()]));

    std::vector<tensorflow::Tensor> outputs;

    tensorflow::Tensor dropout(tensorflow::DT_FLOAT, tensorflow::TensorShape({}));
    dropout.scalar<float>()(0) = 1.0;
    tensorflow::Status run_status = session_->Run({{ECG_NAME, ecgTensor}, {DROPOUT_NAME, dropout}},
                                                 {OUTPUT_NAME}, {}, &outputs);

    if (!run_status.ok()) {
        error_string_ = "Failed to run model: " + run_status.ToString();
        return InversionModel::Status::kFailedRun;
    }

    tensorflow::Tensor* output = &outputs[0];
    auto vec = output->flat<float>();

    result->raw_inverted_probability = vec(1);
    result->confident = result->raw_inverted_probability > confidence_threshold
                        || (1 - result->raw_inverted_probability > confidence_threshold);
    result->was_inverted = (result->raw_inverted_probability) > 0.5;

    return InversionModel::Status::kSuccess;
}

}