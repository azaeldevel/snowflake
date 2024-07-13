/* Feel free to use this example code in any way
   you see fit (Public Domain) */

#include "server.hh"
//extern Resource root;

MYSQL* create_conection()
{
    const char *server = "localhost";
    const char  *user = "admin";
    const char  *password = "123456"; /* set me first */
    const char  *database = "muposys-dev";

    MYSQL *conn = mysql_init(NULL);

    /* Connect to database */
    if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0))
    {
      fprintf(stderr, "%s\n", mysql_error(conn));
      return NULL;
    }
    if (!mysql_set_character_set(conn, "utf8"))
    {
        //printf("New client character set: %s\n", mysql_character_set_name(conn));
    }

    return conn;
}

bool verify_authentication(MYSQL* conn,const char* autho)
{
    const char* sql = "SELECT id FROM User_Autho WHERE autho = '%s'";
    size_t sql_z = strlen (sql) - 2 + strlen(autho) + 1; //strlen ("%s") = 2
    char* newswl = (char*)malloc(sql_z + 1);
    //printf("autho : %s\n",autho);
    snprintf (newswl,sql_z,sql,autho);
    //printf("SQL : %s\n",sql);
    if (mysql_query(conn, newswl))
    {
        fprintf(stderr, "Original SQL : [%s]\n", newswl);
        fprintf(stderr, "%s\n", mysql_error(conn));
        free(newswl);
        return false;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    auto numrow = mysql_num_rows(res);
    if(numrow == 1)
    {
        return true;
    }
    else
    {
        printf("Original SQL : [%s]\n", newswl);
        printf("Se encontraaron : %llu regitros\n", numrow);
    }


    free(newswl);
    mysql_free_result(res);
    return false;
}


bool verify_authentication(MYSQL* conn,const char* u,const char* p)
{
    const char* sql = "SELECT id FROM User WHERE name = '%s' and pwdtxt = '%s'";
    size_t sql_z = strlen (sql) - 4 + strlen(u) + strlen(p) + 1; //strlen ("%s") * 2 = 4
    char* newswl = (char*)malloc(sql_z + 1);
    snprintf (newswl,sql_z,sql,u,p);
    //printf("SQL : %s\n",sql);
    if (mysql_query(conn, newswl))
    {
        fprintf(stderr, "Original SQL : [%s]\n", newswl);
        fprintf(stderr, "%s\n", mysql_error(conn));
        free(newswl);
        return false;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    auto numrow = mysql_num_rows(res);
    if(numrow == 1)
    {
        return true;
    }
    else
    {
        printf("Original SQL : [%s]\n", newswl);
        printf("Se encontraaron : %llu regitros\n", numrow);
    }


    free(newswl);
    mysql_free_result(res);
    return false;
}
