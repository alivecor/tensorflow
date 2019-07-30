//
// Created by Alexander Valys on 10/28/16.
//

#ifndef NEURAL_SUITE_IDENTITY_MODEL_H
#define NEURAL_SUITE_IDENTITY_MODEL_H

#include "tensorflow/contrib/android/tensorflow_model.h"
#include <vector>

namespace ac {

struct IdentityResult {
    bool identity_matched;
    float identity_probability;
};

class IdentityModel : public TensorflowModel {
public:
    Status CalculateIdentityProbability(const std::vector<float>& ecg_samples_mv, IdentityResult* result, float confidence_threshold = 0.5);

    int min_ecg_length() { return 3000; }

private:
    const std::string OUTPUT_NAME = "softmax_output";
    const std::string DROPOUT_NAME = "input/keepProb";
    const std::string ECG_NAME = "input/ecgs";
};

}
#endif //NEURAL_SUITE_IDENTITY_MODEL_H
