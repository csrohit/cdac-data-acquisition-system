package com.g5.wsserver.service;

import com.g5.wsserver.exceptions.NotFoundException;
import com.g5.wsserver.model.Device;
import com.g5.wsserver.model.Node;
import com.g5.wsserver.repository.NodeRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.data.domain.Page;
import org.springframework.data.domain.Pageable;
import org.springframework.lang.NonNull;
import org.springframework.stereotype.Service;

import java.util.List;
import java.util.Optional;
import java.util.Set;

@Service
public class NodeService {

    @Autowired
    private NodeRepository repository;

    public List<Node> findAll(){
        return repository.findAll();
    }


    public Page<Node> findAll(@NonNull Pageable pageable){
        return repository.findAll(pageable);
    }


    public Node save(@NonNull Node node){
        return repository.save(node);
    }


    public Optional<Node> findById(@NonNull Integer id){
        return repository.findById(id);
    }

    public Set<Device> getDevices(@NonNull Integer nodeId) throws NotFoundException {

        Optional<Node> optionalNode = repository.findById(nodeId);
        if(optionalNode.isEmpty()){
            throw new NotFoundException("Node doesn't exist");
        }

        Node node = optionalNode.get();

        return node.getDevices();
    }
}
