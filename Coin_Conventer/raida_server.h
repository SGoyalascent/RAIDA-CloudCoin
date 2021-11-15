#ifndef AGENT_SERVER_H
#define AGENT_SERVER_H
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdint.h>
#include <time.h>
#include <math.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>

//--------------------------------------------
#define SERVER_CONFIG_BYTES 				16
//------------------------------------------

#include "udp_socket.h"

struct server_config {
	unsigned char raida_id;
	unsigned int port_number;
	unsigned int security_check_frequency;
	unsigned int backup_frequency;
	unsigned int refresh_dns;
	unsigned int show_regs_coins_max;
	unsigned int show_denom_coins_max;
	unsigned int show_change_coins_max;
	unsigned int bytes_per_frame;
	unsigned int del_ticket_time_intervel;
	unsigned int del_free_id_time_intervel;	//days
};

struct master_ticket{
	uint32_t ticket_no;
	uint32_t raida_claim;	//0-24 bits represent the ticket spent or not
	uint32_t time_stamp;
};

//-------------------------------------------------
void welcomeMsg();
int load_raida_no();
int load_server_config();
//-------------------------------------------------
extern char execpath[256];
extern struct server_config server_config_obj;

#endif
