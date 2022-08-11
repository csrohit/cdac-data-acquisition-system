package com.g5.wsserver.controller;

import com.g5.wsserver.model.ErrorResponse;
import com.g5.wsserver.model.NodeType;
import com.g5.wsserver.service.NodeTypeService;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.util.List;

@RestController
@RequestMapping("api/node-type")
public class NodeTypeController {

    private static final Logger LOG = LoggerFactory.getLogger(NodeTypeController.class);

    @Autowired
    private NodeTypeService nodeTypeService;


    @PostMapping
    public ResponseEntity<NodeType> save(@RequestBody NodeType nodeType){
        ResponseEntity responseEntity = null;
        try{
            responseEntity = ResponseEntity.status(HttpStatus.CREATED).body(nodeTypeService.save(nodeType));
        }catch (Exception e){
            LOG.error("Couldn't save nodetype", e);
            responseEntity = ResponseEntity.status(HttpStatus.INTERNAL_SERVER_ERROR).body(new ErrorResponse("Couldn't save node type"));
        }
        return responseEntity;
    }

    @GetMapping
    public ResponseEntity<List<NodeType>> findAll(){
        ResponseEntity responseEntity = null;
        try{
            responseEntity = ResponseEntity.ok().body(nodeTypeService.findAll());
        }catch (Exception e){
            LOG.error("Couldn't find nodetype", e);
            responseEntity = ResponseEntity.status(HttpStatus.INTERNAL_SERVER_ERROR).body(new ErrorResponse("Couldn't save node type"));
        }
        return responseEntity;
    }

    @GetMapping("{id}")
    public ResponseEntity<List<NodeType>> findById(@PathVariable(value = "id") Integer id){
        ResponseEntity responseEntity = null;
        try{
            responseEntity = ResponseEntity.ok().body(nodeTypeService.findById(id).orElseThrow());
        }catch (Exception e){
            LOG.error("Couldn't find nodetype", e);
            responseEntity = ResponseEntity.status(HttpStatus.INTERNAL_SERVER_ERROR).body(new ErrorResponse("Couldn't save node type"));
        }
        return responseEntity;
    }

}
