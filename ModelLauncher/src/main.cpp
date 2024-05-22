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
}

// TODO: move them somewhere
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

        file << "1 " << x0 << " " << y0 << " " << x1 << " " << y1 << std::endl;
    }
}

void loadModel(const char* modelPath) {
    if (model)
        return; // TODO: add throwing exception if the model already was loaded

    model = std::make_unique<AutoBackendOnnx>(modelPath, kOnnxLogid, onnxProvider);
}

void predict(const char* input, const char* output) {
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

void unloadModel() {
    model.reset();
}

int main()
{
    //std::string imgPath = "C:/Users/doroshin/Documents/projects/my_yolov8/images/000000000382.jpg";
    std::string imgPath = "C:/Users/doroshin/Documents/projects/my_yolov8/images/test.jpg";
    const std::string& modelPath = "C:/Users/doroshin/Documents/projects/my_yolov8/best.onnx"; // detection
    //const std::string& modelPath = "C:/Users/doroshin/Documents/projects/my_yolov8/checkpoints/yolov8n.onnx"; // detection
    // vs:
    //    const std::string& modelPath = "./checkpoints/yolov8n-seg.onnx"; // instance segmentation
    // clion:
    //const std::string& modelPath = "C:/Users/doroshin/Documents/projects/yolov8-onnx-cpp/checkpoints/yolov8n-pose.onnx"; // pose

    const char* output = "results.txt";

    loadModel(modelPath.c_str());
    predict(imgPath.c_str(), output);
    unloadModel();

    return 0;
}
