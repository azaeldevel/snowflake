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



MHD_Result answer_to_connection_https (void *cls, struct MHD_Connection *connection,
                      const char *url, const char *method,
                      const char *version, const char *upload_data,
                      size_t *upload_data_size, void **con_cls)
{
    (void) cls;               /* Unused. Silent compiler warning. */
    (void) url;               /* Unused. Silent compiler warning. */
    (void) version;           /* Unused. Silent compiler warning. */
    (void) upload_data;       /* Unused. Silent compiler warning. */
    (void) upload_data_size;  /* Unused. Silent compiler warning. */

    //printf("URL : %s\n",url);
    const char* next = next_resource(url,url);
    //printf("URL Next : %s\n",next);
    //Resource* actual = &root;
    //actual->branch.find();

    if (0 != strcmp (method, "GET")) return MHD_NO;
    if (NULL == *con_cls)
    {
        *con_cls = connection;
        return MHD_YES;
    }

    //const MHD_ConnectionInfo* info = MHD_get_connection_info(connection,MHD_CONNECTION_INFO_PROTOCOL);
    //printf("Protocol : %i\n",info->protocol);


    if (! is_authenticated_https (connection)) return ask_for_authentication (connection, REALM);

    return secret_page (connection);
}


MHD_Result answer_to_connection_http (void *cls, struct MHD_Connection *connection,
                      const char *url, const char *method,
                      const char *version, const char *upload_data,
                      size_t *upload_data_size, void **con_cls)
{
    enum MHD_Result ret;
    struct MHD_Response *response;
    (void) cls;               /* Unused. Silent compiler warning. */
    (void) url;               /* Unused. Silent compiler warning. */
    (void) version;           /* Unused. Silent compiler warning. */
    (void) upload_data;       /* Unused. Silent compiler warning. */
    (void) upload_data_size;  /* Unused. Silent compiler warning. */

    //printf("URL : %s\n",url);
    const char* next = next_resource(url,url);
    if(next)
    {
        printf("URL Next : %s\n",next);
        Resource* actual = &root;
        printf("Map size : %llu\n",actual->branch.size());
        for(auto const& r : actual->branch)
        {
            printf("\tkey : %s\n",r.first.c_str());
            printf("\tvalue : %s\n",r.second.name_string.c_str());
        }
        auto itactual = actual->branch.find(next);
        if(itactual != actual->branch.end())
        {
            actual = &(*itactual).second;
        }
        else
        {
            actual = NULL;
            return error_page(connection);
        }
    }

    //const MHD_ConnectionInfo* info = MHD_get_connection_info(connection,MHD_CONNECTION_INFO_PROTOCOL);
    //printf("Protocol : %i\n",info->protocol);

    if (0 != strcmp (method, "GET")) return MHD_NO;
    if (NULL == *con_cls)
    {
        *con_cls = connection;
        return MHD_YES;
    }

    if (is_authenticated_http(connection))
    {
        const char *page = "<html><body>Go away.</body></html>";
        response = MHD_create_response_from_buffer (strlen (page), (void *) page, MHD_RESPMEM_PERSISTENT);
        ret = MHD_queue_basic_auth_fail_response (connection, "my realm", response);
    }
    else
    {
        const char *page = "<html><body>A secret.</body></html>";
        response = MHD_create_response_from_buffer (strlen (page), (void *) page, MHD_RESPMEM_PERSISTENT);
        ret = MHD_queue_response (connection, MHD_HTTP_OK, response);
    }

    MHD_destroy_response (response);
    return ret;
}


MHD_Result answer_connection (void *cls, struct MHD_Connection *connection,
                      const char *url, const char *method,
                      const char *version, const char *upload_data,
                      size_t *upload_data_size, void **con_cls)
{
    enum MHD_Result result;
    (void) cls;               /* Unused. Silent compiler warning. */
    (void) url;               /* Unused. Silent compiler warning. */
    (void) version;           /* Unused. Silent compiler warning. */
    (void) upload_data;       /* Unused. Silent compiler warning. */
    (void) upload_data_size;  /* Unused. Silent compiler warning. */

    //printf("URL : %s\n",url);
    const char* next = next_resource(url,url);
    if(next)
    {
        //printf("URL Next : %s\n",next);
        Resource* actual = &root;
        /*printf("Map size : %llu\n",actual->branch.size());
        for(auto const& r : actual->branch)
        {
            printf("\tkey : %s\n",r.first.c_str());
            printf("\tvalue : %s\n",r.second.name_string.c_str());
        }*/
        auto itactual = actual->branch.find(next);
        if(itactual != actual->branch.end())
        {
            actual = &(*itactual).second;
            const MHD_ConnectionInfo* info = MHD_get_connection_info(connection,MHD_CONNECTION_INFO_PROTOCOL);
            if(info)
            {
                if(actual->identify)
                {
                    if (is_authenticated_https(connection))
                    {
                        return actual->reply(connection);
                    }
                    else
                    {
                        return unauthorized_access(connection);
                    }
                }
                else
                {
                    return actual->reply(connection);
                }
            }
            else
            {
                if(actual->identify)
                {
                    if (is_authenticated_http(connection))
                    {
                        return actual->reply(connection);
                    }
                    else
                    {
                        return unauthorized_access(connection);
                    }
                }
            }
        }
        else
        {
            return unknow_resource(connection);
        }
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
    if(info)
    {
        const char *headervalue = MHD_lookup_connection_value (connection, MHD_HEADER_KIND,"WWW-Authenticate");
        if(headervalue)
        {
            MHD_set_connection_value(connection, MHD_HEADER_KIND,"WWW-Authenticate","");
        }
    }

    response = MHD_create_response_from_buffer (strlen (page), (void *) page, MHD_RESPMEM_PERSISTENT);
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
            root.branch.insert(std::pair(logout.name_string,logout));
            return ask_for_authentication(connection,REALM);
        }

    }
    else
    {
        if (not is_authenticated_http(connection))
        {
            Resource logout("logout",default_logout,true);
            root.branch.insert(std::pair(logout.name_string,logout));
            return ask_for_authentication(connection,REALM);
        }
    }

    return unauthorized_access(connection);
}
