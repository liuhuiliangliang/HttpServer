#include "DataBase.h"
Database::Database()
{
    init();
}

Database::~Database()
{
    
}

void Database::init()
{
    conn = mysql_init(NULL);
}

void Database::connect() 
{
    // 连接数据库
    if (!mysql_real_connect(conn, server, user, password, database, 9006, NULL, 0))
    {
        std::cerr << "连接错误: " << mysql_error(conn) << std::endl;
    }
    else
    {
        std::cout << "连接成功！" << std::endl;
    }
}

void Database::disconnect() 
{
    // 断开数据库连接
    mysql_close(conn);
}

void Database::executeQuery(const char* query)
{
    // 执行数据库查询
    if (!mysql_query(conn,query))
    {
        std::cerr << "查询错误: " << mysql_error(conn) << std::endl;
    }
    else
    {
        res = mysql_use_result(conn);
        while ((row = mysql_fetch_row(res)) != NULL) {
            for (int i = 0; i < mysql_num_fields(res); i++) {
                std::cout << row[i] << " ";
            }
            std::cout << std::endl;
        }
        mysql_free_result(res);
    }
    
}

void Database::insert(const char* query)
{
    if (mysql_query(conn, "INSERT INTO your_table (column1, column2) VALUES ('value1', 'value2')")) 
    {
        std::cerr << "插入错误: " << mysql_error(conn) << std::endl;
    } else {
        std::cout << "插入成功！" << std::endl;
    }
}

void Database::del(const char* query)
{
    if (mysql_query(conn, "DELETE FROM your_table WHERE condition")) {
        std::cerr << "删除错误: " << mysql_error(conn) << std::endl;
    } else {
        std::cout << "删除成功！" << std::endl;
    }
}

void Database::update(const char* query)
{
    if (mysql_query(conn, "UPDATE your_table SET column1='new_value' WHERE condition")) {
        std::cerr << "更新错误: " << mysql_error(conn) << std::endl;
    } else {
        std::cout << "更新成功！" << std::endl;
    }
}