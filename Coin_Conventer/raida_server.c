#include"raida_server.h"
char execpath[256];
struct server_config server_config_obj;
//----------------------------------------------------------
//Welcome message
//----------------------------------------------------------
void welcomeMsg() {
	printf("\nWelcome to Raida Server\n");
}
//----------------------------------------------------------
//Loads raida no from raida_no.txt
//----------------------------------------------------------
int load_raida_no(){
	FILE *fp_inp = NULL;
	int size=0,ch;
	unsigned char buff[24];
	char path[256];
	strcpy(path,execpath);
	strcat(path,"/Data/raida_no.txt");
	if ((fp_inp = fopen(path, "r")) == NULL) {
		printf("raida_no.txt Cannot be opened , exiting \n");
		return 1;
	}
	while( ( ch = fgetc(fp_inp) ) != EOF ){
		size++;
	}
	fclose(fp_inp);
	fp_inp = fopen(path, "r");
	if(fread(buff, 1, size, fp_inp)<size){
		printf("Configuration parameters missing in raida_no.txt \n");
		return 1;
	}
	if(size == 2){
		server_config_obj.raida_id = (buff[0]-48)*10;
		server_config_obj.raida_id+= (buff[1]-48);
	}
	else {
		server_config_obj.raida_id=buff[0]-48;
	}
	printf("Raida_Id:- %d \n", server_config_obj.raida_id);
	fclose(fp_inp);
	return 0;
}
//----------------------------------------------------------
//Loads server configuation from server.bin
//---------------------------------------------------------
int load_server_config() {
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
	//server_config_obj.raida_id=buff[0];
	server_config_obj.port_number = buff[2];
	server_config_obj.port_number|= (((uint16_t)buff[1])<<8);

	printf("------------------------------\n");
	printf("Server Configuration Details..\n");
	printf("------------------------------\n");
	printf("Port Number :- %d \n", server_config_obj.port_number);
	fclose(fp_inp);
	return 0;
}
//---------------------------------------------------------
// Get the current directory path starting from home dir
//---------------------------------------------------------
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

//----------------------------------------------------------
// main function
//---------------------------------------------------------
int main() {
	uint32_t packet_size,i=0;
	welcomeMsg();
	getexepath();
	load_raida_no();
	if(load_server_config()){
		exit(0);
	}
	init_udp_socket();

	while(1) {
		if ((packet_size=listen_request())>0){
			process_request(packet_size);
		}
	}
	return 0;
}
