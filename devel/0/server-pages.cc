/* Feel free to use this example code in any way
   you see fit (Public Domain) */

#include "server.hh"

MHD_Result ask_for_authentication (struct MHD_Connection *connection, const char *realm)
{
    enum MHD_Result ret;
    struct MHD_Response *response;
    char *headervalue;
    size_t slen;
    const char *strbase = "Basic realm=";

    response = MHD_create_response_from_buffer (0, NULL, MHD_RESPMEM_PERSISTENT);
    if (! response) return MHD_NO;

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
