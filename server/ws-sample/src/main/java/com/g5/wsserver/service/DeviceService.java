package com.g5.wsserver.service;


import com.g5.wsserver.exceptions.NotFoundException;
import com.g5.wsserver.model.Device;
import com.g5.wsserver.model.Node;
import com.g5.wsserver.repository.DeviceRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;


import java.util.List;
import java.util.Optional;

@Service
public class DeviceService {

    @Autowired
    private DeviceRepository repository;

    public List<Device> findAll(){
        return repository.findAll();
    }



    public Device save(Device device){
        return repository.save(device);
    }


    public Optional<Device> findById(Integer id){
        return repository.findById(id);
    }

    public Device update(Integer id, Device device) throws NotFoundException {
        Optional<Device> optionalDevice = repository.findById(id);
        if(optionalDevice.isEmpty()){
            throw new NotFoundException("Node doesn't exist");
        }
        Device existingDevice = optionalDevice.get();
        existingDevice.setHwId(device.getHwId());
        return repository.save(existingDevice);
    }

}
