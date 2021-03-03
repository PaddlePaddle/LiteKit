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

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.PixelFormat;
import android.graphics.PorterDuff;
import android.graphics.RectF;
import android.os.Bundle;
import android.support.v7.app.ActionBar;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.widget.ImageView;

import com.baidu.litekit.HandGestureDetectResult;
import com.baidu.litekit.HandGestureDetector;
import com.baidu.litekit.demo.utils.ThreadManager;

public class HandGestureRecognizeImageActivity extends AppCompatActivity {

    Paint LinePaint = new Paint();
    Paint TextPaint = new Paint();
    private SurfaceHolder mDrawSurfaceHolder;
    private ImageView imageView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_hand_gesture_detection_image);

        ActionBar actionBar = getSupportActionBar();
        if (actionBar != null) {
            actionBar.setTitle("手势检测-图片");
            actionBar.setHomeButtonEnabled(true);
            actionBar.setDisplayHomeAsUpEnabled(true);
        }

        LinePaint.setColor((Color.RED));
        LinePaint.setStyle(Paint.Style.STROKE);
        LinePaint.setStrokeWidth(6);

        TextPaint.setColor(Color.DKGRAY);
        TextPaint.setStrokeWidth(2f);
        TextPaint.setTextSize(58);

        SurfaceView drawView = findViewById(R.id.points_view);
        drawView.setZOrderOnTop(true);
        drawView.getHolder().setFormat(PixelFormat.TRANSPARENT);
        mDrawSurfaceHolder = drawView.getHolder();

        imageView = findViewById(R.id.imageView);
        imageView.setImageResource(R.mipmap.face_img);
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
            case android.R.id.home:
                this.finish();
                return true;
            case R.id.action_start:
                doDetect();
                return true;
            default:
                break;
        }
        return super.onOptionsItemSelected(item);
    }

    void doDetect() {
        synchronized (ThreadManager.mCommonlock) {
            Bitmap bmp = BitmapFactory.decodeResource(HandGestureRecognizeImageActivity.this.getResources(), R.mipmap.face_img);
            long start = System.currentTimeMillis();
            HandGestureDetectResult result = HandGestureDetector.detect(bmp);
            drawLabel(result, bmp, System.currentTimeMillis() - start);
            final long end = System.currentTimeMillis();
            Log.d(getResources().getString(R.string.TAG),
                    "【LiteKit】【Gesture】【predict】" + (end - start) + " ms");
        }
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.menu_hand_image, menu);
        return super.onCreateOptionsMenu(menu);
    }

    void drawLabel(HandGestureDetectResult result, Bitmap bitmap, long timeCost) {
        Canvas canvas = null;
        try {
            canvas = mDrawSurfaceHolder.lockCanvas();
            if (canvas == null) return;
            canvas.drawColor(Color.TRANSPARENT, PorterDuff.Mode.CLEAR);
            float w_ratio = imageView.getWidth() * 1.f / bitmap.getWidth();
            float h_ratio = imageView.getHeight() * 1.f / bitmap.getHeight();
            RectF rect = result.handBoxRect;
            rect.left = rect.left * w_ratio;
            rect.right = rect.right * w_ratio;
            rect.top = rect.top * h_ratio;
            rect.bottom = rect.bottom * h_ratio;

            canvas.drawRect(rect, LinePaint);
            canvas.drawText("confidence: " + result.confidence, 10, 50, TextPaint);
            canvas.drawText("gestureLabel: " + result.label, 10, 150, TextPaint);
            canvas.drawText("timecost: " + timeCost + "ms", 10, imageView.getHeight() - 100, TextPaint);

        } catch (Throwable t) {
            Log.e(getResources().getString(R.string.TAG), "Draw result error:" + t);
        } finally {
            if (canvas != null) {
                mDrawSurfaceHolder.unlockCanvasAndPost(canvas);
            }
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
    }
}
