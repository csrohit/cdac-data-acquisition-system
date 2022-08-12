package com.g5.wsserver.model;

import java.util.Set;

import jakarta.persistence.*;

@Entity
public class DeviceType extends IdLabelDescription {

	@ElementCollection
	private Set<Command> cmdList;

	@ElementCollection
	private Set<Command> retCmdList;


	private Direction direction;

	private Interface intf;


	public DeviceType() {
		super();
	}


	public Set<Command> getCmdList() {
		return cmdList;
	}

	public void setCmdList(Set<Command> cmdList) {
		this.cmdList = cmdList;
	}

	public Set<Command> getRetCmdList() {
		return retCmdList;
	}

	public void setRetCmdList(Set<Command> retCmdList) {
		this.retCmdList = retCmdList;
	}

	public Direction getDirection() {
		return direction;
	}

	public void setDirection(Direction direction) {
		this.direction = direction;
	}

	public Interface getIntf() {
		return intf;
	}

	public void setIntf(Interface intf) {
		this.intf = intf;
	}

	@Override
	public String toString() {
		return "DeviceType{" +
				"cmdList=" + cmdList +
				", retCmdList=" + retCmdList +
				", direction=" + direction +
				", intf=" + intf +
				"} " + super.toString();
	}
}
