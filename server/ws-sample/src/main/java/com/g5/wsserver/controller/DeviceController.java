package com.g5.wsserver.controller;

import com.g5.wsserver.exceptions.NotFoundException;
import com.g5.wsserver.model.*;
import com.g5.wsserver.service.DeviceService;
import com.g5.wsserver.service.MqttService;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.messaging.simp.SimpMessagingTemplate;
import org.springframework.web.bind.annotation.*;

import java.util.List;
import java.util.Optional;

@RestController
@RequestMapping("api/device")
public class DeviceController {

    private static final Logger LOG = LoggerFactory.getLogger(DeviceController.class);

    @Autowired
    private DeviceService service;

    @Autowired
    private SimpMessagingTemplate simpMessagingTemplate;

    @Autowired
    private MqttService mqttService;


    @GetMapping
    ResponseEntity<List<Device>> findAll() {
        return new ResponseEntity<>(service.findAll(), HttpStatus.OK);
    }


    @PostMapping
    public ResponseEntity<Device> save(@RequestBody Device device) {
        return ResponseEntity.status(HttpStatus.CREATED).body(service.save(device));
    }

    @GetMapping("{id}")
    public ResponseEntity<Device> findById(@PathVariable(value = "id") Integer id) {
        Optional<Device> device = service.findById(id);
        ResponseEntity responseEntity;
        if (device.isPresent()) {
            responseEntity = new ResponseEntity(device.get(), HttpStatus.OK);
        } else {
            responseEntity = new ResponseEntity(null, HttpStatus.NOT_FOUND);
        }

        return responseEntity;

    }

    @PutMapping("{id}")
    public ResponseEntity<Node> update(@PathVariable(value = "id") Integer id, @RequestBody Device device){
        ResponseEntity responseEntity = null;
        try{
            responseEntity = new ResponseEntity(service.update(id, device), HttpStatus.OK);
        }catch (NotFoundException e){
            LOG.error("Couldn't find device", e);
            responseEntity = ResponseEntity.status(HttpStatus.BAD_REQUEST).body(new ErrorResponse("Invalid device id"));
        }catch (Exception e){
            LOG.error("Couldn't find device", e);
            responseEntity = ResponseEntity.status(HttpStatus.INTERNAL_SERVER_ERROR).body(new ErrorResponse("Couldn't find device"));
        }
        return responseEntity;
    }

    @DeleteMapping("{id}")
    public ResponseEntity<Node> delete(@PathVariable(value = "id") Integer id){
        ResponseEntity responseEntity = null;
        try{
            service.delete(id);
            responseEntity = ResponseEntity.ok().build();
        }catch (NotFoundException e){
            LOG.error("Couldn't find device", e);
            responseEntity = ResponseEntity.status(HttpStatus.BAD_REQUEST).body(new ErrorResponse("Invalid device id"));
        }catch (Exception e){
            LOG.error("Couldn't find device", e);
            responseEntity = ResponseEntity.status(HttpStatus.INTERNAL_SERVER_ERROR).body(new ErrorResponse("Couldn't find device"));
        }
        return responseEntity;
    }

    @PutMapping("{id}/cmd")
    public ResponseEntity<MqttMessage> runCommand(@PathVariable(value = "id") Integer id, @RequestBody Command command) {
        System.out.println("Request received");
        LOG.info("Device id => " + id);
        LOG.info("Command => " +command.toString());

        Optional<Device> optionalDevice = service.findById(id);
        if (optionalDevice.isEmpty()) {
            return new ResponseEntity(new ErrorResponse("Invalid device Id"), HttpStatus.BAD_REQUEST);
        }
        LOG.info("Found device");
        Device device = optionalDevice.get();
        DeviceType type = device.getType();
        Optional<Command> optionalCommand = type.getCmdList().stream().filter(cmd -> cmd.getName().equalsIgnoreCase(command.getName())).findFirst();
        if (optionalCommand.isEmpty()) {
            return new ResponseEntity(new ErrorResponse("Invalid command name"), HttpStatus.BAD_REQUEST);
        }
        LOG.info("Found command");
        Command savedCommand = optionalCommand.get();
        MqttMessage message = new MqttMessage();
        message.setCommand(savedCommand.getHwValue());
        message.setPeripheralId(device.getHwId());

        try {
            byte f[] = message.getBytes();
            System.out.println("Length: " + f.length);
            System.out.println("Peripheral: " + f[0]);
            System.out.println("Command: " + f[1]);
            mqttService.publish(device.getNode().getMqTopic(), f, 1, false);
//            simpMessagingTemplate.convertAndSend("/topic/news", "Hello");
        } catch (MqttException e) {
            LOG.error("Something went wrong", e);
            return new ResponseEntity(new ErrorResponse(e.getMessage()), HttpStatus.INTERNAL_SERVER_ERROR);
        }
        return ResponseEntity.ok().body(message);

    }
}
