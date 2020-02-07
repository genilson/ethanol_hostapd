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
		strlen_ethanol(h->intf_name);

	int i;
	for (i = 0; i < 4; i++){
		size += sizeof(h->wmm_ac_params[i].aifs) +
				sizeof(h->wmm_ac_params[i].cwmin) +
			    sizeof(h->wmm_ac_params[i].cwmax) +
			    sizeof(h->wmm_ac_params[i].txop_limit) +
			    sizeof(h->wmm_ac_params[i].admission_control_mandatory);
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

	//For printing
	char *categories[] = {"Best Effort", "Background", "Video", "Voice"};
	
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

void encode_msg_wmm_params(struct msg_wmm_params *h, char **buf, int *buf_len) {

	*buf_len = msg_size_wmm_params(h);
	*buf = malloc(*buf_len);
	char *aux = *buf;

	h->m_size = *buf_len;
	encode_header(&aux, h->m_type, h->m_id, h->m_size);
	encode_char(&aux, h->sta_ip);
	encode_int(&aux, h->sta_port);
	encode_char(&aux, h->intf_name);
	
	int ac;
	for(ac = 0; ac < 4; ac++) { //4 ACs
		encode_int(&aux, h->wmm_ac_params[ac].aifs);
		encode_int(&aux, h->wmm_ac_params[ac].cwmin);
		encode_int(&aux, h->wmm_ac_params[ac].cwmax);
		encode_int(&aux, h->wmm_ac_params[ac].txop_limit);
		encode_int(&aux, h->wmm_ac_params[ac].admission_control_mandatory);
	}
}

void decode_msg_wmm_params(char *buf, int buf_len, struct msg_wmm_params **h) {
	*h = malloc(sizeof(struct msg_wmm_params));
	(*h)->wmm_ac_params = malloc(4 * sizeof(struct hostapd_wmm_ac_params));

	char *aux = buf;
	decode_header(&aux, &(*h)->m_type, &(*h)->m_id,  &(*h)->m_size, &(*h)->p_version);
		decode_char(&aux, &(*h)->sta_ip);
		decode_int(&aux, &(*h)->sta_port);
		decode_char(&aux, &(*h)->intf_name);

		int ac;
		for (ac = 0; ac < 4; ac++){ //4 ACs
			decode_int(&aux, &(*h)->wmm_ac_params[ac].aifs);
			decode_int(&aux, &(*h)->wmm_ac_params[ac].cwmin);
			decode_int(&aux, &(*h)->wmm_ac_params[ac].cwmax);
			decode_int(&aux, &(*h)->wmm_ac_params[ac].txop_limit);
			decode_int(&aux, &(*h)->wmm_ac_params[ac].admission_control_mandatory);
		}
}


//Passos:
//- Decodificar a mensagem
//- Pegar informação de qual interface deseja-se fazer a consulta
//- Consultar a informação no struct do hostpad, preencher o struct
//- Encodar a mensagem
//- Enviar resposta para o solicitante
//- Se tipo de mensagem for para alterar parâmetros, alterar no struct atual
//

void process_msg_wmm_params(char **input, int input_len, char **output, int *output_len){
	
	struct msg_wmm_params *h;
	struct hostapd_wmm_ac_params *ac_params;

	decode_msg_wmm_params(*input, input_len, &h);

	ac_params = get_wmm_ac_param(h->intf_name);

	if (h->m_type == MSG_GET_WMM_PARAMS){

		int i;
		for (i = 0; i < 4; i++)
			h->wmm_ac_params[i] = ac_params[i];

		print_msg_wmm_params(h);

		encode_msg_wmm_params(h, output, output_len);
	}else if (h->m_type == MSG_SET_WMM_PARAMS){
		
		// Check parameters with positive or zero values and change them on the
		// interface
		// Call function to set params.
		//void set_wmm_ac_param(char *intf_name, int ac, int aifs, int cwmin, int cwmax, int txop, int adm_control);
	}
}

void free_msg_wmm_params(struct msg_wmm_params **p) {
	if (p == NULL) return;
	if (*p == NULL) return;
	if ((*p)->p_version) free((*p)->p_version);
	if ((*p)->sta_ip) free((*p)->sta_ip);
	if ((*p)->intf_name) free((*p)->intf_name);

	//clear params mem space

	free(*p);
	*p = NULL;
}