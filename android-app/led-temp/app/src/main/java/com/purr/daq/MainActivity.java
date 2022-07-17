package com.purr.daq;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Context;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.Switch;
import android.widget.Toast;

import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.purr.daq.socket.MySocketListener;

import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.WebSocket;
import okhttp3.WebSocketListener;
import ua.naiksoftware.stomp.Stomp;
import ua.naiksoftware.stomp.StompClient;

public class MainActivity extends AppCompatActivity {
    private static final String TAG = "MainActivity";
    private Switch switchBlueLed = null, switchYellowLed = null;
    private StompClient mStompClient;
    private ObjectMapper mapper = null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mapper = new ObjectMapper();
        Button buttonConnect = findViewById(R.id.buttonConnect);
        EditText editTextIPAddress = findViewById(R.id.editTextTextIPAddress);
        EditText editTextPort = findViewById(R.id.editTextNumber);
        switchBlueLed = findViewById(R.id.switch_led_blue);
        switchYellowLed = findViewById(R.id.switch_led_yellow);
        buttonConnect.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Log.i(TAG, "Connect button clicked");
                String ipAddress = editTextIPAddress.getText().toString();
                String portString = editTextPort.getText().toString();
                if(portString.length() < 4){

                    Toast.makeText(getApplicationContext(), "Enter port value", Toast.LENGTH_SHORT).show();
                    return;
                }
                int port = Integer.parseInt(portString);
                Toast.makeText(getApplicationContext(), "Connecting to " + ipAddress + ":" + port, Toast.LENGTH_SHORT).show();

                mStompClient = Stomp.over(Stomp.ConnectionProvider.OKHTTP, "ws://"+ipAddress+":"+port+"/ws/websocket");

                mStompClient.lifecycle().subscribe(lifecycleEvent -> {
                    switch (lifecycleEvent.getType()) {
                        case OPENED:
                            Log.d(TAG, "Stomp connection opened");
                            break;
                        case CLOSED:
                            Log.d(TAG, "Stomp connection closed");
                            break;
                        case ERROR:
                            Log.e(TAG, "Stomp connection error", lifecycleEvent.getException());
                            break;
                    }
                });
                mStompClient.connect();
                Log.i(TAG, "Sending first message");
                mStompClient.topic("/app/news")
                        .doOnError(t-> Log.e(TAG, "Subescribe failed", t)).subscribe(topicMessage -> {
                            Message message = mapper.readValue(topicMessage.getPayload(), Message.class);
//                            Toast.makeText(getApplicationContext(), message.getFrom() + ": " + message.getText(), Toast.LENGTH_SHORT).show();

                            Log.i(TAG, message.getFrom() + ": " + message.getText());
                });

//                mStompClient.send("/topic/news", "My first STOMP message!").subscribe();


//                mStompClient.disconnect();

            }
        });
        switchBlueLed.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean isChecked) {
                Message m = new Message("android", "Led is on");
                if(isChecked){
                    m.setText("Led is on");
                    Toast.makeText(getApplicationContext(), "Turning on Blue Led", Toast.LENGTH_SHORT).show();
                }else{
                    m.setText("Led is off");
                    Toast.makeText(getApplicationContext(), "Turning off Blue Led", Toast.LENGTH_SHORT).show();
                }

                try {
                    mStompClient.send("/app/news", mapper.writeValueAsString(m))
                            .doOnError(t-> Log.e(TAG, "Connection failed", t)).subscribe();
                } catch (JsonProcessingException e) {
                        e.printStackTrace();
                }
            }
        });

        switchYellowLed.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean isChecked) {
                if(isChecked){
                    Toast.makeText(getApplicationContext(), "Turning on Yellow Led", Toast.LENGTH_SHORT).show();
                }else{
                    Toast.makeText(getApplicationContext(), "Turning off Yellow Led", Toast.LENGTH_SHORT).show();
                }
            }
        });
    }

    @Override
    protected void onDestroy() {
        Log.i(TAG, "Disconnecting from stomp");
        if(mStompClient != null && mStompClient.isConnected()){

            mStompClient.disconnect();
        }
        super.onDestroy();
    }
}