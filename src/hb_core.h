#ifndef _HB_CORE_H
#define _HB_CORE_H

#include "common.h"
#include "protype.h"


enum hbrc_sm_type {
	HBRC_INVALID = 0,
	HBRC_STRAT,
	HBRC_INIT,
	HBRC_IDLE,
	HBRC_CHANLLENGE,
	HBRC_ECHO,
	HBRC_CLEAN,
};

struct hbc_conf {
	int echo_interval;
	int retry_count;
	int retry_interval;
	int noecho_interval;
};

/* 心跳路由客户端  */
struct heartbeat_route_client {
	int hbrc_sm;
	struct hb_server **hbs_head;
	struct hb_server *current_hbs;
	struct hbc_conf hbrc_conf;
	int hbs_count;

	/* connect param*/
	char equipmentSn[6];
	int hbrc_sockfd;
	int sendsn;	 //发送序列号
	int recvsn;  //接收序列号
	u32_t session_server_key;
	u32_t session_client_key;
	int last_req_echosn; //最近一个收到的心跳回应包
	int last_resp_echosn; //最近一个收到的心跳回应包
	int lost_echo_count;

	/*recv buff*/
    char* gbuf;
    int dataLen;
    int maxLen;
	int activeRecvFlag;

	/*function*/
	void (*chall_encode) (const void *i_blk, void *o_blk, void *key, int len);
	void (*chall_decode) (const void *i_blk, void *o_blk, void *key, int len);
	int (*msg_encode) (void *i_block,  void *o_block, u32_t key, u32_t len);
	int (*msg_decode) (void *i_block,  void *o_block, u32_t key, u32_t len);

};

/* 心跳路由客户端的服务器列表 */
struct hb_server {
	//int hbs_sm;

	struct in_addr hbs_ip;
	int hbs_port;
	int hbs_index;
	int try_conn;
	int used;	
};

struct echo_thread {
	pthread_t echo_thpid;
	int pause_flag;
};


struct recv_thread {
	pthread_t recv_thpid;
	int pause_flag;
	int flush_flag;
};

struct dispatch_thread {
	pthread_t dispatch_thpid;
	int terminal_flag;
};

struct udp_thread {
	pthread_t udpserver_thpid;
	int pause_flag;
};


struct glob_arg {
	/*command*/
	char* configFile;
	int debuglevel;
	int log_syslog;
	//struct in_addr beforeserverip;
	
	/*hbrc*/
	struct heartbeat_route_client *hbrc;

	
	/*thread*/
	struct echo_thread echoThread;
	struct recv_thread recvThread;
	struct dispatch_thread dispatchThread;
	struct udp_thread udpThread;
};

#define MAX_HB_COUNT 10
#endif
