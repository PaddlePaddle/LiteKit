package com.baidu.mml.demo;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Point;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v7.app.ActionBar;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.Display;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;

import com.baidu.mml.demo.utils.ImageUtil;
import com.baidu.sr.SrBridge;

import java.io.File;
import java.nio.ByteBuffer;


/**
 * @class 超分demo activity
 */
public class VideoSuperResolutionActivity extends AppCompatActivity {
    static String TAG = "SR_SDK";
    static Float scale = 2.0f;
    TextView mTimeCost;

    public VideoSuperResolutionActivity() {
        System.loadLibrary("superresolution");
        Log.d(TAG, "isSupportOpenCl: " + SrBridge.isSupportOpenCL());
    }

    String actionBarTitle() {
        return "图片超分";
    }

    /**
     * 创建视图
     */
    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_video_super_resolution);
        mTimeCost = findViewById(R.id.costTime);

        ActionBar actionBar = getSupportActionBar();
        if (actionBar != null) {
            actionBar.setTitle(actionBarTitle());
            actionBar.setHomeButtonEnabled(true);
            actionBar.setDisplayHomeAsUpEnabled(true);
        }

        // 检查模型是否已经存在
        String modelPath = FileManager.sr32(this);
        if (!new File(modelPath).exists()) {
            return;
        }

        final Button btn = (Button) this.findViewById(R.id.srbutton);
        final TextView label = (TextView) this.findViewById(R.id.imageLabel);
        btn.setOnClickListener(new View.OnClickListener() {
            Boolean isSRShowing = false;
            @Override
            public void onClick(View v) {
                if (isSRShowing) {
                    btn.setText("超分");
                    label.setText("原图");
                    isSRShowing = false;
                    mTimeCost.setText("");
                    // 显示原图
                    String path = FileManager.girl2Path(VideoSuperResolutionActivity.this);
                    Bitmap lowBitmap = ImageUtil.getScaleBitmap(VideoSuperResolutionActivity.this, path, getSizeToFit(scale));
                    setBitmapToImageView(lowBitmap, R.id.imageLow, scale);
                } else {
                    btn.setText("显示原图");
                    label.setText("超分后");
                    isSRShowing = true;
                    String path = FileManager.girl2Path(VideoSuperResolutionActivity.this);
                    Bitmap lowBitmap = ImageUtil.getScaleBitmap(VideoSuperResolutionActivity.this, path, getSizeToFit(scale));

                    // 执行超分，可选方法：1. 以RGBA输入：doSrRgba(); 2. 以Bitmap输入：doSrBitmap();
                    Bitmap highBitmap = VideoSuperResolutionActivity.this.doSrBitmap(lowBitmap);

                    setBitmapToImageView(highBitmap, R.id.imageLow, 1.0f);
                }
            }
        });

        // 显示原图
        String path = FileManager.girl2Path(VideoSuperResolutionActivity.this);
        Bitmap lowBitmap = ImageUtil.getScaleBitmap(VideoSuperResolutionActivity.this, path, getSizeToFit(scale));
        setBitmapToImageView(lowBitmap, R.id.imageLow, scale);
    }

    /**
     * home按钮finish activity
     */
    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
            case android.R.id.home:
                this.finish();
                return true;
            default:
                break;
        }
        return super.onOptionsItemSelected(item);
    }

    /**
     * rgba interface sr
     */
    private Bitmap doSrRgba(Bitmap lowBitmap) {
        // 创建srmachine
        final long startInit = System.currentTimeMillis();
        Long handle = SrBridge.nativeInitSr(FileManager.sr32(this));
        final long endInit = System.currentTimeMillis();
        Log.d(getResources().getString(R.string.TAG),
                "【MML】【SR】【Init】"+ (endInit-startInit) +" ms");
        // 超分
        int byteCount = lowBitmap.getByteCount();
        ByteBuffer buf  = ByteBuffer.allocate(byteCount);
        lowBitmap.copyPixelsToBuffer(buf);
        byte[] rgba = buf.array();

        final long start = System.currentTimeMillis();

        byte[] targetRGBA = SrBridge.nativePredictRGBA(
                handle,
                rgba,
                lowBitmap.getHeight(),
                lowBitmap.getWidth(),
                scale
        );

        final long end = System.currentTimeMillis();
        Log.d(getResources().getString(R.string.TAG),
                "【MML】【SR】【predict】" + (end - start) + " ms");
        mTimeCost.setText((System.currentTimeMillis() - start) + "ms");

        for (int i=0; i< 10; i++) {
            final long start2 = System.currentTimeMillis();

            byte[] targetRGBA2 = SrBridge.nativePredictRGBA(
                    handle,
                    rgba,
                    lowBitmap.getHeight(),
                    lowBitmap.getWidth(),
                    scale
            );
            final long end2 = System.currentTimeMillis();
            Log.d(getResources().getString(R.string.TAG),
                    "【MML】【SR】【predict】" + (end2 - start2) + " ms");
            mTimeCost.setText((System.currentTimeMillis() - start2) + "ms");
        }

        // 显示超分后图像
        Bitmap bitmap =
                Bitmap.createBitmap(
                        (int) (lowBitmap.getWidth() * scale),
                        (int) (lowBitmap.getHeight() * scale),
                        Bitmap.Config.ARGB_8888
                );
        bitmap.copyPixelsFromBuffer(ByteBuffer.wrap(targetRGBA));


        // release sr machine
        SrBridge.nativeReleaseSrSdk(handle);

        return bitmap;
    }

    /**
     * bitmap interface sr
     */
    private Bitmap doSrBitmap(Bitmap lowBitmap) {
        // 创建srmachine
        final long startInit = System.currentTimeMillis();
        Long handle = SrBridge.nativeInitSr(FileManager.sr32(this));
        final long endInit = System.currentTimeMillis();
        Log.d(getResources().getString(R.string.TAG),
                "【MML】【SR】【Init】"+ (endInit-startInit) +" ms");


        final long start = System.currentTimeMillis();
        // 执行超分
        Bitmap bitmap = SrBridge.nativePredictBitmap(
                handle,
                lowBitmap,
                scale
        );
        final long end = System.currentTimeMillis();
        Log.d(getResources().getString(R.string.TAG),
                "【MML】【SR】【predict】"+ (end-start) +" ms");

        for (int i=0; i< 10; i++) {
            final long start2 = System.currentTimeMillis();

            Bitmap bitmap2 = SrBridge.nativePredictBitmap(
                    handle,
                    lowBitmap,
                    scale
            );
            final long end2 = System.currentTimeMillis();
            Log.d(getResources().getString(R.string.TAG),
                    "【MML】【SR】【predict】" + (end2 - start2) + " ms");
            mTimeCost.setText((System.currentTimeMillis() - start2) + "ms");
        }

        // release SR machine
        SrBridge.nativeReleaseSrSdk(handle);

        return bitmap;
    }

    /**
     * 根据屏幕大小和scale倍数返回合适的size
     *
     * @param scale 超分scale的倍数
     */
    private int getSizeToFit(Float scale) {
        Display display = getWindowManager().getDefaultDisplay();
        Point point = new Point();
        display.getSize(point);
        return (int) (point.y/(2+scale));
    }

    /**
     * 给imageView设置bitmap，并且设置imageView大小为bitmap大小
     *
     * @param bitmap bitmap数据
     * @param imageViewId imageView的id
     * @param scale imageView的size的scale倍数
     */
    private void setBitmapToImageView(Bitmap bitmap, int imageViewId, Float scale) {
        ImageView imageView = this.findViewById(imageViewId);
        imageView.setImageBitmap(bitmap);

        ViewGroup.LayoutParams layoutParams = imageView.getLayoutParams();
        layoutParams.width = (int) (bitmap.getWidth()*scale);
        layoutParams.height = (int) (bitmap.getHeight()*scale);
    }
}

/**
 * 文件管理工具
 */
class FileManager {

    /**
     * 获取模型存储地址
     */
    public static String modelDir() { return "models/sr"; }

    /**
     * 获取图片存储地址
     */
    public static String sourceDir() {
        return "pics";
    }

    /**
     * @desc 获取模型地址工具
     *
     * @param context Context
     *
     * @return 模型地址
     */
    public static String sr32(Context context) {
        return context.getFilesDir().getAbsolutePath() + File.separator + modelDir()+"/video-sr-int16-opencl-releasev2.6-6942d753-enc.nb";
    }

    /**
     * @desc 获取图片地址工具
     *
     * @param context Context
     *
     * @return 图片地址
     */
    public static String girl2Path(Context context) {
        return context.getFilesDir().getAbsolutePath() + File.separator + sourceDir() +"/low_demo.png";
    }
}
