/**
Copyright © 2020 Baidu, Inc. All Rights Reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/*
 * LiteKit framework
 */
#ifndef LIB_AI_LITEKIT_INTERFACE_API_H
#define LIB_AI_LITEKIT_INTERFACE_API_H

#include <string>

namespace litekit_framework {
//lite backend Target Type
    enum class LiteKitTargetType : int {
        kUnk = 0,
        kHost = 1,
        kX86 = 2,
        kCUDA = 3,
        kARM = 4,
        kOpenCL = 5,
        kFPGA = 7,
        kNPU = 8,
        kXPU = 9,
        kAny = 6,  // any target
        NUM = 10,  // number of fields.
    };


    /// LiteKit string容器
    struct LiteKitString {
        const char *data;
        size_t size;
    };

    struct LiteKitModelBuffer {
        char *model_buffer;
        size_t model_buffer_size;
        char *param_buffer;
        size_t param_buffer_size;
    };

    // LiteKit配置相关参数
    struct LiteKitConfig {
        // 默认的模型和参数文件名
        const std::string MODEL_FILE_NAME = "model.mlm";
        const std::string PARAM_FILE_NAME = "params.mlm";

        enum Precision {
            FP32 = 0
        };
        // Machine类型，PaddleLite、PaddleiOSGPU
        enum MachineType {
            PaddleLite = 2, PaddleiOSGPU = 3
        };

        // 模型输入的数据精度
        Precision precision = FP32;

        // 模型文件路径
        std::string modelUrl;

        // 模型和参数文件名
        std::string model_file_name = "";
        std::string param_file_name = "";

        // 后端类型
        MachineType machine_type = MachineType::PaddleLite;

        // paddle可支持在模型之外添加的前后处理类型
        enum PrePostType {
            NONE_PRE_POST = 0,  // none
            UINT8_255 = 1       // 输入数据归一化
        };

        // Paddle lite config
        struct PaddleLiteConfig {
            typedef enum {
                LITE_POWER_HIGH = 0,
                LITE_POWER_LOW = 1,
                LITE_POWER_FULL = 2,
                LITE_POWER_NO_BIND = 3,
                LITE_POWER_RAND_HIGH = 4,
                LITE_POWER_RAND_LOW = 5
            } PaddleLitePowerMode;
            PaddleLitePowerMode powermode = {PaddleLitePowerMode::LITE_POWER_NO_BIND};
            int threads = {1};

            typedef enum {
                LITE_MODEL_FROM_DIR = 0,
                LITE_MODEL_FROM_FILE = 1,
                LITE_MODEL_FROM_BUFFER = 2,
                LITE_MODEL_FROM_MODELBUFFER = 3,
            } PaddleLiteModelType;
            PaddleLiteModelType model_type = {PaddleLiteModelType::LITE_MODEL_FROM_DIR};

            union PaddleLiteModel {
                LiteKitString model_from_file; // used when LITE_MODEL_FROM_FILE
                LiteKitString model_from_buffer; // used whtn LITE_MODEL_FROM_BUFFER
                LiteKitModelBuffer model_buffer; // used when LITE_MODEL_FROM_MODELBUFFER
            };
            PaddleLiteModel model;
        };

        // PaddleiOSGPU special config （ios特有）
        struct PaddleiOSGPUConfig {
            //支持的模型类型 设置了类型 gpu层自动设置输入维度 LiteKitData中shape不需要了
            enum NetType {
                Default = -1, SuperResolution = 0, MobileNetSSD = 1, Gan = 2, Shuffle = 3
            };
            NetType type = Default;
        };

        // 具体machine的配置
        union MachineConfig {
            PaddleLiteConfig paddle_lite_config;
            PaddleiOSGPUConfig paddle_ios_gpu_config;
        };
        // 专属于某个machine的配置
        MachineConfig machine_config = {PaddleLiteConfig()};

    };

    enum LiteKitMachineType {
        // 暂时只支持PADDLE_LITE
        PADDLE_LITE = 1,
        PADDLE_iOSGPU = 2,
    };

    // LiteKitTensor中使用的一些控制类型
    using shape_t = std::vector<int64_t>;
    using lod_t = std::vector<std::vector<uint64_t>>;

    //LiteKit Tensor,
    class LiteKitTensor {
    public:
        explicit LiteKitTensor(void *raw);

        explicit LiteKitTensor(const void *raw);

        LiteKitTensor() = default;

        void *tensor = 0;

        void Resize(const shape_t &shape);

        void SetLoD(const lod_t &lod);

        /// Readonly data. support int \ float \ int8_t \ uint8_t \ int64_t
        template<typename T>
        const T *data() const;

        template<typename T>
        T *mutable_data(LiteKitTargetType type = LiteKitTargetType::kHost) const;

        shape_t shape() const;

        lod_t lod() const;

        bool autoRelease = true;

        // 如果autoRelease为true，则会在析构时释放rawData
        virtual ~LiteKitTensor();

        // 释放rawData，如果autoRelease为true，则不需要主动调用
        void release();
    };

    // LiteKit配置相关参数
    struct LiteKitData {
        enum RawDataType {
            FLOAT32,
            UINT8,
            PADDLE_LITE_TENSOR
        };

        struct RawDataShape {
            int n = 1;
            int c = 1;
            int h;
            int w;

        public:
            RawDataShape() = default;

            RawDataShape(int n, int c, int h, int w) {
                this->n = n;
                this->c = c;
                this->h = h;
                this->w = w;
            }
        };

        // 待预测的数据，对应input_tensor.data
        void *rawData = nullptr;
        // rawData长度，单位byte
        long dataLength;
        RawDataType rawDataType;
        // 对应input_tensor.shape，非必须
        RawDataShape rawDataShape;
        // 若为true 析构时自动调用release()
        bool autoRelease = true;
        //tensor
        LiteKitTensor *litekitTensor = 0;

        LiteKitData() = default;

        // 如果autoRelease为true，则会在析构时释放rawData
        virtual ~LiteKitData();

        // delete rawData、litekitTensor，如果autoRelease为true，则不需要主动调用
        void release();
    };

    /**
     * 前后处理接口类
     */
    class LiteKitDataProcessor {
    public:
        /**
         * 前处理回调
         * @param preProcessInputData
         * @param preProcessOutputData
         * @return
         */
        virtual int
        preProcess(const LiteKitData &preProcessInputData, LiteKitData *preProcessOutputData) = 0;

        /**
         * 后处理回调
         * @param postProcessInputData
         * @param postProcessOutputData
         * @return
         */
        virtual int
        postProcess(const LiteKitData &postProcessInputData, LiteKitData *postProcessOutputData) = 0;

        virtual ~LiteKitDataProcessor() {};
    };

    /**
     * LiteKitMachine管理类。我们把加载了某个模型的一种Inference引擎（如PaddleLite）称做一个LiteKitMachine。
     */
    class LiteKitMachineService {
    public:
        std::unique_ptr<litekit_framework::LiteKitData> getInputData(int i);

        std::unique_ptr<const litekit_framework::LiteKitData> getOutputData(int i);

        std::vector<std::string> getInputNames();

        std::vector<std::string> getOutputNames();

        std::unique_ptr<litekit_framework::LiteKitData> getInputByName(const std::string &name);

    private:
        // MachinePredictor指针
        void *machineHandle = nullptr;
        // MachinePredictor类型，如PaddleLite, PaddleiOSGPU等
        LiteKitMachineType litekitMachineType;
        // 前后处理回调实现
        LiteKitDataProcessor *mProcessorImpl = nullptr;

        /**
         * 预测函数，不包含前后处理
         *
         * @param modelInputData
         * @param modelOutputData
         * @return
         */
        int predict(LiteKitData &modelInputData, LiteKitData *modelOutputData);

        int predict();

    public:

        /**
         * 如果autoRelease为true，则会在析构时释放machineHandle
         */
        bool autoRelease = true;

        /**
         * 设置前后处理回调实现
         *
         * @param processorImpl
         */
        void setInterceptProcessor(LiteKitDataProcessor *processorImpl);

        /**
         * 预测函数，如果设置了InterceptProcessor，则会先执行InterceptProcessor的前处理回调，再执行predict，
         * 再执行InterceptProcessor的后处理回调。如果未设置InterceptProcessor，则会直接执行predict
         *
         * @param inputData
         * @param outputData
         * @return
         */
        int run(LiteKitData &inputData, LiteKitData *outputData);

        int run();

        /**
         * 根据LiteKitConfig配置，创建MachinePredictor，并加载模型
         *
         * @param config
         * @return
         */
        int load(const LiteKitConfig &config);

        /**
         * 如果autoRelease为true，则会在析构时delete mProcessorImpl以及machineHandle指向的predictor
         */
        virtual ~LiteKitMachineService();

        /**
        * 释放mProcessorImpl与machineHandle，如果autoRelease为true，则不需要主动调用
        */
        void release();
    };

    /**
     * 模型加载错误，成功/解密错误/参数错误/引擎创建错误
     */
    enum ErrorCode {
        SUCCESS = 0,
        ERR_PARAM = -1,

        LOAD_ERR_OTHER = -11,
        LOAD_ERR_MACHINE_TYPE = -13,

        RUN_ERR_OTHER = -20,
        RUN_ERR_PREPROECESS = -21,
        RUN_ERR_POSTPROECESS = -22,
        RUN_ERR_PREDICT = -23,
        RUN_ERR_MACHINE_TYPE = -24,
        RUN_ERR_MACHINE_HANDLE = -25,

    };

    std::shared_ptr<LiteKitMachineService> CreateLiteKitMachineService(LiteKitConfig &config);

}

#endif //LIB_AI_LITEKIT_INTERFACE_API_H
