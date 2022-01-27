#include"raida_server.h"
char execpath[256];
struct server_config server_config_obj;
union bytes binary;
FILE *fd_log;
//----------------------------------------------------------
//Welcome message
//----------------------------------------------------------
void welcomeMsg() {
	printf("\nWelcome to Raida Server\n");
	fprintf(fd_log, "\nWelcome to Raida Server\n");
}
//----------------------------------------------------------
//Loads raida no from raida_no.txt
//----------------------------------------------------------
int load_raida_no(){
	FILE *fp_inp=NULL;
	int size=0,ch;
	unsigned char buff[24];
	char path[256];
	strcpy(path,execpath);
	strcat(path,"/Data/raida_no.txt");
	if ((fp_inp = fopen(path, "r")) == NULL) {
		fprintf(fd_log, "raida.txt file cannot be read. %s\n", strerror(errno));
		printf("raida_no.txt Cannot be opened , exiting \n");
		return 1;
	}
	while( ( ch = fgetc(fp_inp) ) != EOF ){
		size++;
	}
	fclose(fp_inp);
	fp_inp = fopen(path, "r");
	if(fread(buff, 1, size, fp_inp)<size){
		fprintf(fd_log, "Configuration parameters missing in raida_no.txt. %s\n", strerror(errno));
		printf("Configuration parameters missing in raida_no.txt \n");
		return 1;
	}
	if(size >= 2){
		server_config_obj.raida_id = (buff[0]-48)*10;   
		server_config_obj.raida_id+= (buff[1]-48);  
	}else{
		server_config_obj.raida_id=buff[0]-48;
	}

	printf("Raida Id :- %d \n", server_config_obj.raida_id);
	fprintf(fd_log, "RAIDA_NO.: %d\n", server_config_obj.raida_id);
	fclose(fp_inp);
	return 0;
}
//----------------------------------------------------------
//Loads server configuation from server.bin
//----------------------------------------------------------
int load_server_config() {
	FILE *fp_inp = NULL;
	int cnt=0;
	unsigned char buff[SERVER_CONFIG_BYTES];
	char path[256];
	strcpy(path,execpath);
	strcat(path,"/Data/server.bin");
	if ((fp_inp = fopen(path, "rb")) == NULL) {
		fprintf(fd_log, "server.bin Cannot be opened. %s\n", strerror(errno));
		printf("server.bin Cannot be opened , exiting \n");
		return 1;
	}
	if(fread(buff, 1, SERVER_CONFIG_BYTES, fp_inp)<SERVER_CONFIG_BYTES){
		fprintf(fd_log, "Configuration parameters missing in server.bin. %s\n", strerror(errno));
		printf("Configuration parameters missing in server.bin \n");
		return 1;
	}
	/*
	server_config_obj.port_number = buff[1];
	server_config_obj.port_number|= (((uint16_t)buff[0])<<8);
	//server_config_obj.port_number = 18000;
	
	server_config_obj.bytes_per_frame = buff[3];
	server_config_obj.bytes_per_frame |= (((uint16_t)buff[2])<<8);
	//server_config_obj.bytes_per_frame = 1024;
	*/

	binary.byte[0] = buff[0];
    binary.byte[1] = buff[1];
    server_config_obj.port_number = binary.val;
    binary.byte[0] = buff[2];
    binary.byte[1] = buff[3];
    server_config_obj.bytes_per_frame = binary.val;

	printf("------------------------------\n");
	printf("Server Configuration Details..\n");
	printf("------------------------------\n");
	printf("Port Number :- %d \n", server_config_obj.port_number);
	printf("Bytes per UDP Request body :- %d \n",server_config_obj.bytes_per_frame);
	fprintf(fd_log, "Port Number: %d  Bytes per UDP Request body: %d\n", server_config_obj.port_number, server_config_obj.bytes_per_frame);
	fclose(fp_inp);
	return 0;
}
//----------------------------------------------------------
// Returns time in centi seconds
//----------------------------------------------------------
long get_time_cs()
{
    long            ms,cs; // Milliseconds
    time_t          s;  // Seconds
    struct timespec spec;
    clock_gettime(CLOCK_REALTIME, &spec);
    s  = spec.tv_sec;
    //ms = round(spec.tv_nsec / 1.0e3); // Convert nanoseconds to milliseconds
    //cs = ms /100;	
//    printf("Current time: %"PRIdMAX".%03ld seconds since the Epoch\n",(intmax_t)s, ms);
    return ms;	
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
//--------------------------------------------------------
//LOG ERROR FILE  converter_logs.txt
//--------------------------------------------------------
int open_logsfile() {

	fd_log = NULL;
	char path[256];
	strcpy(path,execpath);
	strcat(path,"/Data/converter_logs.txt");
	if ((fd_log = fopen(path, "a")) == NULL) {
		printf("Logs file cannot be opened.\n");
		perror("Error: ");
		return 0;
	}

	return 1;
}
//----------------------------------------------------------
// main function
//---------------------------------------------------------
int main() {
	uint32_t packet_size,i=0;	
	open_logsfile();
	welcomeMsg();
	getexepath();
	if(load_raida_no() || load_server_config()){
		exit(0);
	}

	//fprintf(fd_log, "%s\n", strerror(errno));
	srand(time(NULL));
	init_udp_socket();
	while(1) {
		if ((packet_size=listen_request())>0){
			process_request(packet_size);
		}
	}
	return 0;
}
