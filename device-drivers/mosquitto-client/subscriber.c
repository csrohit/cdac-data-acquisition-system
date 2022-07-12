#include <stdio.h>
#include <errno.h> // to check errno variable
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <mosquitto.h> // for mosquitto connections
#include <mqtt_protocol.h>
#include <signal.h>
#include <stdint.h>
#define DEBUG

int mid = 12;
const char *host = "localhost";
const char *bind_address = "0.0.0.0";
const int port = 1883U;
const char *topic = "usb/black-pill";
const char *client_id = "usb_handler";
const char *priv_data = "Data for all callbacks";
struct mosquitto *client = NULL;
uint8_t isSubscribed = 0;

typedef struct {
    uint8_t addr;
    uint8_t cmd;
    uint8_t len;
} frame_t;

// function declarations
void termination_handler (int);
void on_connect(struct mosquitto *, void *, int);
void on_message(struct mosquitto *, void *, const struct mosquitto_message *);
void on_disconnect(struct mosquitto *, void *, int);
void on_subscribe(struct mosquitto *, void *, int, int, const int *);
void on_unsubscribe(struct mosquitto *, void *, int);
void on_log(struct mosquitto *, void *, int, const char *);

int main()
{
    int ret;

    signal(SIGTERM, termination_handler);
    signal(SIGINT, termination_handler);
    // if(abc < 0){
    //     perror("signal() failed");
    //     _exit(EXIT_FAILURE);
    // }
    printf("Termination signal handler registred\n");

    // always call before calling any mosquitto function
    mosquitto_lib_init();

#ifdef DEBUG
    int major, minor, revision;
    mosquitto_lib_version(&major, &minor, &revision);
    printf("runnig libmosquitto@%d.%d.%d\n", major, minor, revision);
#endif

    client = mosquitto_new(client_id, true, (void *)priv_data);
    if (client == NULL)
    {
        perror("mosquitto_new() failed");
        _exit(EXIT_FAILURE);
    }
    printf("Mosquitto client initialised\n");

    ret = mosquitto_connect_async(client, host, port, 10);
    if (ret < 0)
    {
        switch (ret)
        {
        case MOSQ_ERR_INVAL:
            printf("mosquitto_connect_bind_v5() failed\n Invalid Parameters\n");
            break;
        case MOSQ_ERR_ERRNO:
            perror("mosquitto_connect_bind_v5() failed");
            break;
        case MOSQ_ERR_DUPLICATE_PROPERTY:
            printf("mosquitto_connect_bind_v5() failed\n Duplicate properties\n");
        break;
        case MOSQ_ERR_PROTOCOL:
            printf("mosquitto_connect_bind_v5() failed\n Invalid v5 properties\n");
        break;

        default:
            printf("mosquitto_connect_bind_v5() failed (%d)\n", ret);
            break;
        }
        ret = EXIT_FAILURE;
        goto CLEAN_CLIENT;
    }

    mosquitto_connect_callback_set(client, on_connect);
    mosquitto_message_callback_set(client, on_message);
    mosquitto_disconnect_callback_set(client, on_disconnect);
    mosquitto_subscribe_callback_set(client, on_subscribe);
    mosquitto_unsubscribe_callback_set(client, on_unsubscribe);
    mosquitto_log_callback_set(client, on_log);
    mosquitto_loop_start(client);

    while(1){
        sleep(5);
    }
    // mosquitto_loop_forever(client, 1000, 1);
    printf("Stopped loop");
    CLEAN_CLIENT:
    // free memory associated with client
    mosquitto_destroy(client);

    CLEAN_LIB:
    // free resources associated with library
    mosquitto_lib_cleanup();

    return ret;
}

/**
 * @brief Callback function for connection received event
 * 
 * @param mosq pointer to mosquitto client
 * @param p_data pointer to private data
 * @param return_code return code
 */
void on_connect(struct mosquitto *mosq, void * p_data, int return_code){
        printf("Connected to broker\n");
        if(return_code>0){
        printf("Error returned with code: %d\n",return_code);
        exit(-1);
    }
    printf("Subscribing to topic: %s\n", topic);
    mosquitto_subscribe(mosq,NULL,topic,0);
}

/**
 * @brief Callback for for message received event
 * 
 * @param mosq pointer to mosquitto client
 * @param obj private data object
 * @param msg pointer to mosquitto message
 */
void on_message(struct mosquitto* mosq,void* obj,const struct mosquitto_message *msg){
    printf("Message from topic %s:%s\n",msg->topic,(char *)msg->payload);
    printf("Message id: %d, payload_len: %d\n", msg->mid, msg->payloadlen);
    frame_t *t = (frame_t*) msg->payload;
    printf("Addr: %#x, cmd: %#x, len: %#x\n", t->addr, t->cmd, t->len);
    if(msg->payloadlen > 3){
        printf("Data: %s\n", (char *)(msg->payload + 3));
    }
}

/**
 * @brief Handle program termination signal
 * 
 * @param signal signal number
 */
void termination_handler (int signal){
    printf("Termination signal received\n");

    printf("Disconnecting client\n");
    mosquitto_disconnect(client);

    printf("Stopping loop\n");
    mosquitto_loop_stop(client, 0);

    // free memory associated with client
    printf("Destroying client\n");
    mosquitto_destroy(client);

    // free resources associated with library
    printf("Unloading library\n");
    mosquitto_lib_cleanup();
    _exit(EXIT_FAILURE);
}

void on_disconnect(struct mosquitto *m, void *priv_data, int ret_code){
    printf("Disconnected from broker (%d)\n", ret_code);
    mid = 12;
    if(ret_code != 0){
        mosquitto_reconnect_async(client);
        mosquitto_unsubscribe(m, &mid, topic);
    }
}


void on_subscribe(struct mosquitto *mosq, void *obj, int mid, int qos_count, const int *granted_qos){
    printf("Subscribed to topic: %s, mid=%d\n", topic, mid);
}

void on_unsubscribe(struct mosquitto *mosq, void *obj, int mid){
    printf("Unsubscribed from topic %s, mid=%d\n", topic, mid);
}


void on_log(struct mosquitto *mosq, void *obj, int mid, const char *msg){
    printf("(%d) LOG: %s\n", mid, msg);
}