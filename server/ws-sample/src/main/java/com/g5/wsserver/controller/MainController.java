package com.g5.wsserver.controller;

import com.g5.wsserver.model.MqttUsbMessage;
import com.g5.wsserver.model.WebsocketMessage;
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

    @MessageMapping("/usb")
    // @SendTo("/topic/news")
    public String broadcastNews(@Payload WebsocketMessage message) {
//        try {
            System.out.println("Serial: "+ message.getDeviceId());
            System.out.println("Peripheral: "+ message.getPeripheralId());
            System.out.println("Command: "+ message.getCommandId());
        try {
            MqttUsbMessage m = new MqttUsbMessage();
            m.setCommand((byte) message.getCommandId());
            m.setData("");
            m.setSerial((byte) message.getDeviceId());
            m.setPeripheralId((byte) message.getPeripheralId());
            byte f[] = m.getBytes();
            System.out.println("Length: " + f.length);
            System.out.println("Serial: " + f[0]);
            System.out.println("Peripheral: " + f[1]);
            System.out.println("Command: " + f[2]);
            mqttService.publish("usb/black-pill", m.getBytes(), 1, false);
//            simpMessagingTemplate.convertAndSend("/topic/news", "Hello");
        } catch (MqttException e) {
            return e.getMessage();
        }


//            System.out.println("sending to mqtt" + message);
//            if(message.contains("on")){
//
//                mqttService.publish("led", "on", 1, false);
//            }else{
//
//                mqttService.publish("led", "off", 1, false);
//            }
//            mqttService.subscribe("led");
//            simpMessagingTemplate.convertAndSend("/topic/news", "Hello");
//        } catch (MqttException e) {
//            return e.getMessage();
//        }
        return "done";
    }

    @MessageMapping("/out/+")
    public void outputValue(@Payload WebsocketMessage message){
        System.out.println("message at /out/+");
    }



    @MessageMapping("/news")
    @SendTo("/topic/news")
    public String broadcastNews(@Payload String message) {
        return message;
    }

}