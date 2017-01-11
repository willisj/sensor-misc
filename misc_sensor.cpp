#include <iostream>
#include <cstdlib>
#include <string.h>

#include <cstdlib>
#include <pthread.h>

#include "misc_sensor.hpp"

misc_res::misc_res(char * command){
    
	if (command == NULL) {
		std::cerr << "No command specified.\n";
		exit(1);
	}

    this->newdata.bytes = (uint8_t *) malloc(2048);
    this->newdata.buffer_size = 2048;
    this->newdata.stored_size = 0;

    this->command = command;
    this->data_changed = false;
    
    printf("======== Running COMMAND ========\n%s\n=================================\n", command);
    this->process_fp = popen(command, "r");
}

/*
#define FUNCDBG_MISCRES_POLLRES 
bool misc_res::poll_misc(){
    static char * line = (char *)malloc(2048); //todo: freethis
    size_t recv_size = 2048; 
    bool retval = true;

	this->newdata_mtex.lock();
    recv_size = this->newdata.buffer_size - this->newdata.stored_size;
	this->newdata_mtex.unlock();

    if( recv_size <= 0){
#ifdef FUNCDBG_MISCRES_POLLRES
        printf("%s: Insufficent space in internal buffer.\n", __func__);
#endif
        return true;
    }

    getline(&line, &recv_size, this->process_fp);
    
#ifdef FUNCDBG_MISCRES_POLLRES
    printf("%s: Got %d bytes from sensor\n", __func__, recv_size);
#endif

	// copy the data
	this->newdata_mtex.lock();
    if( this->newdata.buffer_size < this->newdata.stored_size + recv_size){
#ifdef FUNCDBG_MISCRES_POLLRES
        fprintf( stderr, "%s: Not enough room in buffer\n", __func__);
#endif
    }
    else{
        if(memcpy(this->newdata.bytes + this->newdata.stored_size, line, recv_size)){

#ifdef FUNCDBG_MISCRES_POLLRES
            this->newdata.bytes[recv_size - 1] = '\0';
            printf("%s: copied %zu bytes\n", __func__, recv_size);
            printf("%s: %s", __func__, this->newdata.bytes);
#endif
            this->newdata.stored_size += recv_size;
            this->data_changed = true;
        }
        else{
            fprintf(stderr, "%s: memcpy failed\n", __func__);
            retval = false;
        }
    }
	this->newdata_mtex.unlock();
	
	return retval;
}
*/

//#define FUNCDBG_MISCRES_GETMISCDATA
size_t misc_res::get_misc_data(uint8_t *dest, size_t dest_size){
    size_t cp_sz = 0;

    if (!this->data_changed)
        return false;

	this->newdata_mtex.lock();
    
    if( dest_size >= this->newdata.stored_size )
        cp_sz = this->newdata.stored_size;
    else
        cp_sz = dest_size;

	if(memcpy(dest, this->newdata.bytes, cp_sz) != NULL){
#ifdef FUNCDBG_MISCRES_GETMISCDATA
        printf("get_misc_data: Copied %zu bytes\n", cp_sz);
        printf("get_misc_data: %s", this->newdata.bytes);
#endif
        if(cp_sz < this->newdata.stored_size){                                                              // dest < src
            memmove(this->newdata.bytes, this->newdata.bytes + cp_sz, this->newdata.stored_size - cp_sz);   // shift thebuffer forward
            this->data_changed = true;                                                                      // more data
        }
        else
            this->data_changed = false;
        this->newdata.stored_size -= cp_sz;  // the remaining portion
    }
    else{
#ifdef FUNCDBG_MISCRES_GETMISCDATA
        printf("%s: get_misc_data: memcpy failed\n", __func__);
#endif
    }

	this->newdata_mtex.unlock();

    return cp_sz;
}
/*
static void* misc_loop( void *data){

    misc_res *sensor_handler  = (misc_res *) data;
    sensor_handler->process_fp = popen(sensor_handler->command, "r");

    while(sensor_handler->process_fp != NULL) {
        if (!sensor_handler->poll_misc()) {
            std::cerr << "Read error.\n";
            continue;
        }
        usleep(10000);        
    }

    return NULL;
}
*/

size_t misc_res::readline(void **line_t,size_t  recv_size){
    static char * line = NULL;//(char *)malloc(2048); //todo: freethis
        recv_size =0;
    //if(line != NULL){
        getline(&line, &recv_size, this->process_fp);
        *line_t = line;
        printf("line: %s\n", (char *) *line);
        return recv_size;
    /*
    }
    else{
        fprintf(stderr, "%s:%d: Malloc failed\n", __func__, __LINE__);
        return 0
    }
*/
}

/*
void misc_res::thread_misc(){

	this->misc_thread = new pthread_t;
	pthread_create(this->misc_thread, NULL, misc_loop, (void *)this);
}
*/

