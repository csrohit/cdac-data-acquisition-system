package com.g5.wsserver.controller;


import com.g5.wsserver.exceptions.NotFoundException;
import com.g5.wsserver.model.Device;
import com.g5.wsserver.model.ErrorResponse;
import com.g5.wsserver.model.Node;
import com.g5.wsserver.service.NodeService;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.util.List;

@RestController
@RequestMapping("api/node")
public class NodeController {

    private static final Logger LOG = LoggerFactory.getLogger(NodeController.class);

    @Autowired
    private NodeService service;


    @PostMapping
    public ResponseEntity<Node> save(@RequestBody Node node) {
        ResponseEntity responseEntity = null;
        try {
            responseEntity = ResponseEntity.status(HttpStatus.CREATED).body(service.save(node));
        } catch (Exception e) {
            LOG.debug(node.toString());
            LOG.error("Couldn't save node", e);
            responseEntity = ResponseEntity.status(HttpStatus.INTERNAL_SERVER_ERROR).body(new ErrorResponse("Couldn't save node"));
        }
        return responseEntity;
    }

    @GetMapping
    public ResponseEntity<List<Node>> findAll() {
        ResponseEntity responseEntity = null;
        try {
            responseEntity = ResponseEntity.ok().body(service.findAll());
        } catch (Exception e) {
            LOG.error("Couldn't find node", e);
            responseEntity = ResponseEntity.status(HttpStatus.INTERNAL_SERVER_ERROR).body(new ErrorResponse("Couldn't save node"));
        }
        return responseEntity;
    }

    @GetMapping("{id}")
    public ResponseEntity<List<Node>> findById(@PathVariable(value = "id") Integer id) {
        ResponseEntity responseEntity = null;
        try {
            responseEntity = ResponseEntity.ok().body(service.findById(id).orElseThrow());
        } catch (Exception e) {
            LOG.error("Couldn't find node", e);
            responseEntity = ResponseEntity.status(HttpStatus.INTERNAL_SERVER_ERROR).body(new ErrorResponse("Couldn't save node"));
        }
        return responseEntity;
    }

    @GetMapping("{id}/devices")
    public ResponseEntity<List<Device>> findDevices(@PathVariable(value = "id") Integer id) {
        ResponseEntity responseEntity = null;
        try {
            responseEntity = ResponseEntity.ok().body(service.getDevices(id));
        } catch (NotFoundException e) {
            LOG.error("Couldn't find node", e);
            responseEntity = ResponseEntity.status(HttpStatus.BAD_REQUEST).body(new ErrorResponse("Invalid node id"));
        } catch (Exception e) {
            LOG.error("Couldn't find node", e);
            responseEntity = ResponseEntity.status(HttpStatus.INTERNAL_SERVER_ERROR).body(new ErrorResponse("Couldn't find node"));
        }
        return responseEntity;
    }

    @PutMapping("{id}")
    public ResponseEntity<Node> update(@PathVariable(value = "id") Integer id, @RequestBody Node node) {
        ResponseEntity responseEntity = null;
        try {
            responseEntity = new ResponseEntity(service.update(id, node), HttpStatus.OK);
        } catch (NotFoundException e) {
            LOG.error("Couldn't find node", e);
            responseEntity = ResponseEntity.status(HttpStatus.BAD_REQUEST).body(new ErrorResponse("Invalid node id"));
        } catch (Exception e) {
            LOG.error("Couldn't find node", e);
            responseEntity = ResponseEntity.status(HttpStatus.INTERNAL_SERVER_ERROR).body(new ErrorResponse("Couldn't find node"));
        }
        return responseEntity;
    }

    @DeleteMapping("{id}")
    public ResponseEntity<?> delete(@PathVariable(value = "id") Integer id) {
        ResponseEntity responseEntity = null;
        try {
            service.delete(id);
            responseEntity = ResponseEntity.ok().build();
        } catch (NotFoundException e) {
            LOG.error("Couldn't find node", e);
            responseEntity = ResponseEntity.status(HttpStatus.BAD_REQUEST).body(new ErrorResponse("Invalid node id"));
        } catch (Exception e) {
            LOG.error("Couldn't find node", e);
            responseEntity = ResponseEntity.status(HttpStatus.INTERNAL_SERVER_ERROR).body(new ErrorResponse("Couldn't find node"));
        }
        return responseEntity;
    }

}
