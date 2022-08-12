package com.g5.wsserver.service;

import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.data.domain.Page;
import org.springframework.data.domain.Pageable;
import org.springframework.stereotype.Service;

import com.g5.wsserver.model.DeviceType;
import com.g5.wsserver.repository.DeviceTypeRepository;

@Service
public class DeviceTypeService {

	@Autowired
	private DeviceTypeRepository deviceTypeRepository;
	
	public Page<DeviceType> findAll(Pageable pageable){
		return deviceTypeRepository.findAll(pageable);
	}
	
	public List<DeviceType> findAll(){
		return deviceTypeRepository.findAll();
	}
	
	
	public DeviceType save(DeviceType deviceType) {
		return deviceTypeRepository.save(deviceType);
	}
	
}
