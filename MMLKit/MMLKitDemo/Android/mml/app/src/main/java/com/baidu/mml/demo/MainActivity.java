package com.baidu.mml.demo;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;

import com.baidu.mml.demo.utils.FileUtil;

import java.io.File;

import static com.baidu.mml.demo.FileManager.modelDir;
import static com.baidu.mml.demo.FileManager.sourceDir;

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
        FileUtil.copyFilesFromAssets(this, modelDir(), targetModelPath + modelDir());
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

}