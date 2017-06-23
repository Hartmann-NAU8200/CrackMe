package com.example.shush.crackme;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.EditText;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    EditText editText;
    String sn = "";
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        editText = (EditText) findViewById(R.id.etSN);
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String resultFromJNI(String sn);
    public void check(View view) {
        sn = editText.getText().toString();
        String result =  resultFromJNI(sn);
        Toast.makeText(this, result, Toast.LENGTH_SHORT).show();
    }
}
