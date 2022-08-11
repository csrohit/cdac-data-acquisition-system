package com.g5.wsserver.repository;

import com.g5.wsserver.model.Node;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;

@Repository
public interface NodeRepository extends JpaRepository<Node, Integer> {
}
