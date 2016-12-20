#ifndef __SOCKET_JNI_H__
#define __SOCKET_JNI_H__

#define MAX_BUFF        255
#define TARGET_PORT     5001
#define MAX_MSG_LEN     0


void init_socket_server();
int set_socket_param();
int wait_socket_server_session();
int get_socket_message(char* destination_buffer);
int send_socket_message(char* message);
void close_socket_server_session();
void end_socket_server();


#endif //__SOCKET_JNI_H__
