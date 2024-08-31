#include "../db_function/db_function.h"
#include "handle_client_components.h"


void client_objects::set_sock(void* client_socket)
{
    this->sock = *(int*)client_socket;
}

bool client_objects::try_authenticate(void* username,void* password)
{
    char* username_l = (char*)username;
    char* password_l = (char*)password;

    if(check_for_user(username,password)==true)
    {
        this->is_authenticated = true;
        this->id = get_user_id_from_db(username,password);

        std::cout << id << std::endl;

        return true;
    }else
    {
        return false;
    }
}

bool client_objects::put_msg(int l_id,void* msg)
{
    char* msg_l = (char*)msg;

    write_msg(this->id,l_id,msg);

    return true;
}

void client_objects::get_last_msg(void *msg,int user_id)
{
    get_last_msg_from_db(msg,user_id,this->id);
}

void client_objects::start_connection(void* client_socket)
{
    this->set_sock(client_socket);

    std::cout << "New connection..." << std::endl;
};

void client_objects::protocol_for_end_connection()
{
    char *response = new char[4];

    put_code(response,0);
    send(this->sock, response, strlen(response), 0);
            
    work = false;
    std::cout << "Connection end..." << std::endl;

    delete[] response; 
};

void client_objects::protocol_for_log_in(void* username,void* password)
{
    char *response = new char[4];

    if(this->try_authenticate(username,password)==true)
    {
        strcpy(this->username,(char *)username);
        std::cout << this->id << " - Log in request accepted - " << this->username << std::endl;    
        put_code(response,0);
    }else
    {
        std::cout << "Log in request decline" << std::endl;
        put_code(response,1);
    }   

    send(this->sock, response, strlen(response), 0);

    delete[] response; 
}

void client_objects::protocol_for_registration(void* username,void* password)
{
    char *response = new char[4];
            
    if(try_register(username,password)==true)
    {
        std::cout << "Register request accepted - " << username << std::endl;    
        put_code(response,0);
    }else
    {
        std::cout << "Register request decline" << std::endl;    
        put_code(response,1);
    }

    send(this->sock, response, strlen(response), 0);

    delete[] response; 
}

void client_objects::protocol_for_user_list_request()
{
    int code;
    char buffer[BUFFER_SIZE] = {0};

    std::cout << this->id << " - User list request" << std::endl;    

    char *response = new char[10];            
    int user_num = get_user_num()-1;

    char *num = new char[10];

    sprintf(num, "%d", user_num);

    put_code(response,0);
    strcat(response,num);
    strcat(response,"_");

    send(this->sock, response, strlen(response), 0);

    char** username_list = new char*[user_num];

    for(int i=0;i<user_num;i++)
    {
        username_list[i] = new char[40]; 
    }

    get_username_list(username_list,this->id);

    for(int i=0;i<user_num;i++)
    {
        read(this->sock, buffer, BUFFER_SIZE);
        code = get_code(buffer);

        put_code(response,0);
        strcat(response,username_list[i]);

        send(this->sock, response, strlen(response), 0);
    }

    read(this->sock, buffer, BUFFER_SIZE);
    code = get_code(buffer);

    if(code==0)
    {
        put_code(response,0);
        send(this->sock, response, strlen(response), 0);
        std::cout << "List transfer secsfully..." << std::endl;
    }
            
    delete[] response; 
}

void client_objects::protocol_for_send_msg(int l_id, void* msg)
{
    char* msg_l = (char*)msg;
    write_msg(this->id,l_id,msg);

    char *response = new char[10];
    put_code(response,0);

    send(this->sock, response, strlen(response), 0);

    std::cout <<  this->id << " - Msg send" << std::endl;

    delete[] response;
}

void client_objects::protocol_for_get_msgs(int user_id)
{
    int code;
    char buffer[BUFFER_SIZE] = {0};

    std::cout << this->id << " - Msg-s request" << std::endl; 
            
    char *response = new char[10];            
    int msg_num = get_msg_num(this->id,user_id);

    std::cout << "Msg number:" << msg_num << std::endl;

    char *num = new char[10];

    if(msg_num>0)
    {
        sprintf(num, "%d", msg_num);

        put_code(response,0);
        strcat(response,num);
        strcat(response,"_");

        send(this->sock, response, strlen(response), 0);

        delete[] response;

        response = new char[500];
        char** msg_list = new char*[msg_num];

        get_msg_list(msg_list,this->id,user_id);

        for(int i=0;i<msg_num;i++)
        {
            read(this->sock, buffer, BUFFER_SIZE);
            code = get_code(buffer);

            put_code(response,0);
            strcat(response,msg_list[i]);

            send(this->sock, response, strlen(response), 0);
        }

        read(this->sock, buffer, BUFFER_SIZE);
        code = get_code(buffer);

        if(code==0)
        {
            put_code(response,0);
            send(this->sock, response, strlen(response), 0);
            std::cout << "List transfer secsfully..." << std::endl;
        }

        for(int i=0;i<msg_num;i++)
        {
            delete[] msg_list[i];
        }
    }else
    {
        sprintf(num, "%d", msg_num);

        put_code(response,1);
        strcat(response,num);
        strcat(response,"_");

        send(this->sock, response, strlen(response), 0);
    }

    delete[] response;
};

void client_objects::protocol_for_get_msg(int user_id)
{
    char *response = new char[500];  

    std::cout << this->id << " - Msg request" << std::endl; 

    if(get_msg_num(this->id,user_id)>0)
    {
        char* msg = new char[500];

        this->get_last_msg(msg,user_id);

        put_code(response,0);
        strcat(response,msg);

        send(this->sock, response, strlen(response), 0);

        delete[] msg;
    }else
    {
        char* msg = new char[500];

        this->get_last_msg(msg,user_id);

        put_code(response,1);
        strcat(response,msg);

        send(this->sock, response, strlen(response), 0);

        delete[] msg;
    }

    delete[] response;
};

int get_code(char* buffer)
{
    int digit_one = buffer[0] - '0';
    int digit_two = buffer[1] - '0';
    int digit_three = buffer[2] - '0';

    int code = digit_one*100 + digit_two*10 + digit_three;

    return code;
}

void put_code(char* buffer,int n)
{
    int digit_one,digit_two,digit_three;

    digit_one = (n-(n%100));
    
    digit_two = (n-digit_one)-((n-digit_one)%10);
    
    digit_three = n-digit_one-digit_two;
    
    digit_one/=100;
    digit_two/=10;
    
    buffer[0] = digit_one + '0';
    buffer[1] = digit_two + '0';
    buffer[2] = digit_three + '0';
    buffer[3] = '_';
    buffer[4] = 0;
};

void get_msg(char* buffer,void* msg)
{
    char* msg_l = (char*)msg;

    int offset;
    int i=4;

    while(true)
    {
        if(buffer[i]=='_')
        {
            i++;
            break;
        }
        i++;
    }

    offset = i;
    i=0;

    while(true)
    {
        msg_l[i] = buffer[i+offset];

        if(msg_l[i]=='_')
        {
            msg_l[i]=0;
            break;
        }
        i++;
    }
}

void get_username(char* buffer,void* username)
{
    char* username_l = (char*)username;
    for(int i=0;i<32;i++)
    {
        username_l[i]=buffer[i+4];
        if(username_l[i]=='_')
        {
            username_l[i]=0;
            break;
        }
    }
}

int get_password_start(char* buffer)
{
    int offset = 4;

    while(buffer[offset]!='_')
    {
        offset++;
    }

    offset++;

    return offset;
}

void get_password(char* buffer,void* password)
{
    char* password_l = (char*)password;
    int offset = get_password_start(buffer);

    for(int i=0;i<32;i++)
    {
        password_l[i]=buffer[i+offset];
        if(password_l[i]=='_')
        {
            password_l[i]=0;
            break;
        }
    }
}

int get_user_id_after_code(char* buffer)
{
    char id[10];

    int i=4;


    while(true)
    {
        id[i-4] = buffer[i];

        if(buffer[i]=='_')
        {
            id[i-4]=0;
            break;
        }
        i++;
    }

    return atoi(id);
}


bool try_register(void* username,void* password)
{
    char* username_l = (char*)username;
    char* password_l = (char*)password;

    if(create_user(username,password)==true)
    {
        return true;
    }else
    {
        return false;
    }
}

int get_user_num()
{
    int user_num;

    user_num = count_user();

    return user_num;
}

int get_msg_num(int id1,int id2)
{
    int msg_num;

    msg_num = count_msg(id1,id2);
    
    return msg_num;
}

void get_username_list(void* username,int id)
{
    read_and_write_username_id_list(username,id);
}

void get_msg_list(void* msg_list,int id1,int id2)
{
    read_and_write_msg_from_id_list(msg_list,id1,id2);
}