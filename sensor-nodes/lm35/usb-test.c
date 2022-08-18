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
        printf("1. Toggle Blue led\n2. Toggle Yellow Led\n2. Toggle Green Led\n4. Toggle Red Led\n5. Toggle White Led\n6. Blink Red Led\n7. Get temperature\n0. Exit\nEnter your choice: ");
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
            printf("Toggling Green led\n");
            frame.cmd = LED_TOGGLE;
            frame.peripheral_id = LED_GREEN;
            frame.payload_len = 0U;
            ret = write(fd, &frame, sizeof(frame));
            if (ret < 0)
            {
                perror("write() failed");
                close(fd);
                _exit(EXIT_FAILURE);
            }
            break;
        case 4:
            printf("Toggling red led\n");
            frame.cmd = LED_TOGGLE;
            frame.peripheral_id = LED_RED;
            frame.payload_len = 0U;
            ret = write(fd, &frame, sizeof(frame));
            if (ret < 0)
            {
                perror("write() failed");
                close(fd);
                _exit(EXIT_FAILURE);
            }
            break;
        case 5:
            printf("Toggling white led\n");
            frame.cmd = LED_TOGGLE;
            frame.peripheral_id = LED_WHITE;
            frame.payload_len = 0U;
            ret = write(fd, &frame, sizeof(frame));
            if (ret < 0)
            {
                perror("write() failed");
                close(fd);
                _exit(EXIT_FAILURE);
            }
            break;
        case 7:
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
        case 6:
            printf("Blink red led\n");
            frame.cmd = LED_BLINK;
            frame.peripheral_id = LED_RED;
            frame.payload_len = 0U;
            ret = write(fd, &frame, sizeof(frame));
            if (ret < 0)
            {
                perror("write() failed");
                close(fd);
                _exit(EXIT_FAILURE);
            }
            break;
        default:
            printf("Invalid choice\n");
            break;
        }
    } while (choice);

    close(fd);

    return 0;
}