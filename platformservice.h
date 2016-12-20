
void write_klog(const char *tag, char *message);
void display_version();
void dumpsystem();
void start_socket_service();
void wait_socket_session();
void rcv_msg(char* msgbuff);
int reply_msg(char* reply1, char* reply2);
void close_socket_session();
void stop_socket_service();
void clear_params(void);
int msg_parser(char* msgbuff);
int msg_handler(char* msgbuff);