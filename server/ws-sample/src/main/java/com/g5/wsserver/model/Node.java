package com.g5.wsserver.model;

import jakarta.persistence.*;
import reactor.util.annotation.NonNull;

import java.util.List;

@Entity
public class Node extends IdLabelDescription{


    @ManyToOne(fetch = FetchType.EAGER, optional = false)
    private NodeType type;

    public Node() {
        super();
    }

    public NodeType getType() {
        return type;
    }

    public void setType(NodeType type) {
        this.type = type;
    }

    @Override
    public String toString() {
        return "Node{" +
                "type=" + type +
                "} " + super.toString();
    }


}
