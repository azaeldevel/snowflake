/* Feel free to use this example code in any way
   you see fit (Public Domain) */

#include "ListProducts.h"

MYSQL* create_conection()
{
    const char *server = "localhost";
    const char  *user = "admin";
    const char  *password = "123456"; /* set me first */
    const char  *database = "wordpress-devel-6";

    MYSQL *conn = mysql_init(NULL);

    /* Connect to database */
    if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0))
    {
      fprintf(stderr, "%s\n", mysql_error(conn));
      return NULL;
    }

    return conn;
}

bool consumer_verifing(MYSQL* conn,const char* k,const char* s)
{
    #warning "No se esta considerando 'costumer key'"
    //const char* sql = "select consumer_key,consumer_secret FROM wp_woocommerce_api_keys WHERE consumer_key = SHA2(CONCAT('%s'), 256) and consumer_secret ='%s'";
    //size_t sql_z = strlen (sql) - 4 + strlen(k) + strlen(s) + 1; //strlen ("%s") * 2 = 4
    const char* sql = "select consumer_key,consumer_secret FROM wp_woocommerce_api_keys WHERE consumer_secret ='%s'";
    size_t sql_z = strlen (sql) - 2 + strlen(s) + 1; //strlen ("%s") => 2
    char* newswl = (char*)malloc(sql_z + 1);
    //printf("S64 : %s\n",string_to_base64(k));
    snprintf (newswl,sql_z,sql,s);
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
