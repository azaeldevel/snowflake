
/**
 *
 *  This file is part of snowflake.
 *  snowflake is C/CC++ Library to create a Web Server.
 *  Copyright (C) 2024  Azael Reyes
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * */

 /**
*\brief Para guardar funcion de control de seccion y seguridad
**/


#include "server.hh"


char * string_to_base64 (const char *message)
{
  const char *lookup =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  unsigned long l;
  size_t i;
  char *tmp;
  size_t length = strlen (message);

  tmp = (char*)malloc (length * 2 + 1);
  if (NULL == tmp)
    return NULL;
  tmp[0] = 0;
  for (i = 0; i < length; i += 3)
  {
    l = (((unsigned long) message[i]) << 16)
        | (((i + 1) < length) ? (((unsigned long) message[i + 1]) << 8) : 0)
        | (((i + 2) < length) ? ((unsigned long) message[i + 2]) : 0);


    strncat (tmp, &lookup[(l >> 18) & 0x3F], 1);
    strncat (tmp, &lookup[(l >> 12) & 0x3F], 1);

    if (i + 1 < length)
      strncat (tmp, &lookup[(l >> 6) & 0x3F], 1);
    if (i + 2 < length)
      strncat (tmp, &lookup[l & 0x3F], 1);
  }

  if (length % 3)
    strncat (tmp, "===", 3 - length % 3);

  return tmp;
}

MHD_Result ask_for_authentication (MHD_Connection *connection, const char *realm)
{
    MHD_Result ret;
    MHD_Response *response;
    char *headervalue;
    size_t slen;
    const char *strbase = "Basic realm=";

    response = MHD_create_response_from_buffer (0, NULL, MHD_RESPMEM_PERSISTENT);
    if (!response) return MHD_NO;

    slen = strlen (strbase) + strlen (realm) + 1;
    if (NULL == (headervalue = (char*)malloc (slen))) return MHD_NO;
    snprintf (headervalue,
            slen,
            "%s%s",
            strbase,
            realm);
    ret = MHD_add_response_header (response,"WWW-Authenticate", headervalue);
    free (headervalue);
    if (! ret)
    {
        MHD_destroy_response (response);
        return MHD_NO;
    }

    ret = MHD_queue_response (connection, MHD_HTTP_UNAUTHORIZED, response);
    MHD_destroy_response (response);

    return ret;
}
MHD_Result ask_for_authentication (struct MHD_Connection *connection)
{
    char *user;
    char *pass;
    pass = NULL;
    user = MHD_basic_auth_get_username_password (connection,&pass);
    if(user)
    {
        MYSQL* conn = create_conection();
        if(verify_authentication(conn,user,pass))
        {
            mysql_close(conn);
            return default_page(connection);
        }
    }

    MHD_Result result;
    MHD_Response *response;
    const char *page = "<html><body><h1>401 Unauthorized Access</h1></body></html>";
    response =  MHD_create_response_from_buffer (strlen (page), (void *) page,MHD_RESPMEM_PERSISTENT);
    result = MHD_queue_basic_auth_fail_response (connection, "my realm", response);
    MHD_destroy_response (response);
    return result;
}

bool is_authenticated_https (struct MHD_Connection *connection)
{
    //printf("\tEl usuario esta autentificado?\n");
    const char *headervalue;
    const char *strbase = "Basic ";

    headervalue = MHD_lookup_connection_value (connection, MHD_HEADER_KIND,"Authorization");
    if (NULL == headervalue) return false;
    if (0 != strncmp (headervalue, strbase, strlen (strbase))) return false;
    //printf("Header Value : %s\n", headervalue);
    auto conn = create_conection();
    if(verify_authentication(conn,headervalue))
    {
        //printf("\t\tSi\n");
        mysql_close(conn);
        //printf("User : %s",MHD_digest_auth_get_username(connection));
        return true;
    }

    //printf("\t\tNo\n");
    mysql_close(conn);
    return false;
}
bool is_authenticated_http (struct MHD_Connection *connection)
{
    //printf("\tEl usuario esta autentificado?\n");
    bool check;
    char * pass = NULL;
    char *user = MHD_basic_auth_get_username_password (connection,&pass);
    if(user and pass)
    {
        //printf("\t\tUser : '%s'\n\tPassword : '%s'\n",user,pass);
        auto mysql = create_conection();
        check = verify_authentication(mysql,user,pass);
        //if(check) printf("\t\tSi\n");
    }
    else
    {
        check = false;
        //printf("\t\tNo\n");
    }
    if (user) MHD_free (user);
    if (pass) MHD_free (pass);

    return check;
}
