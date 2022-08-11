package com.g5.wsserver.model;

import jakarta.persistence.Entity;
import jakarta.persistence.GeneratedValue;
import jakarta.persistence.GenerationType;
import jakarta.persistence.Id;

@Entity
public class NodeType extends IdLabelDescription{
    public NodeType() {
        super();
    }
}
