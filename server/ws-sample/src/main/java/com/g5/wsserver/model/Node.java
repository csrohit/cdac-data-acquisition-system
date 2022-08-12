package com.g5.wsserver.model;

import com.fasterxml.jackson.annotation.JsonIgnore;
import jakarta.persistence.*;
import reactor.util.annotation.NonNull;

import java.util.List;
import java.util.Set;

@Entity
public class Node extends IdLabelDescription{


    @ManyToOne(fetch = FetchType.EAGER, optional = false)
    private NodeType type;

    private String mqTopic;

    @JsonIgnore
    @OneToMany(mappedBy = "node")
    private Set<Device> devices;

    public Node() {
        super();
    }

    public NodeType getType() {
        return type;
    }

    public void setType(NodeType type) {
        this.type = type;
    }


    public String getMqTopic() {
        return mqTopic;
    }

    public void setMqTopic(String mqTopic) {
        this.mqTopic = mqTopic;
    }

    public Set<Device> getDevices() {
        return devices;
    }

    public void setDevices(Set<Device> devices) {
        this.devices = devices;
    }

    @Override
    public String toString() {
        return "Node{" +
                "type=" + type +
                "} " + super.toString();
    }


}
