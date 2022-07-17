package com.g5.wsserver.configuration;


import org.eclipse.paho.client.mqttv3.IMqttClient;
import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.boot.context.properties.ConfigurationProperties;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

@Configuration
public class MqttConfig {

    @Bean
    @ConfigurationProperties
    public MqttConnectOptions mqttConnectOptions() {
        System.out.println("Creating bean for mqttConnection options");
        return new MqttConnectOptions();
    }

    @Bean
    public IMqttClient mqttClient(@Value("${mqtt.client-id}") final String clientId, @Value("${mqtt.hostname}") final String hostname, @Value("${mqtt.port}") final int port) {
        IMqttClient mqttClient = null;
        System.out.println("Creating bean for mqtt client");
        try {
            mqttClient = new MqttClient("tcp://" + hostname + ":" + port, clientId);
            mqttClient.connect(mqttConnectOptions());
        } catch (MqttException e) {
            System.out.println("Couldn't connect to mqtt broker");
        }
        return mqttClient;
    }

}
