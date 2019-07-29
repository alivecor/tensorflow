//
// Created by Alexander Valys on 10/28/16.
//

#ifndef NEURAL_SUITE_INVERSION_MODEL_H
#define NEURAL_SUITE_INVERSION_MODEL_H

#include "tensorflow/contrib/android/tensorflow_model.h"
#include <vector>

namespace ac {

struct InversionResult {
    bool confident;
    bool was_inverted;

    float raw_inverted_probability;
};

class InversionModel : public TensorflowModel {
public:
    Status CalculateInvertedProbability(const std::vector<float>& ecg_samples_mv, InversionResult* result, float confidence_threshold = 0.5);

    int min_ecg_length() { return 8100; }

private:
    const std::string OUTPUT_NAME = "output_inv_prob";
    const std::string DROPOUT_NAME = "input_dropout";
    const std::string ECG_NAME = "input_ecg";
};

}
#endif //NEURAL_SUITE_INVERSION_MODEL_H
