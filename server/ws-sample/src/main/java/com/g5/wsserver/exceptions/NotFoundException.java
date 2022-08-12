package com.g5.wsserver.exceptions;

public class NotFoundException extends Exception{
    public NotFoundException() {
    }

    public NotFoundException(String message) {
        super(message);
    }
}
