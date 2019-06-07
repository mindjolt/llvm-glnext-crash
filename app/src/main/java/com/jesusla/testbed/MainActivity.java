package com.jesusla.testbed;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.ToggleButton;

public class MainActivity extends AppCompatActivity {
    public static final String EXTRA_PROTEGO = "com.jesusla.testbed.protego";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }

    public void launch(View view) {
        Intent intent = new Intent(this, OpenGLActivity.class);
        ToggleButton toggle = findViewById(R.id.toggleButton);
        boolean protego = toggle.isChecked();
        intent.putExtra(EXTRA_PROTEGO, protego);
        startActivity(intent);
    }
}
