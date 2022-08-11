package com.g5.wsserver.repository;

import com.g5.wsserver.model.NodeType;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;

@Repository
public interface NodeTypeRepository extends JpaRepository<NodeType, Integer> {
}
