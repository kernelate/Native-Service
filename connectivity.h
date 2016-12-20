void set_eth_service(int set);

void set_dns(char* dns1,char* dns2);

int set_ip(char *iface_name, char *ip_addr, char *net_mask);

void interface_static(char* interface,char* ip_addr,char* net_mask, char* gateway);

void interface_dynamic(char* interface);

void* eth_checker(void*arg);

void eth0_service(void);

void adb_wifi_enable(int portno);

void adb_wifi_disable(void); 

int check_ip();

#define ETH_PATH_FILE "/data/temp_determine_ip_alive"