#ifndef MSG_WMM_PARAMS_H
#define MSG_WMM_PARAMS_H

#include "msg_common.h"
#include "ap/global_var.h"
#include "common/ieee802_11_common.h"

/**
 * WMM message structure. hostapd_wmm_ac_params is an 4 sized array.
 */
struct msg_wmm_params{
	int m_type;
	int m_id;
	char *p_version;
	int m_size;
	char * sta_ip; // if sta_ip == NULL grab local wlan information
	int sta_port;  // if not, call the station identified by (sta_ip:sta_port)
	char *intf_name;
	int ac; // -1 if parameters of all access categories should be queried,
			// access category index otherwise (0 to 3)
	
	struct hostapd_wmm_ac_params *wmm_ac_params; 
};

void process_msg_wmm_params(char **input, int input_len, char **output, int *output_len);

void free_msg_wmm_params(struct msg_wmm_params *p);

void print_msg_wmm_params(struct msg_wmm_params *p);

#endif