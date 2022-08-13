package com.g5.wsserver.model;


import com.fasterxml.jackson.annotation.JsonInclude;

@JsonInclude(JsonInclude.Include.NON_NULL)
public class MqttMessage {
    private byte peripheralId;
    private byte command;
    private String data;


    public MqttMessage() {
        data = "";
    }

    public MqttMessage(byte peripheralId, byte command, String data) {
        this.peripheralId = peripheralId;
        this.command = command;
        this.data = data;
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

    public byte[] getBytes(){
        if(data == null){
            data = "";
        }
        int len = data.length();
        byte dataBytes[] = data.getBytes();
        byte[] arr = new byte[2 + len];
                arr[0] = peripheralId;
                arr[1] = command;
                for (int i=0; i<len; i++){
                    arr[2+i] = dataBytes[i];
                }
        return arr;
    }

    @Override
    public String toString() {
        return "MqttMessage{" +
                "peripheralId=" + peripheralId +
                ", command=" + command +
                ", data='" + data + '\'' +
                '}';
    }
}
