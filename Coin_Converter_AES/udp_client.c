// Client side implementation of UDP client-server model
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "aes.h"

 #define VER 255
#define PORT     18000
//#define PORT     8080
#define MAXLINE 1024
#define ENCRYPTION_CONFIG_BYTES  16
#define NOUNCE_BYTES_CNT         8
#define REQ_NO_1  				9
#define REQ_NO_2  				10
#define REQ_NO_3  				11
#define REQ_NO_4  				12
#define REQ_NO_5  				13

#define REQ_NO_6				19
#define REQ_NO_7				20
#define REQ_NO_8				21
  //2021-07-15 17:15:15
// Driver code

char execpath[256];
uint8_t encrypt_key[ENCRYPTION_CONFIG_BYTES];
uint8_t nounce[NOUNCE_BYTES_CNT];

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
	
int load_encrypt_key(){

	FILE *fp_inp = NULL;
	int i = 0;
	unsigned char buff[ENCRYPTION_CONFIG_BYTES];
	char path[256];
	strcpy(path,execpath);
	strcat(path,"/Data/encryption_key.bin");
	printf("------------------------------\n");
	printf("ENCRYPTION CONFIG KEY ..\n");
	printf("------------------------------\n");
	if ((fp_inp = fopen(path, "rb")) == NULL) {
		printf("encryption_key.bin.bin Cannot be opened , exiting \n");
		return 1;
	}
	if(fread(buff, 1, ENCRYPTION_CONFIG_BYTES, fp_inp)<(ENCRYPTION_CONFIG_BYTES)){
		printf("Configuration parameters missing in encryption_key.bin \n");
		return 1;
	}
	memcpy(encrypt_key,buff,ENCRYPTION_CONFIG_BYTES);
	for(i=0;i<ENCRYPTION_CONFIG_BYTES;i++){
		printf("%02x  ",encrypt_key[i]);
	}
	printf("\n");
	fclose(fp_inp);

	memset(nounce,0,NOUNCE_BYTES_CNT);
	//We take nouce 5 bytes
	for(int i=0;i < 5;i++){
		nounce[i] = udp_buffer[REQ_NO_1+i];
	}
	int j=0;
	//We take nouce 3 bytes 
	for(int i=5; i < 8;i++){
		nounce[i] = udp_buffer[REQ_NO_6+j];
		j++;
	}
	for(int i = 8;i < ENCRYPTION_CONFIG_BYTES; i++) {
		nounce[i] = 0;
	}
	return 0;
}

int main() {
    int sockfd,i=0;
    unsigned char buffer[MAXLINE], recv_buffer[MAXLINE];	
    unsigned char buffer_version[MAXLINE]={0,0,25,0,0,15,2,0,0,0,0,0,22,22,0,0,1,0x3E,0x3E};
    unsigned char buffer_echo[MAXLINE]={0,0,2,0,0,4,2,0,0,0,0,0,22,22,0,0,1,0x3E,0x3E};
    unsigned char buffer_news[MAXLINE]={0,0,25,0,0,16,2,0,0,0,0,0,22,22,0,0,1,0x3E,0x3E};
    unsigned char buffer_logs[MAXLINE]={0,0,25,0,0,17,2,0,0,0,0,0,22,22,0,0,1,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,0,0,1,158,16,109,224,249,206,73,23,179,48,157,247,21,112,249,44,0x7,0xE5,7,15,18,00,00,00,00,04,0x3E,0x3E};
    unsigned char buffer_pown[MAXLINE]={0,0,0,0,0,0,VER,0,0,0,0,0,22,22,0,0,1,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,
										  0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
										  51,158,16,109,224,249,206,73,23,179,48,157,247,21,112,249,	
										  62,62};
    unsigned char buffer_validate[MAXLINE]={0,0,1,0,0,5,2,0,0,0,0,0,22,22,0,0,1,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,1,23,196,112,1,0x3E,0x3E};
    unsigned char buffer_postkey[MAXLINE]={0,0,1,0,0,6,2,0,0,0,0,0,22,22,50,0,1,1,2,3,4,5,6,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,0x3E,0x3E};
    unsigned char buffer_getkey[MAXLINE]=  {0,0,1,0,0,7,2,0,0,0,0,0,22,22,50,0,1,1,2,3,4,5,6,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,0x3E,0x3E};
    unsigned char buffer_deposite[MAXLINE]={0,0,0,0,0,100,2,0,0,0,0,0,22,22,0,0,1,//17
											1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,//16
											0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//19
											0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//19
											0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//19
											0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//19
											0,0,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//19
											0,0,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//19
											0,0,12,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//19
											0,0,13,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//19
											0,0,14,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//19
											0,0,15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//19
											0,0,16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//19
											0,0,17,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//19
											0,0,18,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//19
											0,0,19,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//19
											0,0,20,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//19
											0,0,21,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//19
											0,0,22,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//19
											0,0,23,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//19
											0,0,24,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//19
											0,0,25,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//19
											0,0,26,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//19
											0,0,27,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//19
											0,0,28,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//19
											0,0,29,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//19
											0,0,30,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//19
											//0,0,31,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//19
											01,0x32,0x0F,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,
											21,11,2,14,50,0,4,0,//27
											0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//50
											62,62};//2
 unsigned char buffer_deposite_dev[MAXLINE]={0,0,0,0,0,100,2,0,0,0,0,0,22,22,0,0,1,//17
											1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,//16
											0,0,101,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
											0,0,102,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
											0,0,1,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,29,9,2,12,5,0,4,0,//27
											0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//50
											62,62};//2

unsigned char buffer_balance[MAXLINE]={0,0,0,0,0,110,2,0,0,0,0,0,22,22,0,0,1,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,
											0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
											//0x1,0x50, 0x15 ,25, 225, 218, 171, 39, 22, 246, 181, 140, 205, 223, 172, 154, 132, 105, 42, 
											62,62};
 unsigned char buffer_deposite1[MAXLINE]={0,0,0,0,0,100,2,0,0,0,0,0,22,22,0,0,1,1,2,3,4,5,6,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,
											0,0,22,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
											0,0,22,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
											0,0,91,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
											0,0,16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
											0,0,16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
											0,0,2,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,29,9,2,12,4,0,0,0,
											0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
											62,62};
  
  unsigned char buffer_transfer[MAXLINE]={0,0,1,0,0,104,2,0,0,0,0,0,22,22,0,0,1,206,73,23,179,48,157,247,21,112,249,142,16,109,224,249,206,
											0,0,28,48,157,247,21,112,249,44,158,16,109,224,249,206,73,23,179,
											0,0,22,0,0,22,
											0,0,29,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,21,8,2,12,4,0,0,5,
											0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
											0x3E,0x3E};
 unsigned char buffer_break[MAXLINE]={0,0,0,0,0,122,2,0,0,0,0,0,22,22,0,0,1,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,
										0,0,32,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 
										0,0,1,
										0,0,16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 
										0,0,17,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
										0,0,22,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
										0,0,23,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
										0x3E,0x3E};
unsigned char buffer_break_in_bank[MAXLINE]={0,0,0,0,0,123,2,0,0,0,0,0,22,22,0,0,1,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,
										0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
										0,0,32,
										0,0,2,
										0,0,16,0,0,17,0,0,22,0,0,23,
										0x3E,0x3E};


unsigned char buffer_show_statement1[MAXLINE]={0,0,0,0,0,130,2,0,0,0,0,0,22,22,0,0,1,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,
										0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
										0,21,11,2,0,
										0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
										0x3E,0x3E};

 unsigned char buffer_show_statement2[MAXLINE]={0,0,1,0,0,130,2,0,0,0,0,0,22,22,0,0,1,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,
										0,0,29,48,157,247,21,112,249,142,16,109,224,249,206,73,23,179,48,
										5,21,8,2,0,
										0x3E,0x3E};

unsigned char buffer_show_change[MAXLINE]={0,0,0,0,0,116,2,0,0,0,0,0,22,22,0,0,1,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,
											0,0,1,250,
											62,62};

unsigned char buffer_detect[MAXLINE]={0,0,0,0,0,1,0,0,0,0,0,0,255,255,0,0,1,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,
							0,0,105,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,106,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,62,62,};

unsigned char buffer_fix[MAXLINE]={0,0,0,0,0,3,0,0,0,0,0,0,255,255,0,0,1,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,
							0,0,100,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,101,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							1,1,1,1,
							80,179,173,117,
							13,218,2,92,
							1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
							1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
							62,62,};
unsigned char buffer_fix1[MAXLINE]={0,0,0,0,0,3,0,0,0,0,0,0,255,255,0,0,1,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,
							0,0,100,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,101,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							1,1,1,1,
							0,0,0,0,
							0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,0,0,
							62,62,};

unsigned char buffer_free_id[1024]={0,0,0,0,0,30,2,0,0,0,0,0,22,22,0,0,1,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,
						  0,0,1,62,62};
 unsigned char buffer_join[MAXLINE]={0,0,0,0,0,120,2,0,0,0,0,0,22,22,0,0,1,//17
											1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,//16
											0,0,2,0,0,22,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
											0,0,16,89, 26, 136, 93, 91, 253, 205, 134, 118, 232, 129, 2, 137, 160, 32, 86,//19
											0,0,17,84, 219, 13, 192, 209, 27, 1, 11, 248, 133, 141, 217, 32, 6, 169, 247,//19
											0,0,18,158, 50, 85, 248, 174, 34, 253, 36, 10, 127, 164, 19, 31, 196, 104, 241,//19
											0,0,19,31, 116, 50, 239, 14, 177, 122, 6, 54, 7, 222, 212, 13, 7, 204, 170,//19
											62,62};//2
unsigned char buffer_join_in_bank[MAXLINE]={0,0,0,0,0,121,2,0,0,0,0,0,22,22,0,0,1,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,
										0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
										0,0,22,
										0,0,2,
										0,0,16,0,0,17,0,0,18,0,0,19,
										0x3E,0x3E};
unsigned char buffer_show_coins_by_deno[MAXLINE]={0,0,0,0,0,114,2,0,0,0,0,0,22,22,0,0,1,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,
										0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
										250,1,
										0x3E,0x3E};
unsigned char buffer_show_coins_by_type[MAXLINE]={0,0,0,0,0,115,2,0,0,0,0,0,22,22,0,0,1,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,
										0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
										5,
										0,
										0x3E,0x3E};
unsigned char buffer_change_coin_type[MAXLINE]={0,0,0,0,0,135,2,0,0,0,0,0,22,22,0,0,1,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,
										0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
										5,
										0x3E,0x3E};
unsigned char buffer_withdraw[MAXLINE]={0,0,0,0,0,104,2,0,0,0,0,0,22,22,0,0,1,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,
										0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
										0,0,16,0,0,17,0,0,22,0,0,23,
										1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,
										1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,29,9,2,12,4,0,0,
										0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
										0x3E,0x3E};
 unsigned char buffer_primary[MAXLINE]={0,0,0,0,0,42,2,0,0,0,0,0,22,22,0,0,1,0x3E,0x3E};
 unsigned char buffer_mirror[MAXLINE]={0,0,0,0,0,43,2,0,0,0,0,0,22,22,0,0,1,0x3E,0x3E};
 unsigned char buffer_check_updates[MAXLINE]={0,0,0,0,0,44,2,0,0,0,0,0,22,22,0,0,1,0x3E,0x3E};
/*
 unsigned char buffer_upgrade_coin[MAXLINE] = {0,0,2,0,0,215,2,0,0,0,0,0,22,22,0,1,0,0,0,0,0,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,
										0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
										0x3E,0x3E};
*/
unsigned char buffer_upgrade_coin[MAXLINE] = {0,0,2,0,0,215,0,0,0,0,0,0,22,22,0,1,0,0,0,0,0,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,
										121,235,255,24,91,151,152,78,82,115,61,216,134,106,169,152,01,62,143,230,250,37,
										0x3E, 0x3E};


//25 fa  e6  8f  3e 01 98   a9  6a 86  d8  3d 73  52 4e 98   97 5b 18 ff  eb  79
//37 250 230 143 62 01 152 169 106 134 216 61 115 82 78 152 151 91 24 255 235 121

//0,0,2,0,0,215,0,0,0,0,0,0,22,22,0,1,0,0,0,0,0,0,155,102,254,83,80,231,201,183,118,54,232,99,121,39,162,129,
//195,152,52,31,134,73,205,131,241,174,235,124,153,224,16,234,92,87,162,153,226,38,67,29

	    struct sockaddr_in     servaddr;
	    /*char md5[64],result[64];	
	  
		FILE *fp;
		int status;
		char path[64];
                fp = popen("echo abcd |md5sum", "r");
		while (fgets(path,64, fp) != NULL)
		    printf("%s", path);
	    return 0;	*/
			
	//   memcpy(buffer,buffer_version,MAXLINE);	
	//   memcpy(buffer,buffer_news,MAXLINE);	
	//   memcpy(buffer,buffer_logs,MAXLINE);	
	//   memcpy(buffer,buffer_pown,MAXLINE);	
	//	memcpy(buffer,buffer_validate,MAXLINE);	
	//	memcpy(buffer,buffer_postkey,MAXLINE);	
	//	memcpy(buffer,buffer_getkey,MAXLINE);	
	//	memcpy(buffer,buffer_deposite,MAXLINE);	
	//	memcpy(buffer,buffer_deposite_dev,MAXLINE);	
	//	memcpy(buffer,buffer_balance,MAXLINE);	
	//	memcpy(buffer,buffer_transfer,MAXLINE);	
	//	memcpy(buffer,buffer_break,MAXLINE);	
	//	memcpy(buffer,buffer_show_statement1,MAXLINE);	
	//	memcpy(buffer,buffer_show_statement2,MAXLINE);	
	//	memcpy(buffer,buffer_change_coin_type,MAXLINE);	
	//	memcpy(buffer,buffer_echo,MAXLINE);	
	//	memcpy(buffer,buffer_detect,MAXLINE);	
	//	memcpy(buffer,buffer_fix,MAXLINE);	
	//	memcpy(buffer,buffer1,MAXLINE);	
	//	memcpy(buffer,buffer_show_change,MAXLINE);	
	//	memcpy(buffer,buffer_break,MAXLINE);	
	//	memcpy(buffer,buffer_break_in_bank,MAXLINE);	
	//	memcpy(buffer,buffer_free_id,MAXLINE);	
	//	memcpy(buffer,buffer_join,MAXLINE);	
	//	memcpy(buffer,buffer_join_in_bank,MAXLINE);	
	//	memcpy(buffer,buffer_fix1,MAXLINE);	
	//	memcpy(buffer,buffer_show_coins_by_deno,MAXLINE);	
	//	memcpy(buffer,buffer_show_coins_by_type,MAXLINE);	
	//	memcpy(buffer,buffer_withdraw,MAXLINE);	
	//	memcpy(buffer,buffer_primary,MAXLINE);	
	//	memcpy(buffer,buffer_mirror,MAXLINE);	
	//	memcpy(buffer,buffer_check_updates,MAXLINE);
		memcpy(buffer, buffer_upgrade_coin, MAXLINE);

	    // Creating socket file descriptor
	    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
	        perror("socket creation failed");
	        exit(EXIT_FAILURE);
	    }
		else {
			printf("Socket Creation Successful\n");
		}
	  
	    memset(&servaddr, 0, sizeof(servaddr));
	    // Filling server information
	    servaddr.sin_family = AF_INET;
	    servaddr.sin_port = htons(PORT);
//	    servaddr.sin_addr.s_addr = INADDR_ANY;
	   servaddr.sin_addr.s_addr = inet_addr("139.99.155.124");

	      
	    int n, len=0;
	     for(i=0;i<MAXLINE;i++){
		len++;
		//printf("%d,",buffer[i]);
		if(buffer[i]==62 && buffer[i-1]==62)
			break;	
	    }
	   printf("Len = %d %d\n", len,i);
//----------------------------------------------------------
//Loads encrypt key from encryption_key.bin
//--------------------------------------------------------- 
	getexepath();
	int send_req = len - 22;
	unsigned char *req_ptr = &buffer[22];
	unsigned char *key = &encrypt_key[0];
	unsigned char *iv = &nounce[0];

	int status  = load_encrypt_key();
	crypt_ctr(key,req_ptr,send_req,iv);

//-----------------------------------------------------------------------

	    sendto(sockfd, (const char *)buffer, len,
	        MSG_CONFIRM, (const struct sockaddr *) &servaddr, 
	            sizeof(servaddr));
	    n = recvfrom(sockfd, (char *)recv_buffer, MAXLINE, 
	                MSG_WAITALL, (struct sockaddr *) &servaddr,
	                &len);
		printf("n: %d\n", n);

	   for(i=0;i<n;i++){	
	     printf("%d,", recv_buffer[i]);
	   }
	   printf("\n");
	   close(sockfd);
  	  return 0;
}