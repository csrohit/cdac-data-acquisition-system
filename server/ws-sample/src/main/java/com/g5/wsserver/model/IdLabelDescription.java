package com.g5.wsserver.model;

import jakarta.persistence.GeneratedValue;
import jakarta.persistence.GenerationType;
import jakarta.persistence.Id;
import jakarta.persistence.MappedSuperclass;
import reactor.util.annotation.NonNull;

@MappedSuperclass
public class IdLabelDescription {

    @Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    private Integer id;

    private String description;

    @NonNull
    private String label;

    public IdLabelDescription() {
    }


    public Integer getId() {
        return id;
    }

    public void setId(Integer id) {
        this.id = id;
    }

    public String getDescription() {
        return description;
    }

    public void setDescription(String description) {
        this.description = description;
    }

    public String getLabel() {
        return label;
    }

    public void setLabel(String label) {
        this.label = label;
    }


    @Override
    public String toString() {
        return "IdLabelDescription{" +
                "id=" + id +
                ", description='" + description + '\'' +
                ", label='" + label + '\'' +
                '}';
    }
}
