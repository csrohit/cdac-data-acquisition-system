#include<stdio.h>
#include<stdlib.h>
#include<mosquitto.h>
#include<mqtt_protocol.h>

void on_connect(struct mosquitto* mosq,void *obj,int return_code){
    printf("ID: %d\n",*(int *)obj);
    if(return_code>0){
        printf("Error returned with code: %d\n",return_code);
        exit(-1);
    }
    mosquitto_subscribe(mosq,NULL,"adc_val",0);
}
void on_message(struct mosquitto* mosq,void* obj,const struct mosquitto_message *msg){
    printf("Message from topic %s:%s\n",msg->topic,(char *)msg->payload);

}

int main(){
    int return_code, id=12;
    mosquitto_lib_init();
    struct mosquitto *mosq;
    mosq = mosquitto_new("subscribe-adc_value",true,&id);
    mosquitto_connect_callback_set(mosq,on_connect);
    mosquitto_message_callback_set(mosq,on_message);

    return_code = mosquitto_connect(mosq,"localhost",8266,60);
    if(return_code>0){
        printf("broker connection failed with code: %d\n",return_code);
        return -1;
    }
    mosquitto_loop_forever(mosq,-1,1);
    return 0;
}