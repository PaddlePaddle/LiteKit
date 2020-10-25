package com.baidu.mmlcore;

import java.util.ArrayList;

abstract public class MMLBaseMachine {
    /**
     * Predict with input data
     *
     * */
    public abstract ArrayList<MMLData> predictWithInputData(ArrayList<MMLData> aInputData);

    /**
     * Run the Machine.
     *
     * */
    public native void run(long nativeMachineHandler);

    /**
     * Delete C++ Machine object pointed by the input pointer, which is
     * presented by a long value.
     *
     */
    public native void release(long nativeMachineHandler);

    /** name of C++ JNI lib */
    private final static String JNI_LIB_NAME = "mml-core";

    /**
     * Initializes at the beginning
     * loads the C++ JNI lib. */
    static {
        System.loadLibrary(JNI_LIB_NAME);
    }

    /**
     * Java doesn't have pointer. To maintain the life cycle of underneath C++
     * Machine object, we use a long value to maintain it.
     */
    protected long nativeMachineHandler = 0L;

    /**
     * Deletes C++ Machine pointer when Java Machine object is
     * destroyed
     */
    @Override
    protected void finalize() throws Throwable {
        releaseMachine();
        super.finalize();
    }

    /**
     * Deletes C++ Machine pointer
     *
     */
    public void releaseMachine() {
        if (nativeMachineHandler != 0L) {
            release(nativeMachineHandler);
            nativeMachineHandler = 0L;
        }
    }
}
