package io.e3.gr5.prj.service;

import org.eclipse.paho.client.mqttv3.IMqttClient;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;
import org.eclipse.paho.client.mqttv3.MqttPersistenceException;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.ArrayList;
import java.util.List;

@Service
public class MessagingService {

    private final List<String> subscriptions = new ArrayList<>();
    @Autowired
    private IMqttClient mqttClient;

    public void publish(final String topic, final String payload, int qos, boolean retained)
            throws MqttException {
        MqttMessage mqttMessage = new MqttMessage();
        mqttMessage.setPayload(payload.getBytes());
        mqttMessage.setQos(qos);
        mqttMessage.setRetained(retained);

        mqttClient.publish(topic, mqttMessage);

        //mqttClient.publish(topic, payload.getBytes(), qos, retained);

//		mqttClient.disconnect();
    }

    public void subscribe(final String topic) throws MqttException {
        System.out.println("Messages received:");
//		mqttClient.
        subscriptions.add(topic);

        mqttClient.subscribeWithResponse(topic, (tpic, msg) ->
                System.out.println(msg.getId() + " -> " + new String(msg.getPayload()))
        );
    }

    public void unsubscribe(final String topic) throws MqttException {
        subscriptions.remove(topic);
        mqttClient.unsubscribe(topic);
    }

    public List<String> getSubscriptions() {
        return subscriptions;
    }
}