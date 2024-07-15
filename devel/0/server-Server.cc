/**
*\brief
**/

#include "server.hh"


Server::Server() : service(NULL),certificate_file(NULL),root("/",default_page,false),certificate_file_key(NULL),key_pem(NULL),cert_pem(NULL),params_size(3),params(NULL)
{
    params = (void**)malloc(sizeof(void*) * params_size);
    /*for(size_t i = 0; i < params_size; i++)
    {
        params[i] = (void*)malloc(sizeof(void*));
    }*/
    printf("params -> '%llu'\n",params);
    params[0] = (void*)this;
    //printf("params -> '%llu'\n",params);
    printf("server -> '%llu'\n",(void*)this);
}
Server::~Server()
{
    if(key_pem)free (key_pem);
    if(cert_pem)free (cert_pem);
    if (NULL == service)
    {
        if(key_pem) free (key_pem);
        if(cert_pem) free (cert_pem);
    }
    if(params)
    {
        free(params);
    }
}

void Server::start()
{
    ::root = &root;
    if (key_pem and cert_pem)
    {//running https protocol
        //printf("Running SSL server...\n");
        service = MHD_start_daemon (MHD_USE_INTERNAL_POLLING_THREAD | MHD_USE_TLS, PORT, NULL,
                      NULL, &answer_connection_https, params,
                      MHD_OPTION_HTTPS_MEM_KEY, key_pem,
                      MHD_OPTION_HTTPS_MEM_CERT, cert_pem, MHD_OPTION_END);
    }
    else
    {//running http protocol
        service = MHD_start_daemon (MHD_USE_INTERNAL_POLLING_THREAD, PORT, NULL, NULL,
                             &answer_connection_http, params, MHD_OPTION_END);
    }

}
void Server::stop()
{
    MHD_stop_daemon (service);
}
int Server::read_params(int argc, char* argv[])
{
    for(int i = 1; i < argc; i++)
    {
        if(strcmp("-c",argv[i]) == 0 or strcmp("--certificate",argv[i]) == 0)
        {
            if(i + 1 < argc)
            {
                certificate_file = argv[++i];
                cert_pem = load_file (certificate_file);
            }
            else
            {
                printf ("Deve indicar la direccion del certificado\n");
                return EXIT_FAILURE;
            }
        }
        else if(strcmp("-k",argv[i]) == 0 or strcmp("--key",argv[i]) == 0)
        {
            if(i + 1 < argc)
            {
                certificate_file_key = argv[++i];
                key_pem = load_file (certificate_file_key);
            }
            else
            {
                printf ("Deve indicar la direccion de llave\n");
                return EXIT_FAILURE;
            }
        }
    }

    return EXIT_SUCCESS;
}
