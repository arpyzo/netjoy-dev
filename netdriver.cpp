#include "debug.h"
#include "netdriver.h"

/***************************** NetDriver ******************************/
Netdriver::Netdriver() {

}

bool Netdriver::Init_NIC() {
    pcap_if_t *nics, *nic;
	pcap_t *fp;
	char error_buffer[PCAP_ERRBUF_SIZE];
	struct pcap_pkthdr *header;
	const u_char *pkt_data;
	int res;

	if (pcap_findalldevs_ex(PCAP_SRC_IF_STRING, NULL, &nics, error_buffer) == -1) {
		wxMessageBox("Failed to retrieve network card list!");
        return FALSE;
	} else {
		for (nic = nics; nic; nic = nic->next) {
			wxMessageBox(wxString::Format("\"%s\"", nic->name));
			if (nic->description) {
				wxMessageBox(wxString::Format(" (%s)\n", nic->description));
			}
			else {
				wxMessageBox("(No description available)\n");
			}
		}
	}
    Get_Friendly_Name();
    //return TRUE;
/*
	if ((fp = pcap_open(nics->next->name, 100, PCAP_OPENFLAG_PROMISCUOUS, 20, NULL, error_buffer)) == NULL) {
		wxMessageBox(wxString::Format("Error opening source: %s", error_buffer));
	} else {
		// Read the packets
        int num_packets = 0;
		while ((res = pcap_next_ex(fp, &header, &pkt_data)) >= 0 && num_packets < 10) {

			if (res == 0)
				// Timeout elapsed
				continue;
            num_packets++;
			// print pkt timestamp and pkt len
			wxMessageBox(wxString::Format("%ld:%ld (%ld)\n", header->ts.tv_sec, header->ts.tv_usec, header->len));

                ip_header *ih = (ip_header *) (pkt_data + 14);

			// Print the packet
			/*for (int i = 1; (i < header->caplen + 1); i++) {
				printf("%.2x ", pkt_data[i - 1]);
				if ((i % 16) == 0) printf("\n");
			}*/

                  /* print ip addresses and udp ports */
    //printf("%d.%d.%d.%d.%d -> %d.%d.%d.%d.%d\n",
    /*wxMessageBox(wxString::Format("%d.%d.%d.%d -> %d.%d.%d.%d\n",
        ih->saddr.byte1,
        ih->saddr.byte2,
        ih->saddr.byte3,
        ih->saddr.byte4,
        //sport,
        ih->daddr.byte1,
        ih->daddr.byte2,
        ih->daddr.byte3,
        ih->daddr.byte4
        //dport
        ));

			printf("\n\n");
		}

		if (res == -1) {
			wxMessageBox(wxString::Format("Error reading the packets: %s\n", pcap_geterr(fp)));
		}
	}*/
}

void Netdriver::Get_Friendly_Name() {
    HMODULE IPHLPAPI_DLL;
    // Use GUID

        IPHLPAPI_DLL = LoadLibrary(TEXT("iphlpapi.dll"));
    if (IPHLPAPI_DLL == NULL) {
        wxMessageBox("Failed to load iphlpapi.dll");
    } else {
        wxMessageBox("Successfully loaded iphlpapi.dll");
    }
}