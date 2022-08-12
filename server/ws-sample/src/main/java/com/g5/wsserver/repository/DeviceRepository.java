package com.g5.wsserver.repository;

import org.springframework.data.domain.Page;
import org.springframework.data.domain.Pageable;
import org.springframework.data.jpa.repository.JpaRepository;

import com.g5.wsserver.model.Device;

public interface DeviceRepository extends JpaRepository<Device, Integer> {


	
}
