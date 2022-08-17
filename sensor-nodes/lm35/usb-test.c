#include "usb-frame.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>


int main()
{
    int choice;
    frame_t frame;
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
            printf("Temperature: 34\n");
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
            char buff[4];
            ret = read(fd, buff, sizeof(buff));
            printf("Read %d bytes\n", ret);
            printf("Peripheral id: %#x, cmd: %#x, len: %#x\n", buff[0], buff[1], buff[2]);
            printf("Temperature data: %#x\n", buff[3]);
            break;
        default:
            printf("Invalid choice\n");
            break;
        }
    } while (choice);

    close(fd);

    return 0;
}