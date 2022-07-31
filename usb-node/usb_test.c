#include "usb-frame.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#define LED_BLUE 0x34
#define LED_YELLOW 0x35
// #define LED_BLUE 0x34;
#define CMD_TOGGLE 0x04

int main()
{
    int choice;
    frame_t frame;
    int ret;
    int fd = open("/dev/node0", O_WRONLY);
    if (fd < 0)
    {
        perror("open() failed\n");
        _exit(EXIT_FAILURE);
    }
    printf("USB device test\n\n");
    do
    {
        printf("1. Toggle Blue led\n2. Toggle Yellow Led\n3. Get temperature\n0. Exit\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            printf("Toggling Blue led\n");
            frame.cmd = CMD_TOGGLE;
            frame.peripheral_id = LED_BLUE;
            frame.payload_len = 0U;
            ret = write(fd, &frame, sizeof(frame));
            if(ret < 0){
                perror("write() failed");
                close(fd);
                _exit(EXIT_FAILURE);
            }
            break;
        case 0:
            break;
        case 2:
            printf("Toggling Yellow led\n");
            frame.cmd = CMD_TOGGLE;
            frame.peripheral_id = LED_YELLOW;
            frame.payload_len = 0U;
            ret = write(fd, &frame, sizeof(frame));
            if(ret < 0){
                perror("write() failed");
                close(fd);
                _exit(EXIT_FAILURE);
            }
            break;
        case 3:
            printf("Temperature: 34\n");
        default:
            printf("Invalid choice\n");
            break;
        }
    } while (choice);

    close(fd);

    return 0;
}