#include "udp_socket.h"
int sockfd;
fd_set select_fds;               
struct timeval timeout;
union coversion snObj;
struct sockaddr_in servaddr, cliaddr;
long time_stamp_before,time_stamp_after;
unsigned char udp_buffer[UDP_BUFF_SIZE],response[RESPONSE_HEADER_MAX],coin_table_id[5],EN_CODES[EN_CODES_MAX]={0};
unsigned char free_thread_running_flg;
//-----------------------------------------------------------
//Set time out for UDP frames
//-----------------------------------------------------------
void set_time_out(unsigned char secs){     
	FD_ZERO(&select_fds);            
	FD_SET(sockfd, &select_fds);           	                                  
	timeout.tv_sec = secs; 
	timeout.tv_usec = 0;
}
//-----------------------------------------------------------
//Initialize UDP Socket and bind to the port
//-----------------------------------------------------------
int init_udp_socket() {
	// Creating socket file descriptor
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}
	memset(&servaddr, 0, sizeof(servaddr));
	memset(&cliaddr, 0, sizeof(cliaddr));
	// Filling server information
	servaddr.sin_family = AF_INET; // IPv4
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(server_config_obj.port_number);
	// Bind the socket with the server address
	if ( bind(sockfd, (const struct sockaddr *)&servaddr,sizeof(servaddr)) < 0 ){
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
}
//-----------------------------------------------------------
// receives the UDP packet from the client
//-----------------------------------------------------------
int listen_request(){
	unsigned char *buffer,state=STATE_WAIT_START,status_code;
	uint16_t frames_expected=0,curr_frame_no=0,n=0,i,index=0;
	uint32_t	 client_s_addr=0; 	
	socklen_t len=sizeof(struct sockaddr_in);
	buffer = (unsigned char *) malloc(server_config_obj.bytes_per_frame);
	while(1){
		switch(state){
			case STATE_WAIT_START:
				printf("---------------------WAITING FOR REQ HEADER ----------------------\n");
				index=0;
				curr_frame_no=0;
				client_s_addr = 0;	
				memset(buffer,0,server_config_obj.bytes_per_frame);
				n = recvfrom(sockfd, (unsigned char *)buffer, server_config_obj.bytes_per_frame,MSG_WAITALL, ( struct sockaddr *) &cliaddr,&len);
				curr_frame_no=1;
				printf("--------RECVD  FRAME NO ------ %d\n", curr_frame_no);
				state = STATE_START_RECVD;	
			break;		
			case STATE_START_RECVD:
				printf("---------------------REQ HEADER RECEIVED ----------------------------\n");
				 status_code=validate_request_header(buffer,n);
				if(status_code!=NO_ERR_CODE){
					send_err_resp_header(status_code);			
					state = STATE_WAIT_START;
				}else{
					frames_expected = buffer[REQ_FC+1];
					frames_expected|=(((uint16_t)buffer[REQ_FC])<<8);
					memcpy(udp_buffer,buffer,n);
					index = n;
					client_s_addr = cliaddr.sin_addr.s_addr;
					if(frames_expected == 1){
						state = STATE_END_RECVD;
					}else{
						state = STATE_WAIT_END;
					}
				}
			break;
			case STATE_WAIT_END:
				set_time_out(FRAME_TIME_OUT_SECS);
				if (select(32, &select_fds, NULL, NULL, &timeout) == 0 ){
					send_err_resp_header(FRAME_TIME_OUT);
					state = STATE_WAIT_START;
					printf("Time out error \n");
				}else{
					n = recvfrom(sockfd, (unsigned char *)buffer, server_config_obj.bytes_per_frame,MSG_WAITALL, ( struct sockaddr *) &cliaddr,&len);
					if(client_s_addr==cliaddr.sin_addr.s_addr){
						memcpy(&udp_buffer[index],buffer,n);
						index+=n;
						curr_frame_no++;
						printf("--------RECVD  FRAME NO ------ %d\n", curr_frame_no);
						if(curr_frame_no==frames_expected){
							state = STATE_END_RECVD;
						}
					}						
				}	
			break;			
			case STATE_END_RECVD:
					if(udp_buffer[index-1]!=REQ_END|| udp_buffer[index-2]!=REQ_END){
						send_err_resp_header(INVALID_END_OF_REQ);
						printf("Invalid end of packet  \n");
					}else{
						printf("---------------------END RECVD----------------------------------------------\n");
						printf("---------------------PROCESSING REQUEST-----------------------------\n");
						process_request(index);
					}
					state = STATE_WAIT_START;
			break;
		}
	}
}
//-----------------------------------------------------------
// Processes the UDP packet 
//-----------------------------------------------------------
void process_request(unsigned int packet_len){
	uint16_t cmd_no=0, coin_id;
	time_stamp_before = get_time_cs();
	memset(response,0,RESPONSE_HEADER_MAX-1);
	cmd_no = udp_buffer[REQ_CM+1];
	cmd_no |= (((uint16_t)udp_buffer[REQ_CM])<<8);
	coin_id = udp_buffer[REQ_CI+1];
	coin_id |= (((uint16_t)udp_buffer[REQ_CI])<<8);
	switch(cmd_no){
		case CMD_COIN_CONVERTER : 			execute_coin_converter(packet_len);break;
		default:							send_err_resp_header(INVALID_CMD);	
	}
	
}
//-----------------------------------------------------------
// Prepare error response and send it.
//-----------------------------------------------------------
void send_err_resp_header(int status_code){
	int len,size=12;
	unsigned char ex_time;
	char * myfifo = "/tmp/myfifo";
	time_stamp_after = get_time_cs();
	if((time_stamp_after-time_stamp_before) > 255){
		ex_time = 255;
	}else{
		ex_time= time_stamp_after-time_stamp_before;
	}
	printf("Error Status code %d  \n",status_code);
	response[RES_RI] = server_config_obj.raida_id;
	response[RES_SH] = 0;
	response[RES_SS] = status_code;
	response[RES_EX] = 0;
	response[RES_RE] = 0;
	response[RES_RE+1] = 0;
	response[RES_EC] = udp_buffer[REQ_EC];
	response[RES_EC+1] = udp_buffer[REQ_EC+1];
	response[RES_HS] = 0;
	response[RES_HS+1] = 0;
	response[RES_HS+2] = 0;
	response[RES_HS+3] = 0;
	len=sizeof(cliaddr);
	
		sendto(sockfd, (const char *)response, size,
			MSG_CONFIRM, (const struct sockaddr *) &cliaddr,
			len);
	
}
//-----------------------------------------------------------
// Prepare response and send it.
//-----------------------------------------------------------
void prepare_resp_header(unsigned char status_code){
	unsigned char ex_time;
	time_stamp_after = get_time_cs();
	if((time_stamp_after-time_stamp_before) > 255){
		ex_time = 255;
	}else{
		ex_time= time_stamp_after-time_stamp_before;
	}
	response[RES_RI] = server_config_obj.raida_id;
	response[RES_SH] = 0;
	response[RES_SS] = status_code;
	response[RES_EX] = ex_time;
	response[RES_RE] = 0;
	response[RES_RE+1] = 0;
	response[RES_EC] = udp_buffer[REQ_EC];
	response[RES_EC+1] = udp_buffer[REQ_EC+1];
	response[RES_HS] = 0;
	response[RES_HS+1] = 0;
	response[RES_HS+2] = 0;
	response[RES_HS+3] = 0;
}
//-----------------------------------------------------------
//  Validate request header
//-----------------------------------------------------------
unsigned char validate_request_header(unsigned char * buff,int packet_size){
	uint16_t frames_expected,i=0,request_header_exp_len= REQ_HEAD_MIN_LEN, coin_id=0;
	/*if(buff[REQ_EN]!=0){
		for(i=1;i<EN_CODES_MAX+1;i++){
			if(EN_CODES[buff[REQ_EN]]>0)
				break;
		}
	}*/
	if(buff[REQ_EN]!=0 && buff[REQ_EN]!=1){
		return INVALID_EN_CODE;
	}
	request_header_exp_len = REQ_HEAD_MIN_LEN;
	if(packet_size< request_header_exp_len){
		printf("Invalid request header  \n");
		return INVALID_PACKET_LEN;
	}
	frames_expected = buff[REQ_FC+1];
	frames_expected|=(((uint16_t)buff[REQ_FC])<<8);
	printf("No of frames expected :- %d\n", frames_expected);
	if(frames_expected <=0  || frames_expected > FRAMES_MAX){
		printf("Invalid frame count  \n");
		return INVALID_FRAME_CNT;
	}	
	if(buff[REQ_CL]!=0){
		printf("Invalid cloud id \n");
		return INVALID_CLOUD_ID;
	}
	if(buff[REQ_SP]!=0){
		printf("Invalid split id \n");
		return INVALID_SPLIT_ID;
	}
	if(buff[REQ_RI]!=server_config_obj.raida_id){
		printf("Invalid Raida id \n");
		return WRONG_RAIDA;
	}
	return NO_ERR_CODE;
}
//------------------------------------------------------------------------------------------
//  Validate coins and request body and return number of coins 
//-----------------------------------------------------------------------------------------
unsigned char validate_request_body(unsigned int packet_len,unsigned char bytes_per_coin,unsigned int req_body_without_coins,int *req_header_min){
	unsigned int no_of_coins=0;
	*req_header_min = REQ_HEAD_MIN_LEN;// + EN_CODES[udp_buffer[REQ_EN]];	
	no_of_coins = (packet_len-(*req_header_min+req_body_without_coins))/bytes_per_coin;
	if((packet_len-(*req_header_min+req_body_without_coins))%bytes_per_coin!=0){
		send_err_resp_header(LEN_OF_BODY_CANT_DIV_IN_COINS);
		return 0;
	}
	if(no_of_coins==0){
		send_err_resp_header(LEN_OF_BODY_CANT_DIV_IN_COINS);
		return 0;
	}
	if(no_of_coins>COINS_MAX){
		send_err_resp_header(COIN_LIMIT_EXCEED);
		return 0;
	}
	printf("Number of coins = :  %d \n", no_of_coins);	
	return no_of_coins;
}
//------------------------------------------------------------------------------------------
//  Validate coins and request body and return number of coins 
//-----------------------------------------------------------------------------------------
unsigned char validate_request_body_general(unsigned int packet_len,unsigned int req_body,int *req_header_min){
	*req_header_min = REQ_HEAD_MIN_LEN;// + EN_CODES[udp_buffer[REQ_EN]];
	if(packet_len != (*req_header_min) + req_body){
		send_err_resp_header(INVALID_PACKET_LEN);
		return 0;
	}
	return 1;
}
//---------------------------------------------------------------
//	SEND RESPONSE
//---------------------------------------------------------------
void send_response(unsigned char status_code,unsigned int size){
	int len=sizeof(cliaddr);
	char * myfifo = "/tmp/myfifo";
	prepare_resp_header(status_code);
	sendto(sockfd, (const char *)response, size,
		MSG_CONFIRM, (const struct sockaddr *) &cliaddr,
		len);
}
//---------------------------------------------------------------
//Coin converter 215
//---------------------------------------------------------------
void execute_coin_converter(unsigned int packet_len){
	int req_body_without_coins = CH_BYTES_CNT + CMD_END_BYTES_CNT,bytes_per_coin = SN_BYTES_CNT+AN_BYTES_CNT+PAN_BYTES_CNT;
	int req_header_min, no_of_coins,ticket_no=0;
	unsigned int i=0,index=0,j=0,pass_cnt=0,fail_cnt=0,size=0;
	unsigned char status_code,pass_fail[COINS_MAX]={0};
	printf("coin converter Command \n");
	no_of_coins = validate_request_body(packet_len,bytes_per_coin,req_body_without_coins,&req_header_min);
	if(no_of_coins <=0){
		return;
	}
	
	send_response(status_code,size);
}