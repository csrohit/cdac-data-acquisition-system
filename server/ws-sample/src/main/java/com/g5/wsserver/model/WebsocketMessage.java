package com.g5.wsserver.model;

public class WebsocketMessage {
    int deviceId;
    int peripheralId;
    int commandId;
    String data;

    public WebsocketMessage(int deviceId, int peripheralId, int commandId, String data) {
        this.deviceId = deviceId;
        this.peripheralId = peripheralId;
        this.commandId = commandId;
        this.data = data;
    }

    public WebsocketMessage() {
    }

    public int getDeviceId() {
        return deviceId;
    }

    public void setDeviceId(int deviceId) {
        this.deviceId = deviceId;
    }

    public int getPeripheralId() {
        return peripheralId;
    }

    public void setPeripheralId(int peripheralId) {
        this.peripheralId = peripheralId;
    }

    public int getCommandId() {
        return commandId;
    }

    public void setCommandId(int commandId) {
        this.commandId = commandId;
    }

    public String getData() {
        return data;
    }

    public void setData(String data) {
        this.data = data;
    }

    @Override
    public String toString() {
        return "WebsocketMessage{" +
                "deviceId=" + deviceId +
                ", peripheralId=" + peripheralId +
                ", commandId=" + commandId +
                ", data='" + data + '\'' +
                '}';
    }
}
