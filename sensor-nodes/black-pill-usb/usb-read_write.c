#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;

typedef struct {
    uint8_t cmd;
    uint8_t addr;
    uint8_t len;
} frame_t;

int main()
{
    int fd = open("/dev/ttyACM0", O_RDWR), ret;
    frame_t f = {
        .cmd = 0x34,
        .addr = 0x01,
        .len = 0x02
    }, r;
    if (fd < 0)
    {
        perror("open() failed");
        _exit(-1);
    }

    int count = 10;
    char HIGH = '1', LOW = '0';
    unsigned int flag = 0;
    ret = write(fd, (void *)&f, sizeof(f));
    if(ret < 0){
        perror("write() failed");
        close(fd);
        _exit(-1);
    }
    printf("Wrote %d bytes\n", ret);


    // ret = read(fd, (void *)&r, sizeof(r));
    //     if(ret < 0){
    //     perror("read() failed");
    //     close(fd);
    //     _exit(-1);
    // }
    // printf("read %d bytes\n", ret);
    // printf("CMD: %#x\n", r.cmd);

    // printf("ADDR: %#x\n", r.addr);
    // printf("LEN: %#x\n", r.len);

    close(fd);

    return 0;
}