#include "usb-frame.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int main()
{
    int choice;
    frame_t frame;
    frame_ret_t *buff;
    int ret;
    int fd = open("/dev/node0", O_RDWR);
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
            frame.cmd = LED_TOGGLE;
            frame.peripheral_id = LED_BUILTIN;
            frame.payload_len = 0U;
            ret = write(fd, &frame, sizeof(frame));
            if (ret < 0)
            {
                perror("write() failed");
                close(fd);
                _exit(EXIT_FAILURE);
            }
            break;
        case 0:
            break;
        case 2:
            printf("Toggling Yellow led\n");
            frame.cmd = LED_TOGGLE;
            frame.peripheral_id = LED_YELLOW;
            frame.payload_len = 0U;
            ret = write(fd, &frame, sizeof(frame));
            if (ret < 0)
            {
                perror("write() failed");
                close(fd);
                _exit(EXIT_FAILURE);
            }
            break;
        case 3:
            frame.cmd = LM35_READ;
            frame.peripheral_id = LM35;
            frame.payload_len = 0U;
            ret = write(fd, &frame, sizeof(frame));
            if (ret < 0)
            {
                perror("write() failed");
                close(fd);
                _exit(EXIT_FAILURE);
            }
            buff = (frame_ret_t *)malloc(sizeof(frame_ret_t));
            ret = read(fd, buff, sizeof(buff));
            printf("Read %d bytes\n", ret);
            printf("Peripheral id: %#x, cmd: %#x, len: %#x, value: %#x\n", buff->peripheral_id, buff->cmd, buff->payload_len, buff->value);
            // printf("Byte stream: %#x %#x %#x %#x %#x", (uint8_t)buff[0], (uint8_t)buff[1], (uint8_t)buff[2], (uint8_t)buff[3], (uint8_t)buff[4]);

            // int16_t *val = (int16_t *)(buff + 3);
            printf("Temperature data: %#x\n", buff->value);
            free(buff);
            // printf("Temperature data: %#x\n",  (int16_t)buff[3]);
            break;
        default:
            printf("Invalid choice\n");
            break;
        }
    } while (choice);

    close(fd);

    return 0;
}