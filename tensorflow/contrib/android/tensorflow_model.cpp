#include "tensorflow/contrib/android/tensorflow_model.h"

#include <fstream>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wconversion"
#include "google/protobuf/io/coded_stream.h"
#include "google/protobuf/io/zero_copy_stream_impl.h"
#include "google/protobuf/io/zero_copy_stream_impl_lite.h"
#include "google/protobuf/message_lite.h"
#include "tensorflow/core/framework/op_kernel.h"
#include "tensorflow/core/framework/tensor.h"
#include "tensorflow/core/framework/types.pb.h"
#include "tensorflow/core/platform/env.h"
#include "tensorflow/core/platform/logging.h"
#include "tensorflow/core/platform/mutex.h"
#include "tensorflow/core/platform/types.h"
#include "tensorflow/core/public/session.h"
#include "tensorflow/core/public/version.h"
#pragma clang diagnostic pop

#define EXPECTED_TF_MAJOR 0
#define EXPECTED_TF_MINOR 12
#define EXPECTED_TF_REVISION 1

#if (EXPECTED_TF_MAJOR != TF_MAJOR_VERSION \
    || EXPECTED_TF_MINOR != TF_MINOR_VERSION \
    || EXPECTED_TF_REVISION != TF_PATCH_VERSION)
#error "Wrong Tensorflow headers (run /scripts/download_dependencies.sh?)"
#endif

namespace ac {

class IfstreamInputStream : public ::google::protobuf::io::CopyingInputStream {
public:
    explicit IfstreamInputStream(const std::string& file_name)
            : ifs_(file_name.c_str(), std::ios::in | std::ios::binary) {}
    ~IfstreamInputStream() { ifs_.close(); }

    int Read(void* buffer, int size) {
        if (!ifs_) {
            return -1;
        }
        ifs_.read(static_cast<char*>(buffer), size);
        return static_cast<int>(ifs_.gcount());
    }

private:
    std::ifstream ifs_;
};

bool PortableReadFileToProto(const std::string& file_name,
                             ::google::protobuf::MessageLite* proto) {
    ::google::protobuf::io::CopyingInputStreamAdaptor stream(
            new IfstreamInputStream(file_name));
    stream.SetOwnsCopyingStream(true);
    // TODO(jiayq): the following coded stream is for debugging purposes to allow
    // one to parse arbitrarily large messages for MessageLite. One most likely
    // doesn't want to put protobufs larger than 64MB on Android, so we should
    // eventually remove this and quit loud when a large protobuf is passed in.
    ::google::protobuf::io::CodedInputStream coded_stream(&stream);
    // Total bytes hard limit / warning limit are set to 1GB and 512MB
    // respectively.
    coded_stream.SetTotalBytesLimit(1024LL << 20, 512LL << 20);
    return proto->ParseFromCodedStream(&coded_stream);
}

TensorflowModel::Status TensorflowModel::InitializeFromGraph(const std::string& filename) {
    if (!std::ifstream(filename)) {
        this->error_string_ = "Could not read from file: " + filename;
        return Status::kFailedReadGraph;
    }

    tensorflow::SessionOptions options;
    tensorflow::Session* session_ptr = nullptr;
    tensorflow::Status session_status = tensorflow::NewSession(options, &session_ptr);
    if (!session_status.ok()) {
        this->error_string_ = "Failed to initialize session: " + session_status.ToString();
        return Status::kFailedSession;
    }

    session_ = std::unique_ptr<tensorflow::Session>(session_ptr);

    tensorflow::GraphDef graph;
    PortableReadFileToProto(filename, &graph);
    tensorflow::Status graph_status = session_->Create(graph);

    if (!graph_status.ok()) {
        this->error_string_ = "Failed to read graph: " + graph_status.ToString();
        return Status::kFailedReadGraph;
    }

    return TensorflowModel::Status::kSuccess;
}

TensorflowModel::TensorflowModel() {
    session_ = nullptr;
}

TensorflowModel::~TensorflowModel() {
    if (session_ != nullptr) {
        session_->Close();
    }
}

}
