#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "80211e.h"
#include "ap/hostapd.h"
#include "ap/global_var.h"
#include "common/ieee802_11_common.h"
#include "ap/beacon.h"

////////////////////////////////////////////////
//Create a function to enable/disable 802.11e //
////////////////////////////////////////////////

/**
 * is_802e_enabled - Check if wmm_enabled is set in hostapd config 
 * 
 * @param  intf_name: interface to be checked
 * @param  ssid: one of the interfaces's SSID to be checked
 * 
 * Returns: true if WMM is enabled for the SSID, false if it's disabled or
 *          either the interface or the SSID do not exist
 *
 * Checks if WMM is enabled for a specific BSS on a interface. Although WMM
 * parameters are global for the interface, wether this parameters will be
 * used or not is set per SSID.
 */
bool is_80211e_enabled(char *intf_name, char* ssid){

	struct hapd_interfaces *intfs;
	struct hostapd_data *hapd;
	struct hostapd_config *hapd_conf;

	// Get interfaces from hostapd
	intfs = get_had_intf();
	hapd = hostapd_get_iface(intfs, intf_name);

	// If the interface was not found
	if (hapd == NULL)
		return false;

	hapd_conf = hapd->iconf;

	// If no ssid was provided, checks the last one on the interface
	if (!ssid)
		return hapd_conf->last_bss->wmm_enabled;

  // Searching the BSS and checking if WMM is enabled for it (if found)
	int i;
	for (i = 0; i < hapd_conf->num_bss; i++)
		if (strcmp((char *) hapd_conf->bss[i]->ssid.ssid, ssid) == 0)
			return hapd_conf->bss[i]->wmm_enabled;

  // If bss was not found
	return false;
}

/**
 * get_wmm_ac_param - gets wmm parameters currently in use by the access point
 * 
 * @param  intf_name: access point interface to be queried
 * 
 * @return          :a size 4 array of structs with the interface's aifs,
 *                   cwmin, cwmax, txop_limit and
 *                   adminission_control_mandatory values for each
 *                   access category.
 *
 * WMM access category parameteres are global for the interface. This function
 * returns the configurations currently stored in hostapd's data structure for
 * the interface.
 */
struct hostapd_wmm_ac_params *get_wmm_ac_param(char *intf_name){

	struct hapd_interfaces *intfs; //hostapd per-interface data structure
	struct hostapd_data *hapd; //hostapd per-BSS data structure
	struct hostapd_config *hapd_conf; //Per-radio interface configuration

	struct hostapd_wmm_ac_params *ac_params;

	// Get interfaces from hostapd
	intfs = get_had_intf();
	hapd = hostapd_get_iface(intfs, intf_name);

	// If the interface was not found
	if (hapd == NULL)
		return false;

	// Get interface data structure
	hapd_conf = hapd->iconf;

	// Returns the hostapd_wmm_ac_params array (which by definition is also a
	// pointer)
	ac_params = hapd_conf->wmm_ac_params;

	return ac_params;
}

/**void set_wmm_ac_param(char *intf_name, int ac, int aifs, int cwmin, int cwmax, int txop, int adm_control){
	
	struct hapd_interfaces *intfs;
	struct hostapd_data *hapd;
	struct hostapd_config *hapd_conf;

	// Get interfaces from hostpad
	intfs = get_had_intf();
	hapd = hostapd_get_iface(intfs, intf_name);

	// If the interface was not found
	if (hapd == NULL){
		#ifdef DEBUG
			printf("%s: %s\n", "Interface não encontrada", intf_name);
		#endif
	}else{
		hapd_conf = hapd->iconf;
		
		struct hostapd_wmm_ac_params *ac_params = &hapd_conf->wmm_ac_params[ac];

		ac_params->aifs = aifs;
		ac_params->cwmin = cwmin;
		ac_params->cwmax = cwmax;
		ac_params->txop_limit = txop;
		ac_params->admission_control_mandatory = adm_control;

		ieee802_11_update_beacons(hapd->iface);
	}
}**/
