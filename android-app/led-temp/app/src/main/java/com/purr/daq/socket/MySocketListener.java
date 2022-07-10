package com.purr.daq.socket;

import android.util.Log;

import androidx.annotation.NonNull;

import okhttp3.Response;
import okhttp3.WebSocket;
import okio.ByteString;

public class MySocketListener extends okhttp3.WebSocketListener {
    private static final String TAG = "WebSocketListener";
    private static final int CLOSE_STATUS = 1000;

    public MySocketListener() {
        super();
    }

    @Override
    public void onOpen(@NonNull WebSocket webSocket, @NonNull Response response) {
        Log.i(TAG, "websocket opened");
        webSocket.send("What's up ?");
        webSocket.send(ByteString.decodeHex("abcd"));
        webSocket.close(CLOSE_STATUS, "Socket Closed !!");
        super.onOpen(webSocket, response);
    }

    @Override
    public void onMessage(@NonNull WebSocket webSocket, @NonNull String text) {
        Log.i(TAG, "Message: " + text);
        super.onMessage(webSocket, text);
    }

    @Override
    public void onMessage(@NonNull WebSocket webSocket, @NonNull ByteString bytes) {
        Log.i(TAG, "Message: " + bytes.toString());
        super.onMessage(webSocket, bytes);
    }

    @Override
    public void onClosing(@NonNull WebSocket webSocket, int code, @NonNull String reason) {
        Log.i(TAG, "websocket closing");
        super.onClosing(webSocket, code, reason);
    }

    @Override
    public void onClosed(@NonNull WebSocket webSocket, int code, @NonNull String reason) {
        Log.i(TAG, "websocket closed");
        super.onClosed(webSocket, code, reason);
    }

    @Override
    public void onFailure(@NonNull WebSocket webSocket, @NonNull Throwable t, Response response) {

        Log.e(TAG, "Failed", t);
        super.onFailure(webSocket, t, response);
    }
}
