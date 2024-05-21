package com.example.morse_code_generator;

import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;
import androidx.appcompat.app.AppCompatActivity;

public class MainActivity extends AppCompatActivity {

    EditText editText;
    Button dotButton, dashButton, sendButton;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        editText = findViewById(R.id.editTextText2);
        dotButton = findViewById(R.id.button);
        dashButton = findViewById(R.id.button2);
        sendButton = findViewById(R.id.button3);

        dotButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                editText.append(".");
            }
        });

        dashButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                editText.append("_");
            }
        });

        sendButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String message = editText.getText().toString().trim();
                if (!message.isEmpty()) {
                    sendMessage(message);
                } else {
                    Toast.makeText(MainActivity.this, "Please enter a message", Toast.LENGTH_SHORT).show();
                }
            }
        });
    }

    private void sendMessage(String message) {
        // Add your code here to send the message
        // For example, you can send the message via Bluetooth or any other communication method

        // Clear the EditText after sending the message
        editText.setText("");

        // Display a success message or perform any other action
        Toast.makeText(this, "Message sent: " + message, Toast.LENGTH_SHORT).show();
    }
}
