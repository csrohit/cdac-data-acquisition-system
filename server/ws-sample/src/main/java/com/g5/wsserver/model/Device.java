package com.g5.wsserver.model;

import jakarta.persistence.*;
import reactor.util.annotation.NonNull;


@Entity
public class Device extends IdLabelDescription{

	@NonNull
	private byte hwId;

	@NonNull
	@ManyToOne
	@JoinColumn(name = "fk_node")
	private Node node;

    @ManyToOne(fetch = FetchType.EAGER, optional = false)
    private DeviceType type;


	public Device() {
		super();
		this.hwId = (byte) 0xff;
	}

	@NonNull
	public byte getHwId() {
		return hwId;
	}

	public void setHwId(@NonNull byte hwId) {
		this.hwId = hwId;
	}

	@NonNull
	public Node getNode() {
		return node;
	}

	public void setNode(@NonNull Node node) {
		this.node = node;
	}

	public DeviceType getType() {
		return type;
	}

	public void setType(DeviceType type) {
		this.type = type;
	}



	@Override
	public String toString() {
		return "Device{" +
				"hwId=" + hwId +
				", node=" + node +
				", type=" + type +
				"} " + super.toString();
	}
}
