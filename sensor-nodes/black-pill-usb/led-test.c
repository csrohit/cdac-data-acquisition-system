#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
    int fd = open("/dev/ttyACM0", O_WRONLY);
    if (fd < 0)
    {
        perror("open() failed");
        _exit(-1);
    }

    int count = 10;
    char HIGH = '1', LOW = '0';
    unsigned int flag = 0;
    while (count > 0)
    {
        if (flag == 0)
        {
            flag = 1;
            printf("LED OFF\n");
            write(fd, (void *)&HIGH, 1);
        }
        else
        {
            flag = 0;
            printf("LED ON\n");
            write(fd, (void *)&LOW, 1);
        }
        sleep(1);
        count--;
    }

    close(fd);

    return 0;
}