# Check if ETHANOL_ENABLED is set in configs
-include ../hostapd/.config

SUBDIRS=ap common crypto drivers eapol_auth eapol_supp eap_common eap_peer eap_server l2_packet p2p pae radius rsn_supp tls utils wps
SUBDIRS += fst

# Check if ETHANOL_ENABLED is set
ifdef ETHANOL_ENABLED
# #############################################################
#
# ETHANOL
#
SUBDIRS+=messaging ini ethanol_functions
#
#
# #############################################################
endif

all:
	for d in $(SUBDIRS); do [ -d $$d ] && $(MAKE) -C $$d; done

clean:
	for d in $(SUBDIRS); do [ -d $$d ] && $(MAKE) -C $$d clean; done
	rm -f *~

install:
	for d in $(SUBDIRS); do [ -d $$d ] && $(MAKE) -C $$d install; done
