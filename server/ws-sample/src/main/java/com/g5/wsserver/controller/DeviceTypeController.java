package com.g5.wsserver.controller;

import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.data.domain.Page;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import com.g5.wsserver.model.DeviceType;
import com.g5.wsserver.service.DeviceTypeService;

@RestController
@RequestMapping("api/device-type")
public class DeviceTypeController {

	@Autowired
	private DeviceTypeService deviceTypeService;
	
	@GetMapping()
	public ResponseEntity<List<DeviceType>> findAll(){
		
		return new ResponseEntity<>(deviceTypeService.findAll(), HttpStatus.OK);
	}
	
	@PostMapping()
	public ResponseEntity<?> save(@RequestBody DeviceType deviceType){
		return new ResponseEntity<DeviceType>(deviceTypeService.save(deviceType), HttpStatus.CREATED);
	}
	
}
