#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "80211e.h"
#include "ap/hostapd.h"
#include "ap/global_var.h"


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
bool is_80211e_enabled(char * intf_name, char * ssid){
  
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
