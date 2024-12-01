package com.example.blackpearl_multithread_gl_andriod;

import static android.opengl.GLSurfaceView.RENDERMODE_WHEN_DIRTY;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.ViewGroup;
import android.view.ViewTreeObserver;
import android.widget.RelativeLayout;
import android.widget.TextView;
import  com.example.blackpearl_multithread_gl_andriod.MySurfaceView;
import com.example.blackpearl_multithread_gl_andriod.databinding.ActivityMainBinding;
import static android.opengl.GLSurfaceView.RENDERMODE_CONTINUOUSLY;
import static android.opengl.GLSurfaceView.RENDERMODE_WHEN_DIRTY;
import javax.microedition.khronos.opengles.GL10;
import android.view.ViewTreeObserver;
import static com.example.blackpearl_multithread_gl_andriod.MyNativeRender.SAMPLE_TYPE;
public class MainActivity extends AppCompatActivity implements ViewTreeObserver.OnGlobalLayoutListener{

    // Used to load the 'blackpearl_multithread_gl_andriod' library on application startup.
    static {
        System.loadLibrary("blackpearl_multithread_gl_andriod");
    }

    private ActivityMainBinding binding;
    private MyGLRender mGLRender = new MyGLRender();

    private MySurfaceView mGLSurfaceView;
    private ViewGroup mRootView;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        // Example of a call to a native method
        TextView tv = binding.sampleText;
        tv.setText(stringFromJNI());

        setContentView(R.layout.activity_main);
        mRootView = (ViewGroup) findViewById(R.id.rootView);
        mRootView.getViewTreeObserver().addOnGlobalLayoutListener(this);

//        mRootView.removeView(mGLSurfaceView);
        RelativeLayout.LayoutParams lp = new RelativeLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT);
        lp.addRule(RelativeLayout.CENTER_IN_PARENT);

        mGLRender.init();
        mGLSurfaceView = new MySurfaceView(MainActivity.this, mGLRender);
        mRootView.addView(mGLSurfaceView, lp);

        mGLSurfaceView.setRenderMode(RENDERMODE_WHEN_DIRTY);
        //mGLRender.setParamsInt(SAMPLE_TYPE, position + SAMPLE_TYPE, 0);

        if (mRootView.getWidth() != mGLSurfaceView.getWidth()
                || mRootView.getHeight() != mGLSurfaceView.getHeight()) {
            mGLSurfaceView.setAspectRatio(mRootView.getWidth(), mRootView.getHeight());
        }
        mGLSurfaceView.requestRender();


//        dialog.show();
//        dialog.getWindow().setContentView(rootView);
    }

    @Override
    public void onGlobalLayout() {

        mRootView.getViewTreeObserver().removeOnGlobalLayoutListener(this);
        RelativeLayout.LayoutParams lp = new RelativeLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT);
        lp.addRule(RelativeLayout.CENTER_IN_PARENT);
        mGLSurfaceView = new MySurfaceView(this, mGLRender);
        mRootView.addView(mGLSurfaceView, lp);
        mGLSurfaceView.setRenderMode(RENDERMODE_CONTINUOUSLY);

    }
    /**
     * A native method that is implemented by the 'blackpearl_multithread_gl_andriod' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
}