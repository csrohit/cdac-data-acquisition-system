package com.g5.wsserver.controller;

import com.g5.wsserver.service.MqttService;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.messaging.handler.annotation.MessageMapping;
import org.springframework.messaging.handler.annotation.Payload;
import org.springframework.messaging.handler.annotation.SendTo;
import org.springframework.messaging.simp.SimpMessagingTemplate;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;

@Controller()
@RequestMapping("")
public class MainController {

    @Autowired
    private SimpMessagingTemplate simpMessagingTemplate;
    @Autowired
    private MqttService mqttService;

    @GetMapping("/")
    public String index(){
        return "index.html";
    }

    @MessageMapping("/news")
    @SendTo("/topic/news")
    public String broadcastNews(@Payload String message) {
        try {
            System.out.println("sending to mqtt" + message);
            if(message.contains("on")){

                mqttService.publish("led", "on", 1, false);
            }else{

                mqttService.publish("led", "off", 1, false);
            }
//            mqttService.subscribe("led");
//            simpMessagingTemplate.convertAndSend("/topic/news", "Hello");
        } catch (MqttException e) {
            return e.getMessage();
        }
        return message;
    }



}