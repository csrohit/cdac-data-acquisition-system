package com.g5.wsserver.service;

import com.g5.wsserver.model.NodeType;
import com.g5.wsserver.repository.NodeTypeRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.data.domain.Page;
import org.springframework.data.domain.Pageable;
import org.springframework.stereotype.Service;
import org.yaml.snakeyaml.nodes.NodeTuple;

import java.util.List;
import java.util.Optional;
import java.util.Set;

@Service
public class NodeTypeService {

    @Autowired
    private NodeTypeRepository nodeTypeRepository;


    public NodeType save(NodeType nodeType){
        return nodeTypeRepository.save(nodeType);
    }

    public List<NodeType> findAll(){
        return nodeTypeRepository.findAll();
    }

    public Page<NodeType> findAll(Pageable pageable){
        return nodeTypeRepository.findAll(pageable);
    }

    public Optional<NodeType> findById(Integer id){
        return nodeTypeRepository.findById(id);
    }


}
