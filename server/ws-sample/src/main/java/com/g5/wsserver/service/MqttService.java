package com.g5.wsserver.service;

import org.eclipse.paho.client.mqttv3.IMqttClient;
import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.messaging.simp.SimpMessagingTemplate;
import org.springframework.stereotype.Service;

import java.nio.charset.StandardCharsets;

@Service
public class MqttService {

    @Autowired
    private IMqttClient mqttClient;

    @Autowired
    private SimpMessagingTemplate simpMessagingTemplate;

    public void publish(String topic, final String payload, int qos, boolean retained) throws MqttException {
        MqttMessage message = new MqttMessage();
        message.setPayload(payload.getBytes(StandardCharsets.UTF_8));
        message.setQos(qos);
        message.setRetained(false);
        mqttClient.publish(topic, message);
    }

    public void publish(String topic, final byte[] payload, int qos, boolean retained) throws MqttException {
        MqttMessage message = new MqttMessage();
        message.setPayload(payload);
        message.setQos(qos);
        message.setRetained(false);
        mqttClient.publish(topic, message);
    }

    public void subscribe(String topic) throws MqttException {
        mqttClient.subscribeWithResponse(topic, (tpc, msg)-> {
            System.out.println("Received @" + topic + " Payload: " + msg);
//            simpMessagingTemplate.convertAndSend("/topic/news", msg.getPayload());
        });
    }

}
