#ifndef DB_FUNCTION_H
#define DB_FUNCTION_H

#include <iostream>
#include <cstring>
#include <cstdio>
#include <sqlite3.h>

void delete_db();
void create_db();
bool check_for_user(void* username,void* password);
int get_user_id_from_db(void* username,void* password);
bool check_for_username(void* username);
bool create_user(void* username,void* password);
int count_user();
int count_msg(int id1,int id2);
void read_and_write_username_list(void* username);
void read_and_write_username_id_list(void* username,int id);
void get_last_msg_from_db(void* msg,int id1,int id2);
void read_and_write_msg_from_id_list(void* msg_list,int id1,int id2);
bool write_msg(int from_id,int to_id,void* msg);

#endif