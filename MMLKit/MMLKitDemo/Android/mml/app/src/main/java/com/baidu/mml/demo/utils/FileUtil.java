package com.baidu.mml.demo.utils;

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
                copyFileFromAssets(context,oldPath,newPath);
            }
        } catch (Exception e ) {
            e.printStackTrace();
        }

    }

    public static void copyFileFromAssets(Context context, String assetsFile, String outFile) throws IOException {
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
