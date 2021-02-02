# LiteKitCore接口文档(for Native C++ API)
## litekit_inference_api
header文件名：litekit_inference_api.h
说明：包含input/output数据结构的定义、Service的定义、类型枚举的定义、config结构等的定义。
```cpp

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


    /// LiteKit string container
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

    // LiteKit config params
    struct LiteKitConfig {
        // default model & param file name
        const std::string MODEL_FILE_NAME = "model.mlm";
        const std::string PARAM_FILE_NAME = "params.mlm";

        enum Precision {
            FP32 = 0
        };
        // Machine type，PaddleLite、PaddleiOSGPU
        enum MachineType {
            PaddleLite = 2, PaddleiOSGPU = 3
        };

        // model input data precision
        Precision precision = FP32;

        // model file path
        std::string modelUrl;

        // model & param file name
        std::string model_file_name = "";
        std::string param_file_name = "";

        // backend type
        MachineType machine_type = MachineType::PaddleLite;

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

        // PaddleiOSGPU special config （ios only）
        struct PaddleiOSGPUConfig {
            //model types,GPU backend will set input data shape automatic, instead of shape in LiteKitData
            enum NetType {
                Default = -1, SuperResolution = 0, MobileNetSSD = 1, Gan = 2, Shuffle = 3
            };
            NetType type = Default;
        };

        // specific machine config
        union MachineConfig {
            PaddleLiteConfig paddle_lite_config;
            PaddleiOSGPUConfig paddle_ios_gpu_config;
        };
        // config for a specific machine
        MachineConfig machine_config = {PaddleLiteConfig()};

    };

    enum LiteKitMachineType {
        // Android support PADDLE_LITE only
        PADDLE_LITE = 1,
        PADDLE_iOSGPU = 2,
    };

    // types control in LiteKitTensor
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

        // if autoRelease = true，will release rawData when destruct
        virtual ~LiteKitTensor();

        // release rawData，if autoRelease = true，no need to be called
        void release();
    };

    // LiteKit data struct
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

        // data prepare to predict，corresponding to input_tensor.data
        void *rawData = nullptr;
        // rawData length，unit is byte
        long dataLength;
        RawDataType rawDataType;
        // corresponding to input_tensor.shape，unnecessary
        RawDataShape rawDataShape;
        // if true call release() when destruct
        bool autoRelease = true;
        //tensor
        LiteKitTensor *litekitTensor = 0;

        LiteKitData() = default;

        // if autoRelease=true，will release rawData when destruct
        virtual ~LiteKitData();

        // delete rawData、litekitTensor，if autoRelease=true，no need to be called
        void release();
    };

    /**
     * Preprocess/Postproocess Interface
     */
    class LiteKitDataProcessor {
    public:
        /**
         * Preprocess call back
         * @param preProcessInputData preprocess input data
         * @param preProcessOutputData preprocess output data
         * @return ErrorCode
         */
        virtual int
        preProcess(const LiteKitData &preProcessInputData, LiteKitData *preProcessOutputData) = 0;

        /**
         * Postprocess call back
         * @param postProcessInputData postprocess input data
         * @param postProcessOutputData postprocess output data
         * @return ErrorCode
         */
        virtual int
        postProcess(const LiteKitData &postProcessInputData, LiteKitData *postProcessOutputData) = 0;

        virtual ~LiteKitDataProcessor() {};
    };

    /**
     * LiteKitMachine Manager。LiteKitMachine means a engine that loaded a kind of inference(for example :PaddleLite)
     */
    class LiteKitMachineService {
    public:
        std::unique_ptr<litekit_framework::LiteKitData> getInputData(int i);

        std::unique_ptr<const litekit_framework::LiteKitData> getOutputData(int i);

        std::vector<std::string> getInputNames();

        std::vector<std::string> getOutputNames();

        std::unique_ptr<litekit_framework::LiteKitData> getInputByName(const std::string &name);

    private:
        // MachinePredictor pointer
        void *machineHandle = nullptr;
        // MachinePredictor type，like PaddleLite, PaddleiOSGPU..
        LiteKitMachineType litekitMachineType;
        // preprocess/postprocess callback
        LiteKitDataProcessor *mProcessorImpl = nullptr;

        /**
         * predict(without preprocess/postprocess)
         *
         * @param modelInputData input data
         * @param modelOutputData output data
         * @return ErrorCode
         */
        int predict(LiteKitData &modelInputData, LiteKitData *modelOutputData);

        int predict();

    public:

        /**
         *  if autoRelease=true，will release machineHandle when destruct
         */
        bool autoRelease = true;

        /**
         * set preprocess/postprocess impl
         *
         * @param processorImpl processor impl
         */
        void setInterceptProcessor(LiteKitDataProcessor *processorImpl);

        /**
         * predict function，if InterceptProcessor setted，will call preprocess of InterceptProcessor before predict，
         * then call postprocess of InterceptProcessor after。if InterceptProcessor not setted，will call predict
         *
         * @param inputData input datta
         * @param outputData output data
         * @return ErrorCode
         */
        int run(LiteKitData &inputData, LiteKitData *outputData);

        int run();

        /**
         * according to LiteKitConfig，create MachinePredictor，and load model
         *
         * @param config config
         * @return ErrorCode
         */
        int load(const LiteKitConfig &config);

        /**
         * if autoRelease=true，will delete mProcessorImpl and predictor of machineHandle when destruct
         */
        virtual ~LiteKitMachineService();

        /**
        * release mProcessorImpl and machineHandle，if autoRelease=true，no need to be called
        */
        void release();
    };

    /**
     * Errorcode，succeed/param error/engine load error
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


```
