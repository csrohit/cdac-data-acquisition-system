#include<stdint.h>

#ifndef __USB_FRAME_H__
#define __USB_FRAME_H__

typedef struct struct_usb_frame{
    uint8_t peripheral_id;
    uint8_t cmd;
    uint8_t payload_len;
} frame_t;

#endif