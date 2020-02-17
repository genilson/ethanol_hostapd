/** \file 80211.h

   System:         Linux
   Component Name: Ethanol
   Status:         Version 1.0 Release 1
   Language: C

   License: GNU Public License

   Description: Obtem se o ap está com as funcionalidades do 802.11e habilitadas ou não

   Limitations: funciona somente em ambiente linux com sockets
   Dependências:

   Function: 1) is_80211e_enabled

   Thread Safe: NO

   Compiler Options: none

   Change History:           (Sometimes called "Revisions")
   Date         Author       Description
   24/05/2016   Henrique     primeiro release
 */

#ifndef __80211_H
#define __80211_H

#include <stdbool.h>
#include <stdio.h>

/**
   @return if interface "intf_name" is 802.11e enabled
           if interface is not found, returns false
 */
bool is_80211e_enabled(char * intf_name, char * ssid);

struct hostapd_wmm_ac_params *get_wmm_ac_params(char *intf_name);

void set_wmm_ac_params(char *intf_name, int ac,
   struct hostapd_wmm_ac_params *ac_params);

/**
   called by a hook inside config_file.c
*/
void set_if_hostapd_is_80211e_enabled(char * intf_name, bool value);

void free_hostapd_is_80211e_enabled(void);

#endif