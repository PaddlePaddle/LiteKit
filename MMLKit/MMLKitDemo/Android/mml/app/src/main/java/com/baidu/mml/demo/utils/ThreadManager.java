/* Copyright (c) 2019 PaddlePaddle Authors. All Rights Reserved.
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License. */

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