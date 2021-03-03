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

import android.Manifest;
import android.content.pm.PackageManager;
import android.graphics.Bitmap;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.provider.Settings;
import android.support.annotation.NonNull;
import android.support.v7.app.ActionBar;
import android.support.v7.app.AppCompatActivity;
import android.view.KeyEvent;
import android.view.Menu;
import android.view.MenuItem;
import android.view.OrientationEventListener;
import android.widget.Toast;

import com.baidu.litekit.demo.utils.ImageUtil;
import com.baidu.litekit.demo.utils.PermissionUtils;
import com.baidu.litekit.demo.view.CameraView;

public abstract class CameraBaseActivity extends AppCompatActivity {

    protected CameraView mCameraView;

    protected int mPreviewWidth;
    protected int mPreviewHeight;

    private OrientationEventListener mOrientationListener;
    // 设备旋转角度：0/90/180/360
    protected int rotateDegree;

    abstract void onKitCreate();

    abstract void doKitInfer();

    abstract String actionBarTitle();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_video_base);
        // 监听屏幕旋转
        detectScreenRotate();
        // 设置工具栏标题, 显示返回键
        ActionBar actionBar = getSupportActionBar();
        if (actionBar != null) {
            actionBar.setTitle(actionBarTitle());
            actionBar.setHomeButtonEnabled(true);
            actionBar.setDisplayHomeAsUpEnabled(true);
        }
        // 创建Kit实例
        onKitCreate();
        // Android M动态申请权限，摄像头+存储访问
        PermissionUtils.requestPermission(this, new String[]{Manifest.permission.CAMERA, Manifest
                .permission.WRITE_EXTERNAL_STORAGE}, 10, initViewRunnable);
    }

    /**
     * 监听屏幕旋转
     */
    private void detectScreenRotate() {
        mOrientationListener = new OrientationEventListener(this,
                SensorManager.SENSOR_DELAY_NORMAL) {
            @Override
            public void onOrientationChanged(int orientation) {

                if (orientation == OrientationEventListener.ORIENTATION_UNKNOWN) {
                    return;  //手机平放时，检测不到有效的角度
                }

                //可以根据不同角度检测处理，这里只检测四个角度的改变
                orientation = (orientation + 45) / 90 * 90;

                if (screenAutoRotate() && orientation % 360 == 180) {
                    return;
                }

                rotateDegree = orientation % 360;
            }
        };

        if (mOrientationListener.canDetectOrientation()) {
            mOrientationListener.enable();
        } else {
            mOrientationListener.disable();
        }
    }

    /**
     * 处理菜单栏上的返回键
     */
    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
            case android.R.id.home:
                this.finish();
                CameraView.resetCameraId();
                return true;
            default:
                break;
        }
        return super.onOptionsItemSelected(item);
    }

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        if (keyCode == KeyEvent.KEYCODE_BACK){
            this.finish();
            CameraView.resetCameraId();
            return true;
        }
        return super.onKeyDown(keyCode, event);
    }

    /**
     * 加载菜单栏
     */
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.menu_video, menu);
        return super.onCreateOptionsMenu(menu);
    }

    /**
     * 系统是否开启自动旋转
     */
    protected boolean screenAutoRotate() {
        boolean autoRotate = false;
        try {
            autoRotate = 1 == Settings.System.getInt(getContentResolver(), Settings.System.ACCELEROMETER_ROTATION);
        } catch (Settings.SettingNotFoundException e) {
            e.printStackTrace();
        }
        return autoRotate;
    }

    /**
     * 申请权限后的回调处理
     */
    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        if (10 == requestCode) {
            if (grantResults.length > 0 && PackageManager.PERMISSION_GRANTED == grantResults[0]) {
                initViewRunnable.run();
            } else {
                Toast.makeText(this, "没有获得必要的权限", Toast.LENGTH_SHORT).show();
                finish();
            }
        }

    }

    private Runnable initViewRunnable = new Runnable() {
        @Override
        public void run() {
            doKitInfer();
        }
    };

    public Runnable createRunnable(final byte[] data, final int width, final int height) {
        Runnable aRunnable = new Runnable() {
            @Override
            public void run() {
                Bitmap bmp = null, scaleImage = null;
                final int inWidth = mPreviewWidth;
                final int inHeight = mPreviewHeight;
                if (mCameraView.isFrontCamera()) {
                    bmp = ImageUtil.yuv2bitmap(data, width, height);
                    bmp = ImageUtil.rotateBitmap(bmp, 270);
                    bmp = ImageUtil.createFlippedBitmap(bmp, true, false);
                    scaleImage = Bitmap.createScaledBitmap(bmp, inWidth, inHeight, true);
                } else {
                    bmp = ImageUtil.yuv2bitmap(data, width, height);
                    bmp = ImageUtil.rotateBitmap(bmp, 90);
                    scaleImage = Bitmap.createScaledBitmap(bmp, inWidth, inHeight, true);
                }

                inference(scaleImage);

                bmp.recycle();
                scaleImage.recycle();
            }
        };
        return aRunnable;
    }

    abstract void inference(Bitmap scaleImage);
}
