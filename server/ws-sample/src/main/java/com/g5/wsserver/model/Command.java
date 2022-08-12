package com.g5.wsserver.model;

import jakarta.persistence.Embeddable;

@Embeddable
public class Command extends NameLabelDescription {


    /**
     * Actual value passed to device
     */
    byte hwValue;


    public Command() {
        super();
    }


    public byte getHwValue() {
        return this.hwValue;
    }

    public void setHwValue(byte hwValue) {
        this.hwValue = hwValue;
    }

    @Override
    public String toString() {
        return "Command{" +
                "hw_value=" + hwValue +
                "} " + super.toString();
    }
}
