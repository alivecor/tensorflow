
#include "tensorflow/contrib/android/identity_model.h"
#include <fstream>
#include <iostream>

using Status = ac::IdentityModel::Status;

int main(const int argc, const char** argv) {
    if (argc != 3) {
        std::cerr << "Usage: modelPath filename\n";
        return -1;
    }

    const std::string modelPath(argv[1]);
    const std::string filename(argv[2]);

    ac::IdentityModel model;
    Status status = model.InitializeFromGraph(modelPath);

    if (status != Status::kSuccess) {
        std::cerr << "Failed to initialize model\n";
        std::cerr << model.error_string() << "\n";
        return -1;
    }

    std::ifstream fileIn(filename, std::ios::binary | std::ios::in | std::ios::ate);
    if (!fileIn.is_open()) {
        std::cerr << "Could not open binary file\n";
        return -2;
    }

    long fileSize = fileIn.tellg();
    long numSamples = fileSize / sizeof(float);
    fileIn.seekg(0);

    std::cout << "Number of samples: " << numSamples << "\n";
    if (numSamples < model.min_ecg_length()) {
        std::cerr << "File too short\n";
        return -3;
    }

    std::vector<float> ecg_data(static_cast<unsigned long>(numSamples));
    fileIn.read(reinterpret_cast<char*>(ecg_data.data()), fileSize);

    ac::IdentityResult result;
    status = model.CalculateIdentityProbability(ecg_data, &result);

    if (status != Status::kSuccess) {
        std::cerr << "Failed to run model\n";
        std::cerr << model.error_string() << "\n";
        return -4;
    }

    std::cout << "ID probability: " << result.identity_probability << "\n";
}