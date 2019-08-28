/* TODO: create the following messages
          MSG_GET_QUEUE_PARAMS,
          MSG_SET_QUEUE_PARAMS,
          MSG_GET_WMM_PARAMS,
          MSG_SET_WMM_PARAMS
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "openssl/ssl.h"
#include "ssl_common.h"
#include "../ethanol_functions/hostapd_hooks.h"

#include "buffer_handler_fun.h"
#include "msg_common.h"

/*
    create hooks to the functions in hostapd/ctrl_iface.c
    * static int hostapd_ctrl_iface_get_queue_params(struct hostapd_data *hapd, char *buf, size_t buflen)
    * static int hostapd_ctrl_iface_set_queue_params(struct hostapd_data *hapd, char *params, char *buf, size_t buflen)
    * static int hostapd_ctrl_iface_get_wmm_params(struct hostapd_data *hapd, char *buf, size_t buflen)
    * static int hostapd_ctrl_iface_set_wmm_params(struct hostapd_data *hapd, char *params, char *buf, size_t buflen)
 */
