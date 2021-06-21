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

package com.baidu.litekit.demo;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;

import com.baidu.litekit.demo.utils.FileUtil;

import java.io.File;

import static com.baidu.litekit.demo.FileManager.sourceDir;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        getSupportActionBar().hide();

        // 将文件从assets拷贝到file
        prepareResources();
    }

    /**
     * @desc 将assets文件拷贝到
     */
    private void prepareResources() {
        String targetModelPath = (this.getFilesDir().getAbsolutePath() + File.separator);
        // 拷贝超分需要使用的资源到assert
        FileUtil.copyFilesFromAssets(this, sourceDir(), targetModelPath + sourceDir());
    }

    public void onHandGestureDetection(View v) {
        startActivity(new Intent(MainActivity.this, HandGestureRecognizeActivity.class));
    }
    public void onImageSuperResolution(View v) {
        startActivity(new Intent(MainActivity.this, VideoSuperResolutionActivity.class));
    }
    public void onPortraitSegmentation(View v) {
        startActivity(new Intent(MainActivity.this, PortraitSegmentationActivity.class));
    }
    public void onOCR(View v) {
        startActivity(new Intent(MainActivity.this, OCRActivity.class));
    }
}