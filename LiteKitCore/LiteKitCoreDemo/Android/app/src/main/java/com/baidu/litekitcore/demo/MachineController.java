package com.baidu.litekitcore.demo;

import android.util.Log;
import android.view.View;

import com.baidu.litekitcore.LiteKitBaseMachine;
import com.baidu.litekitcore.LiteKitData;
import com.baidu.litekitcore.LiteKitMachineConfig;
import com.baidu.litekitcore.LiteKitMachineService;
import com.baidu.litekitcore.LiteKitPaddleConfig;
import com.baidu.litekitcore.LiteKitPaddleLiteConfig;

import java.util.ArrayList;

public class MachineController implements View.OnClickListener {

    public static final int modelInputBatchSize = 1;
    public static final int modelInputChannel = 3;
    public static final int modelInputWidth = 256;
    public static final int modelInputHeight = 256;
    public static String Tag = "LiteKitDemo";

    MachineController(String modelPath, float[] inputData) {
        mModelPath = modelPath;
        mInputData = inputData;
    }
    String mModelPath;
    float[] mInputData;
    LiteKitBaseMachine mMachine;
    ArrayList<LiteKitData> mInput;
    ArrayList<LiteKitData> mOutput;

    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.button1: {
                // create
                Log.d(Tag, " >>>>>>>>>   create machine   <<<<<<<<< ");
                if (mMachine != null) {
                    this.releaseMachine();
                }
                this.createMachine();
                Log.d(Tag, " ^^^^^^^^^^^^^^^^^^^^^^^^^^^ ");
                break;
            }
            case R.id.button2: {
                // set input
                Log.d(Tag, " >>>>>>>>>   create input   <<<<<<<<< ");
                this.createInput();
                Log.d(Tag, " ^^^^^^^^^^^^^^^^^^^^^^^^^^^ ");
                break;
            }
            case R.id.button3: {
                // predict
                Log.d(Tag, " >>>>>>>>>   predict   <<<<<<<<< ");
                this.predict();
                Log.d(Tag, " ^^^^^^^^^^^^^^^^^^^^^^^^^^^ ");
                break;
            }
            case R.id.button4: {
                // get output
                Log.d(Tag, " >>>>>>>>>   fetch output   <<<<<<<<< ");
                this.fetchOutput();
                Log.d(Tag, " ^^^^^^^^^^^^^^^^^^^^^^^^^^^ ");
                break;
            }
            case R.id.button5: {
                // release
                Log.d(Tag, " >>>>>>>>>   release machine   <<<<<<<<< ");
                this.releaseMachine();
                Log.d(Tag, " ^^^^^^^^^^^^^^^^^^^^^^^^^^^ ");
                break;
            }

            default:
                break;
        }
    }

    void createMachine() {
        // 创建LiteKitMachine
        LiteKitMachineConfig machineConfig = new LiteKitMachineConfig();
        LiteKitPaddleConfig paddleConfig = new LiteKitPaddleConfig();
        paddleConfig.liteConfig = new LiteKitPaddleLiteConfig();
        machineConfig.modelPath = mModelPath;
        machineConfig.machineType = LiteKitMachineConfig.MachineType.LiteKitPaddleLite;
        machineConfig.engineConifg = paddleConfig;
        mMachine = LiteKitMachineService.loadMachineWithConfig(machineConfig);
        Log.d(Tag, mMachine == null ? "create fail\n" : "create succeed\n");
    }

    void createInput() {
        if (mMachine != null) {
            // 预处理数据
            float[] inputData = mInputData;
            logBuffer(inputData, inputData.length, 20);

            if (inputData.length != modelInputBatchSize * modelInputChannel * modelInputHeight * modelInputWidth) {
                Log.e(Tag, "input data error");
                System.exit(-1);
            }
            // 组装LiteKitData输入
            mInput = new ArrayList<>();
            LiteKitData data = new LiteKitData(inputData, modelInputBatchSize, modelInputChannel, modelInputHeight, modelInputWidth, 0);
            mInput.add(data);
            Log.d(Tag, "create input succeed, length="+String.valueOf(mInput.size())+"\n");
        } else {
            Log.d(Tag, "machine is null");
        }
    }

    void predict() {
        if (mMachine != null && mInput != null) {
            mOutput = mMachine.predictWithInputData(mInput);
            Log.d(Tag, mOutput == null ?
                    "predict fail\n" :
                    "predict succeed, length=" + String.valueOf(mOutput.size()) + "\n");
        } else if (mMachine == null) {
            Log.d(Tag, "machine is null");
        } else if (mInput == null) {
            Log.d(Tag, "input is null");
        }
    }

    void fetchOutput() {
        if (mOutput != null) {
            float[] result = mOutput.get(0).output.fetchFloatData();
            logBuffer(result, result.length, 20);
            Log.d(Tag, result == null ? "fetch output fail\n" : "fetch output succeed\n");
        } else {
            Log.d(Tag, "mOutput is null need predict");
        }
    }

    void releaseMachine() {
        if (mMachine != null) {
            mMachine.releaseMachine();
            mMachine = null;
            mOutput = null;
            Log.d(Tag, "release machine succeed");
        } else {
            Log.d(Tag, "machine is null");
        }
    }

    public static void logBuffer(float[] buffer , int length, int count) {
        int stride = length / count;
        if (stride == 0) {
            stride = 1;
        }

        for (int j = 0; j < length / stride; j++) {
            Log.d("logBuffer", String.format("%.6f",buffer[j * stride]));
        }
        Log.d("logBuffer","\n");
    }

}
