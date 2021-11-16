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
#define RAIDA_SERVER_MAX 					25
//--------------------------------------------
#define SERVER_CONFIG_BYTES 				16
//------------------------------------------
#define TICKETS_MAX 						10000
#define TICKET_SR_NOS_MAX 				2000
//------------------------------------------
#define AN_MAX							32
//------------------------------------------
#define COIN_CONFIG_BYTES 				5
#define COIN_ID_INDEX		 				0
#define COIN_CLOUD_INDEX		 			1
#define COIN_TABLES_CNT					2
//-------------------------------------------

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
struct coin_config {
	unsigned char coin_id;
	unsigned int page_size;
	unsigned int no_of_pages;
};
struct dns_config {
	int sockfd;
	struct sockaddr_in servaddr,cliaddr;
	unsigned char buffer[1024];
};

struct master_ticket{
	uint32_t ticket_no;
	uint32_t raida_claim;	//0-24 bits represent the ticket spent or not
	uint32_t time_stamp;
};

struct date {
	unsigned int year;
	unsigned char month,day,hh,mm,ss;
};

struct coin_id {
	unsigned char **AN;
	unsigned char *MFS;
	struct master_ticket **TICKETS;
	unsigned int AN_CNT;
	unsigned char *free_id_days;
};

//-------------------------------------------------
void welcomeMsg();
int load_raida_no();
int load_server_config();
//-------------------------------------------------
extern char execpath[256];
extern unsigned char shards_config[SHARDS_MAX];
extern struct server_config server_config_obj;
extern struct dns_config dns_config_obj[RAIDA_SERVER_MAX];
extern struct raida_legacy_config raida_legacy_config_obj[RAIDA_SERVER_MAX];
extern unsigned int coin_id_cnt;
extern struct coin_id coin_id_obj[255];
extern struct coin_config *coin_config_obj;
extern unsigned int *pages_changed;

#endif
