#include<stdio.h>
#include<stdlib.h>
#include<mosquitto.h>

void send_message(struct mosquitto *f_mosq,const char *choice);

int main(int argc,char *argv[]){
    int return_code,opt=0,id=13;
    const char *on ="1";
    const char *off ="0";
    const char *temp;
    struct mosquitto *mosq;
    do{
        printf("Enter light state:");
        scanf("%d",&opt);
        if(opt==0){
            send_message(mosq,off);
        }else if(opt==1){
            send_message(mosq,on);
        }
    }while(((opt==0)||(opt==1)));
    
    return 0;
}

void send_message(struct mosquitto *f_mosq,const char *choice){
    int return_code;
    f_mosq =mosquitto_new("publisher-light",true,NULL);
    return_code= mosquitto_connect(f_mosq,"localhost",8266,60);
    if(return_code!=MOSQ_ERR_SUCCESS){
        printf("mosquitto_connect() returned with error code: %d\n",return_code);
        mosquitto_destroy(f_mosq);
        exit(1);
    }
    printf("connected to broker: %d\n",return_code);
    return_code=mosquitto_publish(f_mosq,NULL,"light",2,choice,0,false);
    printf("rc=%d",return_code);
    mosquitto_disconnect(f_mosq);
    mosquitto_destroy(f_mosq);
    mosquitto_lib_cleanup();
}