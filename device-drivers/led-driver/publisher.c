#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "MQTTClient.h"
#include <string.h>

const char *address = "tcp://localhost:1883";
const char *client_id = "c_listner";
const char *topic_led = "/led";
const char *topic_temperature = "/temp";
const uint8_t qos = 1U;
const uint64_t timeout = 1000L;

int main()
{
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;
    int rc;

    MQTTClient_create(&client, address, client_id, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;

    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("MQTTClient_connect() failed (%d)\n", rc);
        perror("Couldn't connect");
        exit(EXIT_FAILURE);
    }

    pubmsg.payload = (void *)"hello";
    pubmsg.payloadlen = strlen(pubmsg.payload);
    pubmsg.qos = qos;
    pubmsg.retained = 0;
    MQTTClient_publishMessage(client, topic_led, &pubmsg, &token);
    printf("Waiting for up to %d seconds for publication of %s\n"
           "on topic %s for client with ClientID: %s\n",
           (int)(timeout / 1000), pubmsg.payload, topic_led, client_id);
    rc = MQTTClient_waitForCompletion(client, token, timeout);
    printf("Message with delivery token %d delivered\n", token);
    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    return rc;

    return 0;
}