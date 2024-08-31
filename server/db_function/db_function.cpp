#include "db_function.h"

void delete_db()
{
    const char* filename = "database.db";

    if (std::remove(filename) != 0) {
        std::perror("Error deleting file");
    } else {
        std::cout << "File successfully deleted\n";
    }
}

void create_db()
{

    sqlite3* db;
    sqlite3_stmt* stmt;
    char* errMsg = 0;
    int rc;

    rc = sqlite3_open("database.db", &db);

    const char* createTableSQL = "CREATE TABLE IF NOT EXISTS user (" \
                             "id INTEGER PRIMARY KEY AUTOINCREMENT," \
                             "username TEXT NOT NULL," \
                             "password TEXT NOT NULL);";

    rc = sqlite3_exec(db, createTableSQL, 0, 0, &errMsg);

    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    } else {
        std::cout << "Table created successfully" << std::endl;
    }

    createTableSQL = "CREATE TABLE IF NOT EXISTS messages (" \
                             "id INTEGER PRIMARY KEY AUTOINCREMENT," \
                             "date_time INTEGER NOT NULL," \
                             "to_user INTEGER NOT NULL," \
                             "from_user INTEGER NOT NULL," \
                             "msg TEXT NOT NULL);";

    rc = sqlite3_exec(db, createTableSQL, 0, 0, &errMsg);

    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    } else {
        std::cout << "Table created successfully" << std::endl;
    }

    sqlite3_close(db);
}

bool check_for_user(void* username,void* password)
{
    sqlite3* db;
    sqlite3_stmt* stmt;
    char* errMsg = 0;
    int rc;

    bool ret=false;

    rc = sqlite3_open("database.db", &db);

    char* username_l = (char*)username;
    char* password_l = (char*)password;

    char* querySQL = (char*)malloc(256 * sizeof(char));
    sprintf(querySQL, "SELECT COUNT(*) FROM user WHERE username = '%s' AND password = '%s'", username_l, password_l);

    rc = sqlite3_prepare_v2(db, querySQL, -1, &stmt, 0);

    if (rc != SQLITE_OK) {
        std::cerr << "Failed to fetch data: " << sqlite3_errmsg(db) << std::endl;
    }

    if((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        const unsigned char* text = sqlite3_column_text(stmt, 0);
        if(text[0]=='1')
        {
            ret = true;
        }        
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return ret;
};

int get_user_id_from_db(void* username,void* password)
{
    sqlite3* db;
    sqlite3_stmt* stmt;
    char* errMsg = 0;
    int rc;

    bool ret=false;

    rc = sqlite3_open("database.db", &db);

    char* username_l = (char*)username;
    char* password_l = (char*)password;

    char* querySQL = (char*)malloc(256 * sizeof(char));
    sprintf(querySQL, "SELECT id FROM user WHERE username = '%s' AND password = '%s'", username_l, password_l);

    rc = sqlite3_prepare_v2(db, querySQL, -1, &stmt, 0);

    if (rc != SQLITE_OK) {
        std::cerr << "Failed to fetch data: " << sqlite3_errmsg(db) << std::endl;
    }

    int id_val;
    
    if((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        const unsigned char* id = sqlite3_column_text(stmt, 0);
        id_val = atoi((const char*)id);
    }


    if (rc != SQLITE_DONE) {
        std::cerr << "Execution failed: " << sqlite3_errmsg(db) << std::endl;
    }

    delete querySQL;
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return id_val;
};



bool check_for_username(void* username)
{
    sqlite3* db;
    sqlite3_stmt* stmt;
    char* errMsg = 0;
    int rc;

    bool ret=false;

    rc = sqlite3_open("database.db", &db);

    char* username_l = (char*)username;

    char* querySQL = (char*)malloc(256 * sizeof(char));
    sprintf(querySQL, "SELECT COUNT(*) FROM user WHERE username = '%s'", username_l);

    rc = sqlite3_prepare_v2(db, querySQL, -1, &stmt, 0);

    if (rc != SQLITE_OK) {
        std::cerr << "Failed to fetch data: " << sqlite3_errmsg(db) << std::endl;
    }

    if((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        const unsigned char* text = sqlite3_column_text(stmt, 0);
        if(text[0]=='1')
        {
            ret = true;
        }        
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return ret;
};

bool create_user(void* username,void* password)
{
    if(check_for_username(username))
    {
        return false;
    }

    sqlite3* db;
    
    char* errMsg = 0;
    int rc;

    rc = sqlite3_open("database.db", &db);

    char* username_l = (char*)username;
    char* password_l = (char*)password;

    char* querySQL = (char*)malloc(256 * sizeof(char));
    sprintf(querySQL, "INSERT INTO user (username, password) VALUES ('%s', '%s');", username_l, password_l);

    rc = sqlite3_exec(db, querySQL, 0, 0, &errMsg);

    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    } else {
        std::cout << "Data inserted successfully" << std::endl;
    }
    
    sqlite3_close(db);

    return true;
}


int count_user()
{
    int user_num;
    sqlite3* db;
    sqlite3_stmt* stmt;
    char* errMsg = 0;
    int rc;

    rc = sqlite3_open("database.db", &db);

    char* querySQL = (char*)malloc(256 * sizeof(char));
    sprintf(querySQL, "SELECT COUNT(*) FROM user");

    rc = sqlite3_prepare_v2(db, querySQL, -1, &stmt, 0);

    if (rc != SQLITE_OK) {
        std::cerr << "Failed to fetch data: " << sqlite3_errmsg(db) << std::endl;
    }

    if((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        const unsigned char* text = sqlite3_column_text(stmt, 0);

        const char* text_str = reinterpret_cast<const char*>(text);
        
        user_num = atoi(text_str);
    }else
    {
        user_num = 0;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);   

    return user_num;
};

int count_msg(int id1,int id2)
{
    int msg_num;
    sqlite3* db;
    sqlite3_stmt* stmt;
    char* errMsg = 0;
    int rc;

    rc = sqlite3_open("database.db", &db);

    char* querySQL = (char*)malloc(256 * sizeof(char));

    sprintf(querySQL, "SELECT COUNT(*) FROM messages WHERE (to_user = %d OR from_user = %d) AND (to_user = %d OR from_user = %d)", id1, id1, id2, id2);

    rc = sqlite3_prepare_v2(db, querySQL, -1, &stmt, 0);

    if (rc != SQLITE_OK) {
        std::cerr << "Failed to fetch data: " << sqlite3_errmsg(db) << std::endl;
    }

    if((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        const unsigned char* text = sqlite3_column_text(stmt, 0);

        const char* text_str = reinterpret_cast<const char*>(text);
        
        msg_num = atoi(text_str);

    }else
    {
        msg_num = 0;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);   

    return msg_num;
}

void read_and_write_username_list(void* username)
{
    sqlite3* db;
    sqlite3_stmt* stmt;
    int rc;

    char **username_l = (char **)username;

    rc = sqlite3_open("database.db", &db);

    char* querySQL = (char*)malloc(256 * sizeof(char));
    sprintf(querySQL, "SELECT username FROM user");

    rc = sqlite3_prepare_v2(db, querySQL, -1, &stmt, 0);

    if (rc != SQLITE_OK) {
        std::cerr << "Failed to fetch data: " << sqlite3_errmsg(db) << std::endl;
    }

    int i=0;

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        const unsigned char* text = sqlite3_column_text(stmt, 0);
        strcpy(username_l[i],reinterpret_cast<const char*>(text));
        i++;
    }

    if (rc != SQLITE_DONE) {
        std::cerr << "Execution failed: " << sqlite3_errmsg(db) << std::endl;
    }

    delete querySQL;
    sqlite3_finalize(stmt);
    sqlite3_close(db);   
};

void read_and_write_username_id_list(void* username,int id)
{
    sqlite3* db;
    sqlite3_stmt* stmt;
    int rc;

    char **username_l = (char **)username;

    rc = sqlite3_open("database.db", &db);

    char* querySQL = (char*)malloc(256 * sizeof(char));
    sprintf(querySQL, "SELECT id, username FROM user WHERE NOT (id=%d)",id);

    rc = sqlite3_prepare_v2(db, querySQL, -1, &stmt, 0);

    if (rc != SQLITE_OK) {
        std::cerr << "Failed to fetch data: " << sqlite3_errmsg(db) << std::endl;
    }

    int i=0;
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        const unsigned char* id = sqlite3_column_text(stmt, 0);
        const unsigned char* username = sqlite3_column_text(stmt, 1);
        
        strcpy(username_l[i],reinterpret_cast<const char*>(username));
        strcat(username_l[i],"_");
        strcat(username_l[i],reinterpret_cast<const char*>(id));
        strcat(username_l[i],"_");

        i++;
    }

    if (rc != SQLITE_DONE) {
        std::cerr << "Execution failed: " << sqlite3_errmsg(db) << std::endl;
    }

    delete querySQL;
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}



void get_last_msg_from_db(void* msg,int id1,int id2)
{
    sqlite3* db;
    sqlite3_stmt* stmt;
    int rc;

    rc = sqlite3_open("database.db", &db);

    char *msg_l = (char *)msg;

    char* querySQL = (char*)malloc(256 * sizeof(char));

    sprintf(querySQL, "SELECT msg, to_user FROM messages WHERE (to_user = %d OR from_user = %d) AND (to_user = %d OR from_user = %d) ORDER BY date_time DESC", id1, id1, id2, id2);

    rc = sqlite3_prepare_v2(db, querySQL, -1, &stmt, 0);

    if (rc != SQLITE_OK) {
        std::cerr << "Failed to fetch data: " << sqlite3_errmsg(db) << std::endl;
    }

    if((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        const unsigned char* msg_text = sqlite3_column_text(stmt, 0);
        const unsigned char* to_user = sqlite3_column_text(stmt, 1);

        strcpy(msg_l,reinterpret_cast<const char*>(msg_text));
        strcat(msg_l,"_");
        strcat(msg_l,reinterpret_cast<const char*>(to_user));
        strcat(msg_l,"_");
    }
    
    if (rc != SQLITE_DONE) {
        std::cerr << "Execution failed: " << sqlite3_errmsg(db) << std::endl;
    }

    delete querySQL;
    sqlite3_finalize(stmt);
    sqlite3_close(db);

}

void read_and_write_msg_from_id_list(void* msg_list,int id1,int id2)
{
    sqlite3* db;
    sqlite3_stmt* stmt;
    int rc;

    char **msg_l = (char **)msg_list;


    rc = sqlite3_open("database.db", &db);

    char* querySQL = (char*)malloc(256 * sizeof(char));
    sprintf(querySQL, "SELECT msg, to_user FROM messages WHERE (to_user = %d OR from_user = %d) AND (to_user = %d OR from_user = %d) ORDER BY date_time ASC", id1, id1, id2, id2);

    rc = sqlite3_prepare_v2(db, querySQL, -1, &stmt, 0);

    if (rc != SQLITE_OK) {
        std::cerr << "Failed to fetch data: " << sqlite3_errmsg(db) << std::endl;
    }

    int i=0;

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        
        const unsigned char* msg_text = sqlite3_column_text(stmt, 0);
        const unsigned char* to_user = sqlite3_column_text(stmt, 1);
        
        msg_l[i] = new char[strlen(reinterpret_cast<const char*>(msg_text))+strlen(reinterpret_cast<const char*>(to_user))+2];

        strcpy(msg_l[i],reinterpret_cast<const char*>(msg_text));
        strcat(msg_l[i],"_");
        strcat(msg_l[i],reinterpret_cast<const char*>(to_user));
        strcat(msg_l[i],"_");

        i++;
    }

    if (rc != SQLITE_DONE) {
        std::cerr << "Execution failed: " << sqlite3_errmsg(db) << std::endl;
    }

    delete querySQL;
    sqlite3_finalize(stmt);
    sqlite3_close(db);

}

bool write_msg(int from_id,int to_id,void* msg)
{
    time_t current_time;
    time(&current_time);

    char* msg_l = (char*)msg;
    
    sqlite3* db;
    sqlite3_stmt* stmt;
    char* errMsg = 0;
    int rc;

    rc = sqlite3_open("database.db", &db);

    char* querySQL = (char*)malloc(1024 * sizeof(char));
    
    sprintf(querySQL, "INSERT INTO messages (date_time, to_user, from_user, msg) VALUES ('%ld','%d','%d','%s');", current_time, to_id, from_id, msg_l);
    

    rc = sqlite3_exec(db, querySQL, 0, 0, &errMsg);

    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    } else {
        std::cout << "Data inserted successfully" << std::endl;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return true;
}