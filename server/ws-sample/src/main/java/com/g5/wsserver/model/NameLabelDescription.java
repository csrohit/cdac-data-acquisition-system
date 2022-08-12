package com.g5.wsserver.model;

import jakarta.persistence.Embeddable;
import jakarta.persistence.MappedSuperclass;

@MappedSuperclass
public class NameLabelDescription {
    /**
     * Key for UI
     */
    String name;

    /**
     * Additional information about command
     */
    String description;

    /**
     * Printable text for UI
     */
    String label;

    public NameLabelDescription() {
    }

    public String getName() {
        return this.name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getDescription() {
        return this.description;
    }

    public void setDescription(String description) {
        this.description = description;
    }

    public String getLabel() {
        return this.label;
    }

    public void setLabel(String label) {
        this.label = label;
    }
}
