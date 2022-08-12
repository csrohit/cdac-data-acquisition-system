package com.g5.wsserver.repository;

import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;

import com.g5.wsserver.model.DeviceType;

@Repository
public interface DeviceTypeRepository extends JpaRepository<DeviceType, Integer>{

}
