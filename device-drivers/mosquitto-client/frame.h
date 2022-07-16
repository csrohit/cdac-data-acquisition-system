
#include <stdint.h>



typedef struct struct_mqtt_frame{
    uint8_t serial_no;
    uint8_t peripheral_addr;
    uint8_t cmd;
}mqtt_frame_t;


typedef struct struct_usb_frame{
    uint8_t cmd;
    uint8_t addr;
    uint8_t payload_len;
} usb_frame_t;
