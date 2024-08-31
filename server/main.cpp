// tcp_server_threaded.cpp
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include <time.h>

#include "db_function/db_function.h"
#include "handle_client_components/handle_client_components.h"

void* handle_client(void* client_socket) {

    char buffer[BUFFER_SIZE] = {0};

    client_objects l_user;
    l_user.start_connection(client_socket);


    while(l_user.work)
    {
        read(l_user.sock, buffer, BUFFER_SIZE);
        int code = get_code(buffer);

        if(code == 0)
        {
            l_user.protocol_for_end_connection();
        }

        if(code == 1)
        {
            char username[32] = {0};
            char password[32] = {0};

            get_username(buffer,&username);
            get_password(buffer,&password);

            l_user.protocol_for_log_in(&username,&password);
        }

        if(code == 2)
        {
            char username[32] = {0};
            char password[32] = {0};

            get_username(buffer,&username);
            get_password(buffer,&password);

            l_user.protocol_for_registration(&username,&password);
        }

        if(code == 3)
        {
            l_user.protocol_for_user_list_request();
        }

        if(code == 4)
        {
            int id;
            char msg[500] = {0};

            id = get_user_id_after_code(buffer);
            get_msg(buffer,&msg);

            l_user.protocol_for_send_msg(id,&msg);
        }

        if(code == 5)
        {
            int user_id;
            user_id = get_user_id_after_code(buffer); 

            l_user.protocol_for_get_msgs(user_id);
        }

        if(code==6)
        {
            int user_id;
            user_id = get_user_id_after_code(buffer); 

            l_user.protocol_for_get_msg(user_id);
        }
    }    

    close(l_user.sock);
    return nullptr;
}


int main() {
    delete_db();
    create_db();

    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    pthread_t thread_id;

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    std::cout << "Server listening on port " << PORT << std::endl;

    while (true) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            close(server_fd);
            exit(EXIT_FAILURE);
        }

        if (pthread_create(&thread_id, nullptr, handle_client, (void*)&new_socket) != 0) {
            perror("pthread_create failed");
            close(new_socket);
        }

        pthread_detach(thread_id);
    }

    close(server_fd);
    return 0;
}