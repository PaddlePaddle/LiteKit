package com.baidu.mml.demo;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Matrix;
import android.graphics.Paint;
import android.graphics.PixelFormat;
import android.graphics.PorterDuff;
import android.os.Bundle;
import android.support.v7.app.ActionBar;
import android.support.v7.app.AppCompatActivity;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.widget.ImageView;

import com.baidu.mml.demo.utils.FileUtil;
import com.baidu.mmlkit.portraitsegmentationsdk.MMLPortraitSegmentation;
import com.baidu.mmlkit.portraitsegmentationsdk.MMLPortraitSegmentationConfig;

import java.io.File;
import java.io.IOException;
import java.nio.ByteBuffer;

public class PortraitSegmentationImageActivity extends AppCompatActivity {
    // 执行人像分割任务的对象
    MMLPortraitSegmentation portraitSegmentation;

    public static final float SEGMENT_WIDTH = 192.f;
    public static final float SEGMENT_HEIGHT = 192.f;

    private Paint TextPaint = new Paint();

    private SurfaceHolder mSurfaceHolder;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_portrait_segmentation_image);

        ActionBar actionBar = getSupportActionBar();
        if (actionBar != null) {
            actionBar.setTitle("人像分割-图片");
            actionBar.setHomeButtonEnabled(true);
            actionBar.setDisplayHomeAsUpEnabled(true);
        }

        TextPaint.setColor(Color.DKGRAY);
        TextPaint.setStrokeWidth(2f);
        TextPaint.setTextSize(58);

        SurfaceView drawView = findViewById(R.id.points_view);
        drawView.setZOrderOnTop(true);
        drawView.getHolder().setFormat(PixelFormat.TRANSPARENT);
        mSurfaceHolder = drawView.getHolder();

        ImageView imageView = findViewById(R.id.imageView);
        imageView.setImageResource(R.mipmap.portrait_girl);
    }

    /**
     * 画图, bmp是黑(人像)白(背景)图宽高: MASK_WIDTH * MASK_HEIGHT
     */
    void drawMask(Bitmap orgBmp, Bitmap maskBmp, long timeCost) {
        Canvas canvas = null;
        try {
            canvas = mSurfaceHolder.lockCanvas();
            if (canvas == null) return ;

            canvas.drawColor(Color.TRANSPARENT, PorterDuff.Mode.CLEAR);
            // 屏幕长宽
            DisplayMetrics metric = new DisplayMetrics();
            getWindowManager().getDefaultDisplay().getMetrics(metric);
            int screenW = metric.widthPixels;
            int imgW = orgBmp.getWidth();
            int imgH = orgBmp.getHeight();
            int previewWidth = screenW;
            int previewHeight = (int) (screenW * imgH / (float) imgW);
            Matrix matrix = new Matrix();
            matrix.postScale(previewWidth / SEGMENT_WIDTH, previewHeight / SEGMENT_HEIGHT);
            Paint paint = new Paint();
            paint.setColor((Color.WHITE));
            canvas.drawBitmap(maskBmp, matrix, paint);
            canvas.drawText(timeCost + " ms", 20, previewHeight + 70, TextPaint);

        } catch (Throwable t) {
            Log.e(getResources().getString(R.string.TAG), "Draw result error:" + t);
        } finally {
            if (canvas != null) {
                mSurfaceHolder.unlockCanvasAndPost(canvas);
            }
        }
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
            case android.R.id.home:
                this.finish();
                return true;
            case R.id.action_start:
                doSegment();
                return true;
            default:
                break;
        }
        return super.onOptionsItemSelected(item);
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.menu_portrait_image, menu);
        return super.onCreateOptionsMenu(menu);
    }

    private void createKitInstance() {
        String dir = this.getFilesDir().getAbsolutePath() + File.separator;
        String model_name = "/humanseg_shv75_0303-fp32-cpu-releasev2.6-fix.9e12fd68-enc.nb";
        try {
            FileUtil.copyFileFromAssets(this.getApplicationContext(), "models/portrait" + model_name, dir + model_name);
        } catch (IOException e) {
            Log.e(getResources().getString(R.string.TAG), e.toString());
        }
        MMLPortraitSegmentationConfig config = new MMLPortraitSegmentationConfig();
        config.setModelUrl(dir + model_name);
        final long start = System.currentTimeMillis();
        portraitSegmentation = new MMLPortraitSegmentation(config);
        final long end = System.currentTimeMillis();
        Log.d(getResources().getString(R.string.TAG),
                "【MML】【Portrait】【Init】"+ (end-start) +" ms");
    }

    void doSegment() {
        createKitInstance();
        Bitmap bmp = null, scaleImage = null;
        bmp = BitmapFactory.decodeResource(PortraitSegmentationImageActivity.this.getResources(), R.mipmap.portrait_girl);
        final int inWidth = 192;
        final int inHeight = 192;

        scaleImage = Bitmap.createScaledBitmap(bmp, inWidth, inHeight, true);

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
        final long start = System.currentTimeMillis();
        int[] alpha_data = portraitSegmentation.predictor(inputData, rgbaImage.getWidth(), rgbaImage.getHeight());
        final long end = System.currentTimeMillis();
        Log.d(getResources().getString(R.string.TAG),
                "【MML】【Portrait】【predict】"+ (end-start) +" ms");
        Bitmap maskbmp = Bitmap.createBitmap(alpha_data, 0, (int) SEGMENT_WIDTH, (int) SEGMENT_WIDTH, (int) SEGMENT_HEIGHT, Bitmap.Config.ARGB_8888);
        drawMask(bmp, maskbmp, System.currentTimeMillis() - start);

        bmp.recycle();
        scaleImage.recycle();
        rgbaImage.recycle();
        buffer.clear();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        if (portraitSegmentation != null) {
            portraitSegmentation.release();
            portraitSegmentation = null;
        }
    }
}
