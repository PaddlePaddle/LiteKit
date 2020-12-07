/**
 *
 */
#ifndef paddle_mobile_gpu_interface_h
#define paddle_mobile_gpu_interface_h

#include <string>
#include <vector>

namespace paddle_moible_gpu {

///配置参数
struct PaddleMobileGPUModelConfig {
    /// 全路径的模型和参数文件名
    std::string model_path = "";
    std::string param_path = "";

    /// 支持的模型类型 特定的类型已做特定支持 比如输入维度、优化参数已设置好
    enum NetType{ Default = -1, SuperResolution = 0, MobileNetSSD = 1, GanNet = 2, ShuffleNet = 3 };
    NetType netType = Default;
};

struct PaddleMobileGPUData {
    void* data{nullptr};
    long length{0};
    std::vector<int> shape; ///nchw
};

class PaddleMobileGPUPredictor {
public:
    PaddleMobileGPUPredictor();
    ~PaddleMobileGPUPredictor();
    void init(const PaddleMobileGPUModelConfig &config);
    bool load();
    bool predict(PaddleMobileGPUData& input , std::vector<PaddleMobileGPUData> *outputs);
private:
    void * ocObj;
};

std::shared_ptr<PaddleMobileGPUPredictor> createPaddleGPUPredictor(const PaddleMobileGPUPredictor &config);

}
#endif /* paddle_mobile_gpu_interface_h */
