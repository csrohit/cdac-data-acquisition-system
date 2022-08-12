package com.g5.wsserver.model;

import jakarta.persistence.Embeddable;

@Embeddable
public class Parameter extends NameLabelDescription{

    /**
     * Value of parameter
     */
    private Object value;

    public Parameter() {
        super();
    }

    public Object getValue() {
        return value;
    }

    public void setValue(Object value) {
        this.value = value;
    }
}
