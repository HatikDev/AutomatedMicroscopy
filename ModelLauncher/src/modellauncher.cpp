#include "modellauncher.h"

#include <random>

#include <filesystem>
#include "nn/onnx_model_base.h"
#include "nn/autobackend.h"
#include <opencv2/opencv.hpp>
#include <vector>
#include <fstream>

#include "utils/augment.h"
#include "constants.h"
#include "utils/common.h"

namespace fs = std::filesystem;

namespace {
    std::unique_ptr<AutoBackendOnnx> model; // TODO: replace by pointer to base class

    const char* kOnnxLogid = "yolov8_inference2";
    const char* onnxProvider = OnnxProviders::CPU;

    // TODO: think about serializer
    // TODO: replace YoloResults by common type
    void writeResults(const char* output, cv::Mat& image, const std::vector<YoloResults>& results) {
        std::ofstream file(output, std::ios::out);

        // TODO: make constants dynamic
        const float kWidth = 640;
        const float kHeight = 480;

        for (auto& obj : results) {
            float x0 = obj.bbox.x / kWidth;
            float y0 = obj.bbox.y / kHeight;
            float x1 = (obj.bbox.x + obj.bbox.width) / kWidth;
            float y1 = (obj.bbox.y + obj.bbox.height) / kHeight;

            file << obj.class_idx << " " << x0 << " " << y0 << " " << x1 << " " << y1 << std::endl;
        }
    }
}

namespace loader
{
    void __cdecl loadModel(const char* modelPath, const char* datasetPath) {
        if (model)
            return; // TODO: add throwing exception if the model already was loaded

        model = std::make_unique<AutoBackendOnnx>(modelPath, datasetPath, kOnnxLogid, onnxProvider);
    }

    void __cdecl predict(const char* input, const char* output) {
        cv::Mat img = cv::imread(input, cv::IMREAD_UNCHANGED);
        if (img.empty())
            return; // TODO: add throwing exception

        float kMaskThreshold = 0.5f;  // in python it's 0.5 and you can see that at ultralytics/utils/ops.process_mask line 705 (ultralytics.__version__ == .160)
        float kConfThreshold = 0.30f;
        float kIouThreshold = 0.45f;  //  0.70f;
        int conversion_code = cv::COLOR_BGR2RGB;

        std::vector<YoloResults> objs = model->predict_once(img, kConfThreshold, kIouThreshold, kMaskThreshold, conversion_code);

        writeResults(output, img, objs);
    }

    void __cdecl unloadModel() {
        model.reset();
    }
}
