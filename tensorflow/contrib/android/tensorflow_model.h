
#ifndef NEURAL_SUITE_TENSORFLOW_MODEL_H
#define NEURAL_SUITE_TENSORFLOW_MODEL_H

#include <memory>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wconversion"
#include "tensorflow/core/public/session.h"
#pragma clang diagnostic pop

namespace ac {

class TensorflowModel {
public:

    enum class Status { kSuccess = 0, kFailedSession = -1, kFailedReadGraph = -2, kFailedRun = -3, kInvalidInput = -4 };

    TensorflowModel();

    Status InitializeFromGraph(const std::string& filename);

    ~TensorflowModel();

    const std::string& error_string() { return this->error_string_; }
protected:
    TensorflowModel(const TensorflowModel& model) = delete;
    void operator=(const TensorflowModel& model) = delete;

    std::unique_ptr<tensorflow::Session> session_;
    std::string error_string_;
};

}
#endif //NEURAL_SUITE_TENSORFLOW_MODEL_H