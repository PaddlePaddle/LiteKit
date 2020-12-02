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
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Matrix;
import android.graphics.Paint;
import android.graphics.PixelFormat;
import android.graphics.PorterDuff;
import android.hardware.Camera;
import android.os.Bundle;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.MenuItem;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.ViewGroup;
import android.view.ViewStub;
import android.widget.FrameLayout;
import android.widget.RelativeLayout;
import android.widget.TextView;

import com.baidu.litekit.demo.utils.ThreadManager;
import com.baidu.litekit.demo.view.CameraView;
import com.baidu.litekit.PortraitSegmentation;
import com.baidu.litekit.PortraitSegmentationConfig;

import java.nio.ByteBuffer;

public class PortraitSegmentationActivity extends CameraBaseActivity {
    // 执行人像分割任务的对象
    PortraitSegmentation portraitSegmentation;

    // 人像分割模型输出图片的大小
    public static final float MASK_WIDTH = 192.f;
    public static final float MASK_HEIGHT = 192.f;

    private TextView mTimeCost;

    // 负责把人像分割模型输出的结果铺在相机预览上
    private SurfaceHolder mDrawSurfaceHolder;

    // 显示在手机上的宽度和高度
    protected int mActualPreviewWidth;
    protected int mActualPreviewHeight;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        mPreviewHeight = 192;
        mPreviewWidth = 192;
        super.onCreate(savedInstanceState);
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
            case R.id.action_switch_camera:
                mCameraView.switchCamera();
                break;
            case R.id.action_image:
                startActivity(new Intent(this, PortraitSegmentationImageActivity.class));
                break;
            default:
                break;
        }
        return super.onOptionsItemSelected(item);
    }

    /**
     * 人像分割对象的初始化, 即模型的加载
     */
    @Override
    void onKitCreate() {
        PortraitSegmentationConfig config = new PortraitSegmentationConfig(this);
        final long start = System.currentTimeMillis();
        portraitSegmentation = new PortraitSegmentation(config);
        final long end = System.currentTimeMillis();
        Log.d(getResources().getString(R.string.TAG),
                "【LiteKit】【Portrait】【Init】"+ (end-start) +" ms");
    }

    /**
     * 运行, 加载人像分割的布局, 打开相机, 开启相机预览
     */
    @Override
    void doKitInfer() {
        ViewStub stub = findViewById(R.id.viewStub);
        stub.setLayoutResource(R.layout.activity_portrait_segmentation);
        stub.inflate();

        final SurfaceView drawView = findViewById(R.id.mask_view);
        drawView.setZOrderOnTop(true);
        drawView.getHolder().setFormat(PixelFormat.TRANSPARENT);
        mDrawSurfaceHolder = drawView.getHolder();
        mCameraView = findViewById(R.id.camera_view);
        mCameraView.setMinPreviewSize(600, 600);
        mTimeCost = findViewById(R.id.costTime);
        mCameraView.setPreviewCallback(new CameraView.PreviewCallback() {
            /**
             * 获取相机预览显示时的尺寸: 宽度等于手机真实的宽度, 高度等比例缩放
             */
            @Override
            public void onCameraCreated(Camera.Size previewSize, int cameraOrientation, int deviecAutoRotateAngle) {
                // w为图像短边，h为长边
                int w = previewSize.width;
                int h = previewSize.height;
                if (cameraOrientation == 90 || cameraOrientation == 270) {
                    w = previewSize.height;
                    h = previewSize.width;
                }
                // 屏幕长宽
                DisplayMetrics metric = new DisplayMetrics();
                getWindowManager().getDefaultDisplay().getMetrics(metric);
                int screenW = metric.widthPixels;
                int fixedVideoHeight = screenW * h / w;
                RelativeLayout layoutVideo = findViewById(R.id.portraitVideoLayout);
                FrameLayout frameLayout = layoutVideo.findViewById(R.id.portraitVideoContentLayout);
                ViewGroup.LayoutParams params = frameLayout.getLayoutParams();
                params.height = fixedVideoHeight;
                frameLayout.setLayoutParams(params);
                mActualPreviewWidth = metric.widthPixels;
                mActualPreviewHeight = fixedVideoHeight;
            }

            @Override
            public void onPreviewFrame(byte[] data, Camera.Size previewSize, int cameraOrientation) {
                // 优化, 减少等待(阻塞)队列的线程数
                if (ThreadManager.getPool().getQueue().size() >= 1) return;
                // 放在线程池异步执行
                ThreadManager.getPool().submit(createRunnable(data, previewSize.width, previewSize.height));
            }
        });
    }

    @Override
    String actionBarTitle() {
        return "人像分割";
    }

    /**
     * 画图, bmp是黑(人像)白(背景)图宽高: MASK_WIDTH * MASK_HEIGHT
     */
    void drawMask(Bitmap bmp) {
        Canvas canvas = null;
        try {
            canvas = mDrawSurfaceHolder.lockCanvas();
            if (canvas == null) return;

            canvas.drawColor(Color.TRANSPARENT, PorterDuff.Mode.CLEAR);
            Matrix matrix = new Matrix();
            matrix.postScale(mActualPreviewWidth / MASK_WIDTH, mActualPreviewHeight / MASK_HEIGHT);
            Paint paint = new Paint();
            paint.setColor((Color.WHITE));
            canvas.drawBitmap(bmp, matrix, paint);
        } catch (Throwable t) {
            Log.e(getResources().getString(R.string.TAG), "Draw result error:" + t);
        } finally {
            if (canvas != null) {
                mDrawSurfaceHolder.unlockCanvasAndPost(canvas);
            }
        }
    }

    /**
     * 释放资源
     */
    @Override
    protected void onDestroy() {
        super.onDestroy();
        if (portraitSegmentation != null) {
            synchronized (ThreadManager.mCommonlock) {
                portraitSegmentation.release();
                portraitSegmentation = null;
            }
        }
    }

    @Override
    public void inference(Bitmap scaleImage) {
        Bitmap rgbaImage = scaleImage.copy(scaleImage.getConfig(), true);
        byte[] inputData = new byte[rgbaImage.getWidth() * rgbaImage.getHeight() * 3];
        ByteBuffer buffer = ByteBuffer.allocate(rgbaImage.getByteCount());
        rgbaImage.copyPixelsToBuffer(buffer);
        byte[] rgba = buffer.array();
        for (int i = 0; i < rgba.length / 4; i++) {
            inputData[i * 3] = rgba[i * 4];
            inputData[i * 3 + 1] = rgba[i * 4 + 1];
            inputData[i * 3 + 2] = rgba[i * 4 + 2];
        }

        int[] alpha_data;
        synchronized (ThreadManager.mCommonlock) {
            if (portraitSegmentation == null) return;
            final long start = System.currentTimeMillis();
            alpha_data = portraitSegmentation.predictor(inputData, rgbaImage.getWidth(), rgbaImage.getHeight());
            final long end = System.currentTimeMillis();
            Log.d(getResources().getString(R.string.TAG),
                    "【LiteKit】【Portrait】【predict】"+ (end-start) +" ms");
            mTimeCost.post(new Runnable() {
                @Override
                public void run() {
                    mTimeCost.setText((end - start) + "ms");
                }
            });
        }
        Bitmap maskbmp = Bitmap.createBitmap(alpha_data, 0, (int) MASK_WIDTH, (int) MASK_WIDTH, (int) MASK_HEIGHT, Bitmap.Config.ARGB_8888);
        drawMask(maskbmp);

        rgbaImage.recycle();
        buffer.clear();
    }
}

