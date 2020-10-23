package com.baidu.mml.demo.utils;

import android.util.Log;

import java.util.concurrent.Executors;
import java.util.concurrent.ThreadPoolExecutor;

/**
 * Usage: ThreadManager.getPool().execute(runnable);
 *
 */
public class ThreadManager {
    private static ThreadPoolExecutor mPoolExecutor;
    private static Object mPoolock = new Object();
    public static Object mCommonlock = new Object();

    public static ThreadPoolExecutor getPool() {
        if (mPoolExecutor == null) {
            synchronized (mPoolock) {
                if (mPoolExecutor == null) {
                    mPoolExecutor = (ThreadPoolExecutor) Executors.newFixedThreadPool(1);
                }
            }
        }
        return mPoolExecutor;
    }
}