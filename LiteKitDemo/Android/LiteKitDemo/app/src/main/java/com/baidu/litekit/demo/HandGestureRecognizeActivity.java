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
import android.graphics.Paint;
import android.graphics.PixelFormat;
import android.graphics.PorterDuff;
import android.graphics.Rect;
import android.hardware.Camera;
import android.os.Bundle;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.MenuItem;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.ViewGroup;
import android.view.ViewStub;
import android.view.Window;
import android.widget.FrameLayout;
import android.widget.RelativeLayout;
import android.widget.TextView;

import com.baidu.litekit.HandGestureDetectResult;
import com.baidu.litekit.HandGestureDetector;
import com.baidu.litekit.demo.utils.ThreadManager;
import com.baidu.litekit.demo.view.CameraView;

public class HandGestureRecognizeActivity extends CameraBaseActivity {

    private TextView mTimeCost;
    private SurfaceHolder mSurfaceHolder;

    Paint LinePaint = new Paint();
    Paint LablePaint = new Paint();

    Boolean mIsLoaded = false;
    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        LinePaint.setColor((Color.WHITE));
        LinePaint.setStyle(Paint.Style.STROKE);
        LinePaint.setStrokeWidth(6);

        LablePaint.setColor(Color.WHITE);
        LablePaint.setStrokeWidth(2f);
        LablePaint.setTextSize(40);
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
            case R.id.action_switch_camera:
                mCameraView.switchCamera();
                break;
            case R.id.action_image:
                startActivity(new Intent(this, HandGestureRecognizeImageActivity.class));
                break;
            default:
                break;
        }
        return super.onOptionsItemSelected(item);
    }

    /**
     * 手势对象的初始化, 即模型的加载
     */
    @Override
    void onKitCreate() {
        final long start = System.currentTimeMillis();
        if (!HandGestureDetector.init(this)) {
            Log.e(getResources().getString(R.string.TAG), "initialization gesture failed");
            mIsLoaded = false;
            System.exit(-1);
        }

        mIsLoaded = true;
        final long end = System.currentTimeMillis();
        Log.i(getResources().getString(R.string.TAG),
                "【LiteKit】【Gesture】【Init】"+ (end-start) +" ms");
    }

    @Override
    void doKitInfer() {
        ViewStub stub = findViewById(R.id.viewStub);
        stub.setLayoutResource(R.layout.activity_hand_gesture_detection);
        stub.inflate();
        mCameraView = findViewById(R.id.camera_view);
        mTimeCost = findViewById(R.id.costTime);
        final SurfaceView drawView = findViewById(R.id.points_view);
        drawView.setZOrderOnTop(true);
        drawView.getHolder().setFormat(PixelFormat.TRANSPARENT);
        mSurfaceHolder = drawView.getHolder();
        mCameraView.setPreviewCallback(new CameraView.PreviewCallback() {
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
                int screenH = metric.heightPixels;

                int contentTop = getWindow().findViewById(Window.ID_ANDROID_CONTENT).getTop();
                Rect frame = new Rect();
                getWindow().getDecorView().getWindowVisibleDisplayFrame(frame);
                int statusBarHeight = frame.top;

                RelativeLayout layoutVideo = findViewById(R.id.handVideoLayout);
                FrameLayout frameLayout = layoutVideo.findViewById(R.id.handVideoContentLayout);

                if (deviecAutoRotateAngle == 0 || deviecAutoRotateAngle == 180) {

                    int fixedScreenH = screenW * h / w;// 宽度不变，等比缩放的高度

                    ViewGroup.LayoutParams params = frameLayout.getLayoutParams();
                    params.height = fixedScreenH;
                    frameLayout.setLayoutParams(params);

                    mPreviewWidth = screenW;
                    mPreviewHeight = fixedScreenH;
                } else {

                    int previewHeight = screenH - contentTop - statusBarHeight;
                    int fixedScreenW = previewHeight * h / w;// 高度不变，等比缩放的宽

                    ViewGroup.LayoutParams params = frameLayout.getLayoutParams();
                    params.width = fixedScreenW;
                    frameLayout.setLayoutParams(params);

                    mPreviewWidth = fixedScreenW;
                    mPreviewHeight = previewHeight;
                }

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
        return "手势检测";
    }

    void drawLabel(HandGestureDetectResult result) {
        Canvas canvas = null;
        try {
            canvas = mSurfaceHolder.lockCanvas();
            if (canvas == null || result == null) return;

            canvas.drawColor(Color.TRANSPARENT, PorterDuff.Mode.CLEAR);
            canvas.drawRect(result.handBoxRect, LinePaint);
            canvas.drawPoint(result.fingerPoint.x, result.fingerPoint.y, LinePaint);

            canvas.drawText("confidence: " + result.confidence, 0, Math.max(0, mPreviewHeight - 70), LablePaint);
            canvas.drawText("gestureLabel: " + result.label, 0, Math.max(0, mPreviewHeight - 20), LablePaint);
        } catch (Throwable t) {
            Log.e(getResources().getString(R.string.TAG), "Draw result error:" + t);
        } finally {
            if (canvas != null) {
                mSurfaceHolder.unlockCanvasAndPost(canvas);
            }
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        synchronized (ThreadManager.mCommonlock) {
            mIsLoaded = false;
            HandGestureDetector.release();
        }
    }

    @Override
    public void inference(Bitmap scaleImage) {
        HandGestureDetectResult result = null;
        synchronized (ThreadManager.mCommonlock) {
            if (!mIsLoaded) {
                return;
            }
            final long start = System.currentTimeMillis();
            result = HandGestureDetector.detect(scaleImage);
            final long end = System.currentTimeMillis();
            Log.d(getResources().getString(R.string.TAG),
                    "【LiteKit】【Gesture】【predict】"+ (end-start) +" ms");
            mTimeCost.post(new Runnable() {
                @Override
                public void run() {
                    mTimeCost.setText((end - start) + "ms");
                }
            });
        }
        drawLabel(result);
    }

}
