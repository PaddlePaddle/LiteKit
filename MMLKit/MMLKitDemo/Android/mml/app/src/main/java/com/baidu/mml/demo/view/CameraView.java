package com.baidu.mml.demo.view;

import android.app.Activity;
import android.content.Context;
import android.graphics.ImageFormat;
import android.hardware.Camera;
import android.util.AttributeSet;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

import com.baidu.mml.demo.R;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;

public class CameraView extends SurfaceView implements SurfaceHolder.Callback, Camera.PreviewCallback {

    private static final int MIN_PREVIEW_SIZE = 320;

    private static Camera mCamera;
    private static int mCameraFacing = Camera.CameraInfo.CAMERA_FACING_FRONT;

    private Camera.Parameters mParams;
    private Camera.Size mPreviewSize;
    private PreviewCallback mPreviewCallback;
    private int mOrientation;

    private int minPreviewWidth;
    private int minPreviewHeight;

    public static void resetCameraId() {
        mCameraFacing = Camera.CameraInfo.CAMERA_FACING_FRONT;
        releaseCamera();
    }

    public CameraView(Context context) {
        this(context, null);
    }

    public CameraView(Context context, AttributeSet attrs) {
        super(context, attrs);
        minPreviewWidth = 800;
        minPreviewHeight = 800;
        SurfaceHolder holder = getHolder();
        holder.addCallback(this);
    }

    public void setPreviewCallback(CameraView.PreviewCallback previewCallback) {
        mPreviewCallback = previewCallback;
    }

    public void setMinPreviewSize(int minWidth, int minHeight) {
        minPreviewWidth = minWidth;
        minPreviewHeight = minHeight;
    }

    private void openCamera(SurfaceHolder holder) {
        releaseCamera();
        mCamera = Camera.open(mCameraFacing);

        Camera.CameraInfo info = new Camera.CameraInfo();
        Camera.getCameraInfo(mCameraFacing, info);
        int rotation = ((Activity) getContext()).getWindowManager().getDefaultDisplay().getRotation();
        int degrees = rotation * 90;

        mOrientation = info.orientation;

        int mirror = (info.facing == Camera.CameraInfo.CAMERA_FACING_FRONT)? -1 : 1;
        int displayOrientation = (360 + mirror * (info.orientation - degrees) ) % 360;
        mCamera.setDisplayOrientation(displayOrientation);

        mParams = mCamera.getParameters();
        mPreviewSize = getPropPreviewSize(mParams.getSupportedPreviewSizes(), minPreviewWidth, minPreviewHeight);
        mParams.setPreviewSize(mPreviewSize.width, mPreviewSize.height);
        mParams.setPreviewFormat(ImageFormat.NV21);
        if (mCameraFacing == Camera.CameraInfo.CAMERA_FACING_BACK) {
            mParams.setFocusMode(Camera.Parameters.FOCUS_MODE_CONTINUOUS_VIDEO);
        }

        mCamera.setParameters(mParams);

        if (mPreviewCallback != null) {
            mPreviewCallback.onCameraCreated(mPreviewSize, mOrientation, degrees);
        }

        try {
            mCamera.setPreviewDisplay(holder);
        } catch (IOException e) {
            e.printStackTrace();
        }
        mCamera.setPreviewCallback(this);
        mCamera.startPreview();
    }

    private static synchronized void releaseCamera() {
        if (mCamera != null) {
            try {
                mCamera.setPreviewCallback(null);
            } catch (Exception e) {
                e.printStackTrace();
            }
            try {
                mCamera.stopPreview();
            } catch (Exception e) {
                e.printStackTrace();
            }
            try {
                mCamera.release();
            } catch (Exception e) {
                e.printStackTrace();
            }
            mCamera = null;
        }
    }

    @Override
    public void surfaceCreated(SurfaceHolder surfaceHolder) {
        Log.i(getResources().getString(R.string.TAG), "surfaceCreated");
        openCamera(surfaceHolder);
    }

    @Override
    public void surfaceChanged(SurfaceHolder surfaceHolder, int i, int i1, int i2) {
        Log.i(getResources().getString(R.string.TAG), "surfaceChanged");
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder surfaceHolder) {
        Log.i(getResources().getString(R.string.TAG), "surfaceDestroyed");
    }

    public void switchCamera() {
        mCameraFacing = isFrontCamera() ? Camera.CameraInfo.CAMERA_FACING_BACK : Camera.CameraInfo.CAMERA_FACING_FRONT;
        openCamera(getHolder());
    }

    public boolean isFrontCamera() {
        return Camera.CameraInfo.CAMERA_FACING_FRONT == mCameraFacing;
    }


    /**
     * 相机回调函数, bytes保存的是yuv NV21的数据
     */
    @Override
    public void onPreviewFrame(byte[] bytes, Camera camera) {
        if (mPreviewCallback != null) {
            mPreviewCallback.onPreviewFrame(bytes, mPreviewSize, mOrientation);
        }
    }

    private Camera.Size getPropPreviewSize(List<Camera.Size> choices, int minWidth, int minHeight) {
        final int minSize = Math.max(Math.min(minWidth, minHeight), MIN_PREVIEW_SIZE);

        final List<Camera.Size> big = new ArrayList<Camera.Size>();
        final List<Camera.Size> small = new ArrayList<Camera.Size>();

        for (Camera.Size size : choices) {
            if (size.width == minWidth && size.height == minHeight) {
                return size;
            }

            if (size.height >= minSize && size.width >= minSize) {
                big.add(size);
            } else {
                small.add(size);
            }
        }

        if (big.size() > 0) {
            Camera.Size chosenSize = Collections.min(big, new CompareSizesByArea());
            return chosenSize;
        } else {
            return choices.get(0);
        }
    }


    public interface PreviewCallback {
        // camera created
        void onCameraCreated(Camera.Size previewSize, int cameraOrientation, int deviecAutoRotateAngle);

        // preview callback
        void onPreviewFrame(byte[] data, Camera.Size previewSize, int cameraOrientation);
    }

    static class CompareSizesByArea implements Comparator<Camera.Size> {
        @Override
        public int compare(final Camera.Size lhs, final Camera.Size rhs) {
            return Long.signum(
                    (long) lhs.width * lhs.height - (long) rhs.width * rhs.height);
        }
    }

}