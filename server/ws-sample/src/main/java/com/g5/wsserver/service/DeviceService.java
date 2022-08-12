package com.g5.wsserver.service;


import com.g5.wsserver.model.Device;
import com.g5.wsserver.repository.DeviceRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;


import java.util.List;
import java.util.Optional;

@Service
public class DeviceService {

    @Autowired
    private DeviceRepository deviceRepository;

    public List<Device> findAll(){
        return deviceRepository.findAll();
    }



    public Device save(Device device){
        return deviceRepository.save(device);
    }


    public Optional<Device> findById(Integer id){
        return deviceRepository.findById(id);
    }



}
