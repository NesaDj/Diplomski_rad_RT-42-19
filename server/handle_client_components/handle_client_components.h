#ifndef HANDLE_CLIENT_H
#define HANDLE_CLIENT_H

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

const int PORT = 12345;
const int BUFFER_SIZE = 1024;

int get_code(char* buffer);
void put_code(char* buffer,int n);
void get_msg(char* buffer,void* msg);
void get_username(char* buffer,void* username);
int get_password_start(char* buffer);
void get_password(char* buffer,void* password);
int get_user_id_after_code(char* buffer);
bool try_register(void* username,void* password);
int get_user_num();
int get_msg_num(int id1,int id2);
void get_username_list(void* username,int id);
void get_msg_list(void* msg_list,int id1,int id2);

class client_objects
{
    public:
        bool is_authenticated;
        int id;

        bool work;
        int sock;

        char username[32];
        
        client_objects():is_authenticated(false),id(0),work(true)
        {};

        void set_sock(void* client_socket);
        bool try_authenticate(void* username,void* password);
        bool put_msg(int l_id,void* msg);
        void get_last_msg(void *msg,int user_id);
        void start_connection(void* client_socket);
        void protocol_for_end_connection();
        void protocol_for_log_in(void* username,void* password);
        void protocol_for_registration(void* username,void* password);
        void protocol_for_user_list_request();
        void protocol_for_send_msg(int l_id, void* msg);
        void protocol_for_get_msgs(int user_id);
        void protocol_for_get_msg(int user_id);
};

#endif