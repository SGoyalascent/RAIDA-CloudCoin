#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdint.h>
#include <time.h>
#include <math.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define SERVER_CONFIG_BYTES    4

char execpath[256];	
unsigned int port_number;
unsigned int bytes_per_frame;

union bytes {
    unsigned int val;
    unsigned char byte[2];
};

union bytes binary;

void getexepath()
{
  char buff[256];
  int count = readlink( "/proc/self/exe", buff, 256);
  int i=0,slash_pos;
  while(buff[i]!='\0'){
	if(buff[i]=='/'){
		slash_pos = i;
	}
	i++;
  }	
  strncpy(execpath,buff,slash_pos);
}

void write_server_config() {

    unsigned char buf[SERVER_CONFIG_BYTES];
    unsigned int port_number = 18000;
    binary.val = port_number;
    buf[0] = binary.byte[0];
    buf[1] = binary.byte[1];
    unsigned int bytes_per_frame = 1024;
    binary.val = bytes_per_frame;
    buf[2] = binary.byte[0];
    buf[3] = binary.byte[1];

    FILE *fp_out;
    char path[256];
	strcpy(path,execpath);
	strcat(path,"/Data/server.bin");
    fp_out = fopen(path, "wb");
    if(fp_out == NULL) {
        printf("server.bin Cannot be opened , exiting \n");
		return;
    }

    fwrite(&port_number, 2, 1,fp_out);
    fwrite(&bytes_per_frame, 2, 1,fp_out);
    //fwrite(buf, 2, 1,fp_out);

    fclose(fp_out);
    
    

}

int load_server_config() {
	
    
    FILE *fp_inp;
    char path[256];
	strcpy(path,execpath);
	strcat(path,"/Data/server.bin");
    unsigned char buf[SERVER_CONFIG_BYTES];
    unsigned int port_number;
    unsigned int bytes_per_frame;
    fp_inp = fopen(path, "rb");
    if(fp_inp == NULL) {
        printf("server.bin Cannot be opened , exiting \n");
		return 1;
    }

    fread(buf, 2, 2, fp_inp); 

    binary.byte[0] = buf[0];
    binary.byte[1] = buf[1];
    port_number = binary.val;
    binary.byte[0] = buf[2];
    binary.byte[1] = buf[3];
    bytes_per_frame = binary.val;

    printf("port_no.: %d  bytes_per_frame: %d\n",port_number, bytes_per_frame );

    fclose(fp_inp);
    
    
    /*
    
    FILE *fp_inp = NULL;
	int cnt=0;
	unsigned char buff[SERVER_CONFIG_BYTES];
	char path[256];
	strcpy(path,execpath);
	strcat(path,"/Data/server.bin");
	if ((fp_inp = fopen(path, "rb")) == NULL) {
		printf("server.bin Cannot be opened , exiting \n");
		return 1;
	}

	if(fread(buff, 1, SERVER_CONFIG_BYTES, fp_inp)<SERVER_CONFIG_BYTES){
		printf("Configuration parameters missing in server.bin \n");
		return 1;
	}
	port_number = buff[1];
	port_number|= (((uint16_t)buff[0])<<8);
	
	bytes_per_frame = buff[3];
	bytes_per_frame |= (((uint16_t)buff[2])<<8);
	
	printf("Port Number :- %d \n", port_number);
	printf("Bytes per UDP Request body :- %d \n", bytes_per_frame);
	fclose(fp_inp);*/
	return 0; 
} 


int main() {
    
    getexepath();
    write_server_config();
    int x = load_server_config();

    
}