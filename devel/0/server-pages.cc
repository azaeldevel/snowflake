
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
*\brief Para guardar handler de creacion de pagina
**/


#include "server.hh"


MHD_Result secret_page (struct MHD_Connection *connection)
{
  enum MHD_Result ret;
  struct MHD_Response *response;
  const char *page = "<html><body>A secret.</body></html>";

  response =
    MHD_create_response_from_buffer (strlen (page), (void *) page,
                                     MHD_RESPMEM_PERSISTENT);
  if (! response)
    return MHD_NO;

  ret = MHD_queue_response (connection, MHD_HTTP_OK, response);
  MHD_destroy_response (response);

  return ret;
}

MHD_Result error_page (MHD_Connection *connection)
{
    enum MHD_Result ret;
    MHD_Response *response;
    const char *page = "<html><body>Error</body></html>";
    response = MHD_create_response_from_buffer (strlen (page), (void *) page, MHD_RESPMEM_PERSISTENT);
    if (! response) return MHD_NO;
    ret = MHD_queue_response (connection, MHD_HTTP_BAD_REQUEST, response);
    MHD_destroy_response (response);

    return ret;
}

MHD_Result answer_connection_http (void *cls, struct MHD_Connection *connection,
                      const char *url, const char *method,
                      const char *version, const char *upload_data,
                      size_t *upload_data_size, void **con_cls)
{
    //enum MHD_Result result;
    (void) cls;               /* Unused. Silent compiler warning. */
    (void) url;               /* Unused. Silent compiler warning. */
    (void) version;           /* Unused. Silent compiler warning. */
    (void) upload_data;       /* Unused. Silent compiler warning. */
    (void) upload_data_size;  /* Unused. Silent compiler warning. */
    //printf("Conecion: %llu\n" , (void*)connection);

    //if (0 != strcmp (method, "GET"))
    //return MHD_NO;
    //printf("URL : '%s'\n",url);
    if(NULL == *con_cls)
    {
        *con_cls = connection;
        return MHD_YES;
    }
    //printf("cls -> '%llu'\n",cls);
    //printf("con_cls -> '%llu'\n",con_cls);
    void** params_extra = (void**)cls;
    //printf("temp -> '%llu'\n",temp);
    //printf("temp[0] -> '%llu'\n",(void*)temp[0]);
    Server* serv = (Server*)params_extra[0];

    //printf("URL : %s\n",url);
    Resource* actual = serv->root.find(url);
    if(not actual) return unknow_resource(connection);
    //printf("no SSL..\n");
    if(actual->identify)
    {
        if (is_authenticated_http(connection))
        {
            //printf("\tautorizado..\n");
            return actual->reply(connection);
        }
        else
        {
            //printf("\tno autorizado..\n");
            return unauthorized_access(connection);
        }
    }
    else
    {
        //printf("no autorizacion requerida..\n");
        return actual->reply(connection);
    }

    return default_page(connection);
}



MHD_Result answer_connection_https (void *cls, struct MHD_Connection *connection,
                      const char *url, const char *method,
                      const char *version, const char *upload_data,
                      size_t *upload_data_size, void **con_cls)
{
    //enum MHD_Result result;
    (void) cls;               /* Unused. Silent compiler warning. */
    (void) url;               /* Unused. Silent compiler warning. */
    (void) version;           /* Unused. Silent compiler warning. */
    (void) upload_data;       /* Unused. Silent compiler warning. */
    (void) upload_data_size;  /* Unused. Silent compiler warning. */
    //printf("Conexion: %llu\n" , (void*)connection);

    //if(0 != strcmp (method, "GET")) return MHD_NO;
    //printf("URL : '%s'\n",url);
    /*if (NULL == *con_cls)
    {
        *con_cls = connection;
        return MHD_YES;
    }*/
    //printf("cls -> '%llu'\n",cls);
    //printf("con_cls -> '%llu'\n",con_cls);
    void** params_extra = (void**)cls;
    //printf("cls -> '%llu'\n",params_extra);
    //printf("cls[0] -> '%llu'\n",(void*)params_extra[0]);
    Server* serv = (Server*)params_extra[0];
    Databox box;

    MHD_get_connection_values(connection, serv->kind, iterator,(void*)&box);
    //printf("HEADER size -> '%llu'\n",box.HEADER.size());
    //printf("COOKIES size -> '%llu'\n",box.COOKIES.size());
    //printf("POST size -> '%llu'\n",box.POST.size());
    //printf("GET size -> '%llu'\n",box.GET.size());
    //printf("FOOTER size -> '%llu'\n",box.FOOTER.size());

    //printf("URL : %s\n",url);
    Resource* actual = serv->root.find(url);
    if(not actual) return unknow_resource(connection);
    //printf("SSL detected..\n");
    if(actual->identify)
    {
        //printf("autorizacion requerida..\n");
        if (is_authenticated_https(connection))
        {
            //printf("\tautorizado..\n");
            return actual->reply(connection);
        }
        else
        {
            //printf("\tno autorizado..\n");
            return unauthorized_access(connection);
        }
    }
    else
    {
        //printf("no autorizacion requerida..\n");
        return actual->reply(connection);
    }

    return default_page(connection);
}


MHD_Result default_page(MHD_Connection* connection)
{
    MHD_Response *response;
    MHD_Result result;
    const char *page = "<html><body>Welcome!!!</body></html>";
    const MHD_ConnectionInfo* info = MHD_get_connection_info(connection,MHD_CONNECTION_INFO_PROTOCOL);
    if(info)
    {
        response = MHD_create_response_from_buffer (strlen (page), (void *) page, MHD_RESPMEM_PERSISTENT);
        result = MHD_queue_response (connection, MHD_HTTP_OK, response);
    }
    else
    {
        response = MHD_create_response_from_buffer (strlen (page), (void *) page, MHD_RESPMEM_PERSISTENT);
        result = MHD_queue_response (connection, MHD_HTTP_OK, response);
    }



    MHD_destroy_response (response);
    return result;
}

MHD_Result unauthorized_access (MHD_Connection *connection)
{
    struct MHD_Response *response;
    MHD_Result result;
    const char *page = "<html><body><h1>401 Unauthorized Access</h1></body></html>";

    response = MHD_create_response_from_buffer (strlen (page), (void *) page, MHD_RESPMEM_PERSISTENT);
    result = MHD_queue_response (connection, MHD_HTTP_OK, response);

    MHD_destroy_response (response);
    return result;
}
MHD_Result unknow_resource (MHD_Connection *connection)
{
    struct MHD_Response *response;
    MHD_Result result;
    const char *page = "<html><body><h1>404 Not Found Resource</h1></body></html>";

    response = MHD_create_response_from_buffer (strlen (page), (void *) page, MHD_RESPMEM_PERSISTENT);
    result = MHD_queue_response (connection, MHD_HTTP_OK, response);

    MHD_destroy_response (response);
    return result;
}
MHD_Result default_logout(MHD_Connection* connection)
{
    struct MHD_Response *response;
    MHD_Result result;
    const char *page = "<html><body></body></html>";

    printf("Logout...\n");
    const MHD_ConnectionInfo* info = MHD_get_connection_info(connection,MHD_CONNECTION_INFO_PROTOCOL);
    response = MHD_create_response_from_buffer (strlen (page), (void *) page, MHD_RESPMEM_PERSISTENT);
    if(info)
    {
        const char *headervalue = MHD_lookup_connection_value (connection, MHD_HEADER_KIND,"WWW-Authenticate");
        if(headervalue)
        {
            //MHD_set_connection_value(connection, MHD_HEADER_KIND,"WWW-Authenticate","#");
            MHD_del_response_header(response,"WWW-Authenticate","#");
        }
    }
    MHD_add_response_header (response, "Location", "/");
    result = MHD_queue_response (connection, MHD_HTTP_OK, response);

    MHD_destroy_response (response);
    return result;
}

MHD_Result default_loging(MHD_Connection* connection)
{
    const MHD_ConnectionInfo* info = MHD_get_connection_info(connection,MHD_CONNECTION_INFO_PROTOCOL);
    if(info)
    {
        if (not is_authenticated_https(connection))
        {
            Resource logout("logout",default_logout,true);
            root->branch.insert(std::pair(logout.name_string,logout));
            return ask_for_authentication(connection,REALM);
        }
        else
        {
            return default_page(connection);
        }
    }
    else
    {
        if (not is_authenticated_http(connection))
        {
            Resource logout("logout",default_logout,true);
            root->branch.insert(std::pair(logout.name_string,logout));
            return ask_for_authentication(connection);
        }
        else
        {
            return default_page(connection);
        }
    }

    return unauthorized_access(connection);
}

MHD_Result favicon_request(MHD_Connection *connection)
{
    //printf("\tfavicon imitido\n");
    return MHD_NO;
}
