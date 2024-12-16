package com.example.blackpearl_multithread_gl_andriod;

import static android.opengl.GLSurfaceView.RENDERMODE_WHEN_DIRTY;

import androidx.appcompat.app.AppCompatActivity;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.opengl.GLES20;
import android.opengl.GLException;
import android.os.Bundle;
import android.view.View;
import android.view.ViewGroup;
import android.view.ViewTreeObserver;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.RelativeLayout;
import android.widget.TextView;
import  com.example.blackpearl_multithread_gl_andriod.MySurfaceView;
import com.example.blackpearl_multithread_gl_andriod.databinding.ActivityMainBinding;
import static android.opengl.GLSurfaceView.RENDERMODE_CONTINUOUSLY;
import static android.opengl.GLSurfaceView.RENDERMODE_WHEN_DIRTY;
import javax.microedition.khronos.opengles.GL10;
import android.view.ViewTreeObserver;
import static com.example.blackpearl_multithread_gl_andriod.MyNativeRender.SAMPLE_TYPE;

import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteBuffer;
import java.nio.IntBuffer;

public class MainActivity extends AppCompatActivity implements ViewTreeObserver.OnGlobalLayoutListener{

    // Used to load the 'blackpearl_multithread_gl_andriod' library on application startup.
    static {
        System.loadLibrary("blackpearl_multithread_gl_andriod");
    }

    private ActivityMainBinding binding;
    private MyGLRender mGLRender = new MyGLRender();

    private MySurfaceView mGLSurfaceView;
    private ViewGroup mRootView;

    private ImageView mImageView;
    private Button mBtn;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        //binding = ActivityMainBinding.inflate(getLayoutInflater());
        //setContentView(binding.getRoot());

        // Example of a call to a native method
        //TextView tv = binding.sampleText;
        //tv.setText(stringFromJNI());

        //setContentView(R.layout.activity_main);
        mRootView = (ViewGroup) findViewById(R.id.rootView);
        mRootView.getViewTreeObserver().addOnGlobalLayoutListener(this);
        mImageView = (ImageView) findViewById(R.id.imageView);
        mBtn = (Button) findViewById(R.id.button);

        mBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                //if (mBtn.getText().equals("重置")) {
//                    mImageView.setImageResource(R.drawable.ic_launcher_background);
                    //mGLRender.init();


//        mRootView.removeView(mGLSurfaceView);
                    RelativeLayout.LayoutParams lp = new RelativeLayout.LayoutParams(
                            ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT);
                    lp.addRule(RelativeLayout.CENTER_IN_PARENT);


//                    mGLSurfaceView = new MySurfaceView(MainActivity.this, mGLRender);
//                    mRootView.addView(mGLSurfaceView, lp);
//
                    mGLSurfaceView.setRenderMode(RENDERMODE_CONTINUOUSLY);
                    //mGLRender.setParamsInt(SAMPLE_TYPE, position + SAMPLE_TYPE, 0);

                    if (mRootView.getWidth() != mGLSurfaceView.getWidth()
                            || mRootView.getHeight() != mGLSurfaceView.getHeight()) {
                        mGLSurfaceView.setAspectRatio(mRootView.getWidth(), mRootView.getHeight());
                    }
                    //loadRGBAImage(R.drawable.ic_launcher_foreground, mGLRender);
                    mGLSurfaceView.requestRender();
                   // mImageView.setImageBitmap(createBitmapFromGLSurface(0, 0, 933, 1400));

                    mBtn.setText("后台渲染");
//                } else {
//                    //startBgRender();
//                    mBtn.setText("重置");
//                }
            }
        });

//        dialog.show();
//        dialog.getWindow().setContentView(rootView);
    }
    private Bitmap createBitmapFromGLSurface(int x, int y, int w, int h) {
        int bitmapBuffer[] = new int[w * h];
        int bitmapSource[] = new int[w * h];
        IntBuffer intBuffer = IntBuffer.wrap(bitmapBuffer);
        intBuffer.position(0);
        try {
            GLES20.glReadPixels(x, y, w, h, GLES20.GL_RGBA, GLES20.GL_UNSIGNED_BYTE,
                    intBuffer);
            int offset1, offset2;
            for (int i = 0; i < h; i++) {
                offset1 = i * w;
                offset2 = (h - i - 1) * w;
                for (int j = 0; j < w; j++) {
                    int texturePixel = bitmapBuffer[offset1 + j];
                    int blue = (texturePixel >> 16) & 0xff;
                    int red = (texturePixel << 16) & 0x00ff0000;
                    int pixel = (texturePixel & 0xff00ff00) | red | blue;
                    bitmapSource[offset2 + j] = pixel;
                }
            }
        } catch (GLException e) {
            return null;
        }
        return Bitmap.createBitmap(bitmapSource, w, h, Bitmap.Config.ARGB_8888);
    }

    private void loadRGBAImage(int resId, MyGLRender render) {
        InputStream is = this.getResources().openRawResource(resId);
        Bitmap bitmap;
        try {
            bitmap = BitmapFactory.decodeStream(is);
            if (bitmap != null) {
                int bytes = bitmap.getByteCount();
                ByteBuffer buf = ByteBuffer.allocate(bytes);
                bitmap.copyPixelsToBuffer(buf);
                byte[] byteArray = buf.array();
                render.setImageData(byteArray, bitmap.getWidth(), bitmap.getHeight());
            }
        }
        finally
        {
            try
            {
                is.close();
            }
            catch(IOException e)
            {
                e.printStackTrace();
            }
        }
    }

    @Override
    public void onGlobalLayout() {

        mRootView.getViewTreeObserver().removeOnGlobalLayoutListener(this);
        RelativeLayout.LayoutParams lp = new RelativeLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT);
        lp.addRule(RelativeLayout.CENTER_IN_PARENT);
        mGLSurfaceView = new MySurfaceView(this, mGLRender);
        mRootView.addView(mGLSurfaceView, lp);
       // mGLSurfaceView.setRenderMode(RENDERMODE_CONTINUOUSLY);

    }
    /**
     * A native method that is implemented by the 'blackpearl_multithread_gl_andriod' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
}