#include "communication_object.h"

bool communication::start_communication()
{
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cout << "Socket creation error" << std::endl;
        return false;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0) {
        std::cout << "Invalid address / Address not supported" << std::endl;
        return false;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cout << "Connection Failed" << std::endl;
        return false;
    }

    return true;
};

bool communication::try_registration(char username[32],char password[32])
{
    char buffer[BUFFER_SIZE] = {0};

    int size_of_msg = 6+strlen(username)+strlen(password);
    char *message = (char*)malloc(size_of_msg * sizeof(char));
            
    put_code(message,CODE_FOR_REGISTER);
    append_username_password(message,username,password);

    send(sock, message, strlen(message), 0);

    int valread = read(sock, buffer, BUFFER_SIZE);
    int code = get_code(buffer);

    delete message;

    if(code == 0)
    {
        return true;
    }else
    {
        return false;
    }
};

bool communication::try_login(char username[32],char password[32])
{
    char buffer[BUFFER_SIZE] = {0};

    int size_of_msg = 6+strlen(username)+strlen(password);
    char *message = (char*)malloc(size_of_msg * sizeof(char));

    put_code(message,CODE_FOR_LOGIN);
    append_username_password(message,username,password);

    send(sock, message, strlen(message), 0);

    int valread = read(sock, buffer, BUFFER_SIZE);
    int code = get_code(buffer);

    delete message;

    if(code == 0)
    {
        return true;
    }else
    {
        return false;
    }
};

bool communication::end_communication()
{
    char buffer[BUFFER_SIZE] = {0};
    char *message = (char*)malloc(5 * sizeof(char));;

    put_code(message,CODE_FOR_END);

    send(sock, message, strlen(message), 0);

    int valread = read(sock, buffer, BUFFER_SIZE);
    int code = get_code(buffer);

    delete message;
    close(sock);

    if(code == 0)
    {
        return true;
    }else
    {
        return false;
    }
};

bool communication::try_get_users()
{
    char buffer[BUFFER_SIZE] = {0};            
    char *message = (char*)malloc(5 * sizeof(char));
            
    put_code(message,CODE_FOR_USER_LIST);
    send(sock, message, strlen(message), 0);

    int valread = read(sock, buffer, BUFFER_SIZE);
    
    int n = get_num_after_code(buffer);
    
    user_list = (User *)malloc(n * sizeof(User));
    number_of_user_in_list = n;

    put_code(message,NO_CODE);
    for(int i=0;i<n;i++)
    {
        int id;
        char username[32] = {0};

        send(sock, message, strlen(message), 0);
        int valread = read(sock, buffer, BUFFER_SIZE);
                
        get_username(buffer,username);
        id = get_id_after_username(buffer);

        user_list[i].id = id;
        strcpy(user_list[i].username,username);
    }

    put_code(message,NO_CODE);
    send(sock, message, strlen(message), 0);

    valread = read(sock, buffer, BUFFER_SIZE);
    int code = get_code(buffer);

    delete message;

    if(code==0)
    {
        return true;
    }else
    {
        return false;
    }
}

void communication::print_user_list()
{
    for(int i=0;i<number_of_user_in_list;i++)
    {
        std::cout << user_list[i].id << " - " << user_list[i].username << std::endl;
    }
}

int communication::get_id_from_username(char username[32])
{
    for(int i=0;i<number_of_user_in_list;i++)
    {
        if(strcmp(username,user_list[i].username)==0)
        {
            //std::cout << user_list[i].id << " " << user_list[i].username << std::endl;
            //break;
            return user_list[i].id;
        }
    }
    return 0;
};

bool communication::try_to_send_msg(int id,char msg[500])
{
    char buffer[BUFFER_SIZE] = {0};
    char *message = (char*)malloc(BUFFER_SIZE * sizeof(char));

    put_code(message,CODE_FOR_MSG_SEND);
    put_id_msg(message,id,msg);


    /*
    while(getting_msg)
    {
        std::cout << "*" << std::endl;
    }
    */

    posting_msg=true;

    send(sock, message, strlen(message), 0);
    read(sock, buffer, BUFFER_SIZE);
    int code = get_code(buffer);

    posting_msg=false;

    delete[] message;

    if(code == 0)
    {
        return true;
    }else
    {
        return false;
    }
}

bool communication::try_to_get_msgs(int id)
{
    char buffer[BUFFER_SIZE] = {0};
    char *message = (char*)malloc(12 * sizeof(char));

    put_code(message,CODE_FOR_MSGS);
    append_id_after_code(message,id);

    send(sock, message, strlen(message), 0);
    int valread = read(sock, buffer, BUFFER_SIZE);

    if(get_code(buffer)==1)
    {
        return false;
    }

    number_of_msg = get_num_after_code(buffer);
    //std::cout << "Msg num:" << number_of_msg << std::endl;

    msgs_list = (Msg *)malloc(number_of_msg * sizeof(Msg));

    int to_user;
    int msg_len;
    char* msg;

    put_code(message,0);
    for(int i=0;i<number_of_msg;i++)
    {
        send(sock, message, strlen(message), 0);
        int valread = read(sock, buffer, BUFFER_SIZE);

        msg_len = get_msg_len(buffer);
        msgs_list[i].text = (char*)malloc(msg_len+1 * sizeof(char));

        msgs_list[i].to_user_id = get_id_after_msg(buffer,msg_len);

        get_msg(buffer,msgs_list[i].text);
    }

    last_msg.text = (char*)malloc(msg_len+1 * sizeof(char));
    last_msg.to_user_id = get_id_after_msg(buffer,msg_len);
    get_msg(buffer,last_msg.text);

    put_code(message,0);
    send(sock, message, strlen(message), 0);

    valread = read(sock, buffer, BUFFER_SIZE);
    int code = get_code(buffer);

    delete message;

    if(code==0)
    {
        return true;
    }else
    {
        return false;
    }
};


bool communication::try_to_get_msg(int id)
{

    char buffer[BUFFER_SIZE] = {0};
    char *message = (char*)malloc(12 * sizeof(char));

    put_code(message,CODE_FOR_MSG);
    append_id_after_code(message,id);

    send(sock, message, strlen(message), 0);
    int valread = read(sock, buffer, BUFFER_SIZE);

    if(get_code(buffer)==1)
    {
        return false;
    }

    getting_msg = false;

    int n = get_msg_len(buffer);
            
    int l_id=get_id_after_msg(buffer,n);
    char* l_text = (char*)malloc(n+1 * sizeof(char));
    get_msg(buffer,l_text);

    if(l_id==last_msg.to_user_id && (strcmp(l_text,last_msg.text)==0))
    {
        return false;
    }else
    {
        last_msg.to_user_id = l_id;
        free(last_msg.text);

        last_msg.text = (char*)malloc(n+1 * sizeof(char));
        get_msg(buffer,last_msg.text);

        return true;
    }
};



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

int get_code(char* buffer)
{
    int digit_one = buffer[0] - '0';
    int digit_two = buffer[1] - '0';
    int digit_three = buffer[2] - '0';

    int code = digit_one*100 + digit_two*10 + digit_three;

    return code;
};

void append_username_password(char* buffer,char username[32],char password[32])
{
    strcat(buffer,username);
    strcat(buffer,"_");
    strcat(buffer,password);
    strcat(buffer,"_");
};

int get_num_after_code(char* buffer)
{
    char num[32];

    for(int i=0;i<32;i++)
    {
        num[i] = buffer[i+4];

        if(num[i]=='_')
        {
            num[i]=0;
            break;
        }
    }

    return atoi(num);
};

int get_id_after_username(char* buffer)
{
    char number[8];
    int dc=0,i=0,j=0;

    while(1)
    {
        if(dc==2)
        {
            number[j]=buffer[i];
            j++;
        }

        if(dc==3)
        {
            number[j-1]=0;
            break;
        }

        if(buffer[i]=='_')
        {
            dc++;
        }
   
        i++;
    }

    return atoi(number);
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

void put_id_msg(char* buffer,int id,char* msg)
{
    char num[10];
    sprintf(num, "%d", id);
    strcat(buffer,num);
    strcat(buffer,"_");

    strcat(buffer,msg);
    strcat(buffer,"_");
}

void append_id_after_code(char* buffer,int id)
{
    char num[10];

    sprintf(num, "%d", id);

    strcat(buffer,num);
    strcat(buffer,"_");
}


int get_msg_len(char* buffer)
{
    int msg_len=0;
    int i=4;

    while(true)
    {
        if(buffer[i]=='_')
        {
            i++;
            break;
        }else
        {
            msg_len++;
            i++;
        }
    }
    
    return msg_len;
}

int get_id_after_msg(char* buffer,int msg_len)
{
    char num[10];
    int j=0;
    int i=4+msg_len+1;

    while(true)
    {
        if(buffer[i]=='_')
        {
            i++;
            num[j] = 0;
            break;
        }else
        {
            num[j] = buffer[i];
            i++;
            j++;
        }
    }

    return atoi(num);
}

void get_msg(char* buffer,char* msg)
{
    int i=4;
    
    while(true)
    {
        if(buffer[i]=='_')
        {
            msg[i-4] = 0;
            i++;
            break;
        }else
        {
            msg[i-4] = buffer[i];
            i++;
        }
    }
}
