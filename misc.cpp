#include <iostream>
#include <cstdlib>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#include "misc_sensor.hpp"

#define BUFFSIZE 2048

void data_handler(char * data, size_t length){
    uint8_t snr, wlan_type, wlan_subtype;
    uint32_t rmac, dmac, tmac;
    char * ssid, *token = NULL;
    size_t index = 0;

    printf("{ ");

    token = strtok(data,",");
    while(token != NULL){
        printf(" ");
        // frame.time_epoch,radiotap.dbm_antsignal,wlan.fc.type,wlan.fc.subtype,wlan.ra,wlan.da,wlan.ta,wlan_mgt.ssid
        switch(index++){
            case 0:
                printf("\"time\"");
                break;
            case 1:
                printf("\"snr\"");
                break;
            case 2:
                printf("\"type\"");
                break;
            case 3:
                printf("\"subtype\"");
                break;
            case 4:
                printf("\"ra\"");
                break;
            case 5:
                printf("\"da\"");
                break;
            case 6:
                printf("\"ta\"");
                break;
            case 7:
                printf("\"ssid\",");
                break;
        }
        printf(":\"%s\", ",token);

        token = strtok(NULL,",");
    }
    printf("}\n");
       
}

char * build_wireshark_string(){
    char base_command[] = "tshark -i wlan0 -I -Eheader=y -Eseparator=,  -Tfields ";
    char header[] = "frame.time_epoch,radiotap.dbm_antsignal,wlan.fc.type,wlan.fc.subtype,wlan.ra,wlan.da,wlan.ta,wlan_mgt.ssid";
    char command_buffer[2048];
    char * token; 
    size_t command_len;
    
    printf("Header: %s\n",header);
    printf("base_command: %s\n",base_command);

    token = strtok(header,",");
    printf("First Token: %s\n", token);

    strcpy(command_buffer, base_command);
    printf("Got %s\n", header);
    while(token != NULL){
        printf("Token: %s\n",token);
        command_len = strlen(command_buffer);
        if(command_len > 1){
            strcat(command_buffer, " -e");
            strcat(command_buffer, token);  
 //            sprintf(command_buffer + command_len, "-e%s ", token);
        }  
        token = strtok(NULL, ",");
    }

    printf("Command: %s\n",command_buffer);
    return command_buffer;
}

//frame.time_epoch,radiotap.dbm_antsignal,wlan.fc.type,wlan.fc.subtype,wlan.ra,wlan.da,wlan.ta,wlan_mgt.ssid
int main(void)
{
	char * command = build_wireshark_string();
	misc_res misc(command); 
	uint8_t * newdata;
    size_t rcv_sz = 0;
	
    newdata =(uint8_t*) malloc(2048);

    /* 
	std::cout << "Starting MISC thread" << std::endl;
	misc.thread_misc();
    */

    for (;;) {
        rcv_sz = misc.get_misc_data(newdata, 2048);
        //rcv_sz = misc.readline((void **)&newdata);
        if(newdata == NULL || rcv_sz <= 0)
            continue;

        newdata[rcv_sz] = '\0'; 
        if(rcv_sz > 0){
            //printf("%s(%zu): %s\n", __func__, rcv_sz, newdata);
            data_handler((char *)newdata, rcv_sz);
        }
        usleep(20000);        
    }

	std::cout << "Exiting." << std::endl;
    return 0;
}
