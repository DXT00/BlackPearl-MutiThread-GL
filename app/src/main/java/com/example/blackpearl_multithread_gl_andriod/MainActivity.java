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
import static com.example.blackpearl_multithread_gl_andriod.MySurfaceView.IMAGE_FORMAT_GARY;
import static com.example.blackpearl_multithread_gl_andriod.MySurfaceView.IMAGE_FORMAT_I420;
import static com.example.blackpearl_multithread_gl_andriod.MySurfaceView.IMAGE_FORMAT_I444;
import static com.example.blackpearl_multithread_gl_andriod.MySurfaceView.IMAGE_FORMAT_NV21;
import static com.example.blackpearl_multithread_gl_andriod.MySurfaceView.IMAGE_FORMAT_RGBA;
import static com.example.blackpearl_multithread_gl_andriod.MySurfaceView.IMAGE_FORMAT_YUYV;
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

                    RelativeLayout.LayoutParams lp = new RelativeLayout.LayoutParams(
                            ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT);
                    lp.addRule(RelativeLayout.CENTER_IN_PARENT);

                    if (mRootView.getWidth() != mGLSurfaceView.getWidth()
                            || mRootView.getHeight() != mGLSurfaceView.getHeight()) {
                        mGLSurfaceView.setAspectRatio(mRootView.getWidth(), mRootView.getHeight());
                    }
                    mGLRender.beginDraw = true;
                    loadRGBAImage(R.drawable.lye, mGLRender);
                    mGLSurfaceView.requestRender();
                    mGLSurfaceView.setRenderMode(RENDERMODE_CONTINUOUSLY);
                    mBtn.setText("后台渲染");

            }
        });

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
               // mGLSurfaceView.setAspectRatio(bitmap.getWidth(), bitmap.getHeight());

                render.setImageData(IMAGE_FORMAT_RGBA, bitmap.getWidth(), bitmap.getHeight(),byteArray);
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
        mGLSurfaceView.setRenderMode(RENDERMODE_WHEN_DIRTY);
        mRootView.addView(mGLSurfaceView, lp);

    }
    /**
     * A native method that is implemented by the 'blackpearl_multithread_gl_andriod' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
}