#include<stdint.h>

#ifndef __USB_FRAME_H__
#define __USB_FRAME_H__

#define LED_BUILTIN 0x34
#define LED_YELLOW 0x35
#define LED_WHITE 0x36
#define LED_RED 0x37
#define LED_GREEN 0x38


#define LED_ON 0x01
#define LED_OFF 0x02
#define LED_TOGGLE 0x03
#define LED_BLINK 0x04
#define LED_WRITE 0x05
#define LED_READ 0x06

#define LM35 0x81

#define LM35_READ 0x06

typedef struct struct_usb_frame{
    uint8_t peripheral_id;  
    uint8_t cmd;            
    uint8_t payload_len;
} frame_t;

typedef struct struct_usb_frame_ret{
    uint16_t value;
    uint8_t peripheral_id;  
    uint8_t cmd;            
    uint8_t payload_len;
} frame_ret_t;

#endif