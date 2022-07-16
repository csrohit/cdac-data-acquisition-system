package com.g5.wsserver.model;

public class MqttUsbMessage {
    private byte serial;
    private byte peripheralId;
    private byte command;
    private String data;

    public MqttUsbMessage(byte serial, byte peripheralId, byte command, String data) {
        this.serial = serial;
        this.peripheralId = peripheralId;
        this.command = command;
        this.data = data;
    }

    public MqttUsbMessage() {
    }

    public byte getSerial() {
        return serial;
    }

    public void setSerial(byte serial) {
        this.serial = serial;
    }

    public byte getPeripheralId() {
        return peripheralId;
    }

    public void setPeripheralId(byte peripheralId) {
        this.peripheralId = peripheralId;
    }

    public byte getCommand() {
        return command;
    }

    public void setCommand(byte command) {
        this.command = command;
    }

    public String getData() {
        return data;
    }

    public void setData(String data) {
        this.data = data;
    }

    @Override
    public String toString() {
        return "MqttUsbMessage{" +
                "serial=" + serial +
                ", peripheralId=" + peripheralId +
                ", command=" + command +
                ", data='" + data + '\'' +
                '}';
    }

    public byte[] getBytes(){
        int len = data.length();
        byte dataBytes[] = data.getBytes();
        byte[] arr = new byte[3 + len];
                arr[0] = serial;
                arr[1] = peripheralId;
                arr[2] = command;
                for (int i=0; i<len; i++){
                    arr[3+i] = dataBytes[i];
                }
        return arr;
    }
}
