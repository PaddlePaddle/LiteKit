/*
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

package com.baidu.litekitcore.demo.utils;

import android.content.Context;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;

public class FileUtil {

    /**
     * 从assert拷贝文件工具方法
     *
     * @param context Context
     * @param oldPath 文件原地址
     * @param newPath 文件copy目标地址
     *
     */
    public static void copyFilesFromAssets(Context context, String oldPath, String newPath) {
        try {
            String[] fileNames = context.getAssets().list(oldPath);
            if (fileNames != null && fileNames.length != 0) {
                // directory
                File file = new File(newPath);
                file.mkdirs();
                // copy recursivelyC
                for (int i=0 ; i< fileNames.length; i++) {
                    String fileName = fileNames[i];
                    copyFilesFromAssets(context,
                            oldPath+File.separator+fileName,
                            newPath+File.separator+fileName);
                }
            } else {
                // file
                InputStream iss = context.getAssets().open(oldPath);
                FileOutputStream fos = new FileOutputStream(new File(newPath));
                byte[] bytes = new byte[1024];
                while (iss.read(bytes) != -1) { fos.write(bytes); }
                iss.close();
                fos.close();
            }
        } catch (Exception e ) {
            e.printStackTrace();
        }

    }

    public static void copyAssetResource2File(Context context, String assetsFile, String outFile) throws IOException {
        InputStream is = context.getAssets().open(assetsFile);
        File outF = new File(outFile);
        FileOutputStream fos = new FileOutputStream(outF);

        int byteCount;
        byte[] buffer = new byte[1024];
        while ((byteCount = is.read(buffer)) != -1) {
            fos.write(buffer, 0, byteCount);
        }
        fos.flush();
        is.close();
        fos.close();
        outF.setReadable(true);
    }

}
