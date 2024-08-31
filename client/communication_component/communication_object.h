#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string> 

#define SERVER_IP "127.0.0.1"
#define PORT 12345
#define BUFFER_SIZE 1024

#define NO_CODE 0
#define CODE_FOR_END 0
#define CODE_FOR_LOGIN 1
#define CODE_FOR_REGISTER 2
#define CODE_FOR_USER_LIST 3
#define CODE_FOR_MSG_SEND 4
#define CODE_FOR_MSGS 5
#define CODE_FOR_MSG 6

typedef struct {
    char username[32];
    int id;
} User;

typedef struct {
    char* text;
    int to_user_id;
}Msg;


void put_code(char* buffer,int n);
int get_code(char* buffer);
void append_username_password(char* buffer,char username[32],char password[32]);
int get_num_after_code(char* buffer);
int get_id_after_username(char* buffer);
void get_username(char* buffer,void* username);
void put_id_msg(char* buffer,int id,char* msg);
void append_id_after_code(char* buffer,int id);
void get_msg(char* buffer,char* msg);
int get_msg_len(char* buffer);
int get_id_after_msg(char* buffer,int msg_len);

class communication
{
    private:
        int sock = 0;
        struct sockaddr_in serv_addr;

        bool posting_msg;
        bool getting_msg;


    public:

        User* user_list;
        int number_of_user_in_list;

        Msg* msgs_list;
        int number_of_msg;

        Msg last_msg;

        communication()
        {
            posting_msg = false;
            getting_msg = false;
        };

        bool start_communication();
        bool try_registration(char username[32],char password[32]);
        bool try_login(char username[32],char password[32]);
        bool end_communication();
        bool try_get_users();
        void print_user_list();
        int get_id_from_username(char username[32]);
        bool try_to_send_msg(int id,char msg[500]);
        bool try_to_get_msgs(int id);
        bool try_to_get_msg(int id);

};

#endif