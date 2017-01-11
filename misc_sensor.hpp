
#ifndef MISC_SENSOR_HPP
#define MISC_SENSOR_HPP

#include <iostream>
#include <cstdlib>
#include <string.h>

#include <pthread.h>
#include <mutex>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

struct misc_data_t{
    size_t buffer_size;
    size_t stored_size;
    uint8_t * bytes; 
};

class misc_res{
	private:
		struct misc_data_t newdata;
		pthread_t *misc_thread;
		std::mutex newdata_mtex;
        bool data_changed;
	public:
        FILE * process_fp;
        char * command;
		misc_res(char * command);
		// bool poll_misc();
		// void thread_misc();
		size_t get_misc_data(uint8_t* dest, size_t dest_size);
        size_t readline(void **line_t,size_t  recv_size);

};


#endif
