package com.g5.wsserver.controller;

import com.g5.wsserver.service.MqttService;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.messaging.simp.SimpMessagingTemplate;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
@RequestMapping("mqtt")
public class MqttController {

    @Autowired
    private MqttService mqttService;

    @Autowired
    private SimpMessagingTemplate simpMessagingTemplate;

    @GetMapping
    public String hello(){
        try {

            mqttService.subscribe("led");
            mqttService.publish("led", "off", 1, false);
            simpMessagingTemplate.convertAndSend("/topic/news", "Hello");
        } catch (MqttException e) {
            return e.getMessage();
        }
        return  "Subscribing to topic";
    }
}
