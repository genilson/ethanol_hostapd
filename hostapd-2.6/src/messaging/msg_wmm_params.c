#include "msg_wmm_params.h"
#include "buffer_handler_fun.h"
#include "common/ieee802_11_common.h"
#include "ethanol_functions/80211e.h"
#include "messaging/msg_common.h"

unsigned int msg_size_wmm_params(struct msg_wmm_params * h){

	int size = 0;

	size =
		sizeof(h->m_type) +
		sizeof(h->m_id) +
		strlen_ethanol(h->p_version) +
		sizeof(h->m_size) +
		strlen_ethanol(h->sta_ip) +
		sizeof(h->sta_port) +
		strlen_ethanol(h->intf_name) +
		sizeof(h->ac);

	if (h->ac >= 0 && h->ac <= 3){
		size += sizeof(h->wmm_ac_params->aifs) +
				sizeof(h->wmm_ac_params->cwmin) +
			    sizeof(h->wmm_ac_params->cwmax) +
			    sizeof(h->wmm_ac_params->txop_limit) +
			    sizeof(h->wmm_ac_params->admission_control_mandatory);
	}else if (h->ac == -1){
		int i;
		for (i = 0; i < 4; i++){
			size += sizeof(h->wmm_ac_params[i].aifs) +
					sizeof(h->wmm_ac_params[i].cwmin) +
				    sizeof(h->wmm_ac_params[i].cwmax) +
				    sizeof(h->wmm_ac_params[i].txop_limit) +
				    sizeof(h->wmm_ac_params[i].admission_control_mandatory);
		}
	}

	return size;
}

void print_msg_wmm_params(struct msg_wmm_params *h){
	printf("Type     : %d\n", h->m_type);
	printf("Msg id   : %d\n", h->m_id);
	printf("Version  : %s\n", h->p_version);
	printf("Msg size : %d\n", h->m_size);
	printf("Station  : %s:%d\n", h->sta_ip, h->sta_port);
	printf("Interface: %s\n", h->intf_name);
	printf("AC: %i\n", h->ac);

	//For printing
	char *categories[] = {"Best Effort", "Background", "Video", "Voice"};
	
	if (h->ac >= 0 && h->ac <= 3){
	// Printing parameters of selected access category
		printf("Category : %s\n", categories[h->ac]);
		printf("	       AIFS: %i\n", h->wmm_ac_params->aifs);
		printf("	       CWmin: %i\n", h->wmm_ac_params->cwmin);
		printf("	       CWmax: %i\n", h->wmm_ac_params->cwmax);
		printf("           TXOP Limit: %i\n", h->wmm_ac_params->txop_limit);
		printf("	       Admission Control Mandatory: %i\n",
			h->wmm_ac_params->admission_control_mandatory);
	}else if (h->ac == -1){
	// Printing parameters of all access categories
		int i;
		for(i=0; i < 4; i++) {
			printf("Category : %s\n", categories[i]);
			printf("	       AIFS: %i\n", h->wmm_ac_params[i].aifs);
			printf("	       CWmin: %i\n", h->wmm_ac_params[i].cwmin);
			printf("	       CWmax: %i\n", h->wmm_ac_params[i].cwmax);
			printf("           TXOP Limit: %i\n", h->wmm_ac_params[i].txop_limit);
			printf("	       Admission Control Mandatory: %i\n",
				h->wmm_ac_params[i].admission_control_mandatory);
		}
	}
}

void encode_msg_wmm_params(struct msg_wmm_params *h, char **buf, int *buf_len) {

	*buf_len = msg_size_wmm_params(h);
	*buf = malloc(*buf_len);
	char *aux = *buf;

	h->m_size = *buf_len;
	encode_header(&aux, h->m_type, h->m_id, h->m_size);
	encode_char(&aux, h->sta_ip);
	encode_int(&aux, h->sta_port);
	encode_char(&aux, h->intf_name);
	encode_int(&aux, h->ac);

	if (h->ac >= 0 && h->ac <= 3){
		encode_int(&aux, h->wmm_ac_params->aifs);
		encode_int(&aux, h->wmm_ac_params->cwmin);
		encode_int(&aux, h->wmm_ac_params->cwmax);
		encode_int(&aux, h->wmm_ac_params->txop_limit);
		encode_int(&aux, h->wmm_ac_params->admission_control_mandatory);
	}else if (h->ac == -1){
		int i;
		for(i = 0; i < 4; i++) { //4 ACs
			encode_int(&aux, h->wmm_ac_params[i].aifs);
			encode_int(&aux, h->wmm_ac_params[i].cwmin);
			encode_int(&aux, h->wmm_ac_params[i].cwmax);
			encode_int(&aux, h->wmm_ac_params[i].txop_limit);
			encode_int(&aux, h->wmm_ac_params[i].admission_control_mandatory);
		}
	}
}

void decode_msg_wmm_params(char *buf, int buf_len, struct msg_wmm_params **h) {
	// Allocating space for the metadata
	*h = malloc(sizeof(struct msg_wmm_params));

	char *aux = buf;
	decode_header(&aux, &(*h)->m_type, &(*h)->m_id,  &(*h)->m_size, &(*h)->p_version);
	decode_char(&aux, &(*h)->sta_ip);
	decode_int(&aux, &(*h)->sta_port);
	decode_char(&aux, &(*h)->intf_name);
	decode_int(&aux, &(*h)->ac);

	//Allocating space accordingly and decoding the payload
	if ((*h)->ac >= 0 && (*h)->ac <= 3){
		(*h)->wmm_ac_params = malloc(sizeof(struct hostapd_wmm_ac_params));
		decode_int(&aux, &(*h)->wmm_ac_params->aifs);
		decode_int(&aux, &(*h)->wmm_ac_params->cwmin);
		decode_int(&aux, &(*h)->wmm_ac_params->cwmax);
		decode_int(&aux, &(*h)->wmm_ac_params->txop_limit);
		decode_int(&aux, &(*h)->wmm_ac_params->admission_control_mandatory);
	}else if ((*h)->ac == -1){

		(*h)->wmm_ac_params = malloc(4 * sizeof(struct hostapd_wmm_ac_params));

		int i;
		for (i = 0; i < 4; i++){ //4 ACs
			decode_int(&aux, &(*h)->wmm_ac_params[i].aifs);
			decode_int(&aux, &(*h)->wmm_ac_params[i].cwmin);
			decode_int(&aux, &(*h)->wmm_ac_params[i].cwmax);
			decode_int(&aux, &(*h)->wmm_ac_params[i].txop_limit);
			decode_int(&aux, &(*h)->wmm_ac_params[i].admission_control_mandatory);
		}
	}
}

void process_msg_wmm_params(char **input, int input_len, char **output, int *output_len){
	
	struct msg_wmm_params *h;
	struct hostapd_wmm_ac_params *ac_params;

	decode_msg_wmm_params(*input, input_len, &h);

	ac_params = get_wmm_ac_params(h->intf_name);

	if (h->m_type == MSG_GET_WMM_PARAMS){
		// If an AC was provided, only its parameters values will be sent on
		// the response
		if (h->ac >= 0 && h->ac <=3){
			h->wmm_ac_params->aifs = ac_params[h->ac].aifs;
			h->wmm_ac_params->cwmin = ac_params[h->ac].cwmin;
			h->wmm_ac_params->cwmax = ac_params[h->ac].cwmax;
			h->wmm_ac_params->txop_limit = ac_params[h->ac].txop_limit;
			h->wmm_ac_params->admission_control_mandatory =
				ac_params[h->ac].admission_control_mandatory;
		// Otherwise all the parameters values of the 4 AC will be sent to the
		// controller
		}else if (h->ac == -1){
			int i;
			for (i = 0; i < 4; i++){
				h->wmm_ac_params[i].aifs = ac_params[i].aifs;
				h->wmm_ac_params[i].cwmin = ac_params[i].cwmin;
				h->wmm_ac_params[i].cwmax = ac_params[i].cwmax;
				h->wmm_ac_params[i].txop_limit = ac_params[i].txop_limit;
				h->wmm_ac_params[i].admission_control_mandatory =
					ac_params[i].admission_control_mandatory;
			}
		}
		
		//Check debug options
		//#ifdef DEBUG
			print_msg_wmm_params(h);
		//#endif

		encode_msg_wmm_params(h, output, output_len);

		free_msg_wmm_params(h);

	}else if (h->m_type == MSG_SET_WMM_PARAMS){
		// Setting parameters is done one AC at a time.
		set_wmm_ac_params(h->intf_name, h->ac, h->wmm_ac_params);
	}
}

void free_msg_wmm_params(struct msg_wmm_params *p) {
	if (p == NULL) return;
	if (p->p_version) free(p->p_version);
	if (p->sta_ip) free(p->sta_ip);
	if (p->intf_name) free(p->intf_name);
	if (p->wmm_ac_params) free(p->wmm_ac_params);

	free(p);
	p = NULL;
}