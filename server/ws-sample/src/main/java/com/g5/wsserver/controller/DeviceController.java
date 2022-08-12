package com.g5.wsserver.controller;

import com.g5.wsserver.model.Device;
import com.g5.wsserver.service.DeviceService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.util.List;
import java.util.Optional;
import java.util.Set;

@RestController
@RequestMapping("api/device")
public class DeviceController {

    @Autowired
    private DeviceService deviceService;

    @GetMapping
    ResponseEntity<List<Device>> findAll(){
        return new ResponseEntity<>(deviceService.findAll(), HttpStatus.OK);
    }


    @PostMapping
    public ResponseEntity<Device> save(@RequestBody Device device){
        return ResponseEntity.status(HttpStatus.CREATED).body(deviceService.save(device));
    }

    @GetMapping("{id}")
    public ResponseEntity<Device> findById(@PathVariable(value = "id") Integer id){
        Optional<Device> device = deviceService.findById(id);
        ResponseEntity responseEntity;
        if(device.isPresent()){
            responseEntity = new ResponseEntity(device.get(), HttpStatus.OK);
        }else{
            responseEntity = new ResponseEntity(null, HttpStatus.NOT_FOUND);
        }

        return  responseEntity;

    }
}
