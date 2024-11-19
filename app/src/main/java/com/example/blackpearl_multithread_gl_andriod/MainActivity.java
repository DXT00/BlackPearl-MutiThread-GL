package com.example.blackpearl_multithread_gl_andriod;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.widget.TextView;

import com.example.blackpearl_multithread_gl_andriod.databinding.ActivityMainBinding;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'blackpearl_multithread_gl_andriod' library on application startup.
    static {
        System.loadLibrary("blackpearl_multithread_gl_andriod");
    }

    private ActivityMainBinding binding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        // Example of a call to a native method
        TextView tv = binding.sampleText;
        tv.setText(stringFromJNI());
    }

    /**
     * A native method that is implemented by the 'blackpearl_multithread_gl_andriod' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
}