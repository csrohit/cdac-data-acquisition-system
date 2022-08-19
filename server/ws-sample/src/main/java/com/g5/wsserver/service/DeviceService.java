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

    /**
     * UPdate device details
     * @param id id of device
     * @param device devices details to be updated
     * @return updated device
     * @throws NotFoundException if device does not exist
     */
    public Device update(Integer id, Device device) throws NotFoundException {
        Optional<Device> optionalDevice = repository.findById(id);
        if(optionalDevice.isEmpty()){
            throw new NotFoundException("Device doesn't exist");
        }
        Device existingDevice = optionalDevice.get();
        System.out.println(device.getHwId());
        if(device.getHwId() != 0xff)
            existingDevice.setHwId(device.getHwId());

        if(!device.getLabel().isEmpty())
            existingDevice.setLabel(device.getLabel());

        if(!device.getDescription().isEmpty())
            existingDevice.setDescription(device.getDescription());

        if(device.getType() != null && device.getType().getId() != null)
            existingDevice.setType(device.getType());

        return repository.save(existingDevice);
    }

    /**
     * Delete device
     * @param id id of device
     * @throws NotFoundException if device does not exist
     */
    public void delete(Integer id) throws NotFoundException {
        Optional<Device> optionalDevice = repository.findById(id);
        if(optionalDevice.isEmpty()){
            throw new NotFoundException("Device doesn't exist");
        }
        this.repository.deleteById(id);
    }

}
