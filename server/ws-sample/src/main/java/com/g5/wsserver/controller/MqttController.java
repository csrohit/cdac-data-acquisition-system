package com.g5.wsserver.controller;

import com.g5.wsserver.model.MqttMessage;
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

//    @GetMapping
//    public String hello(){
//        try {
//            MqttMessage m = new MqttMessage((byte) 0x01, (byte) 0x01, "");
//            byte f[] = m.getBytes();
//            System.out.println("Length: " + f.length);
//            System.out.println("Peripheral: " + f[0]);
//            System.out.println("Command: " + f[1]);
//            mqttService.publish("usb/black-pill", m.getBytes(), 1, false);
////            simpMessagingTemplate.convertAndSend("/topic/news", "Hello");
//        } catch (MqttException e) {
//            return e.getMessage();
//        }
//        return  "Subscribing to topic";
//    }
}
