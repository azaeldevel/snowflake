
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
*\brief
**/

#include "server.hh"


Server::Server() : service(NULL),certificate_file(NULL),certificate_file_key(NULL),root("/",default_page,false),key_pem(NULL),cert_pem(NULL),params_size(3),params(NULL),kind(MHD_GET_ARGUMENT_KIND),port(8081)
{
    params = (void**)malloc(sizeof(void*) * params_size);
    /*for(size_t i = 0; i < params_size; i++)
    {
        params[i] = (void*)malloc(sizeof(void*));
    }*/
    //printf("params -> '%llu'\n",params);
    params[0] = (void*)this;
    //printf("params -> '%llu'\n",params);
    //printf("server -> '%llu'\n",(void*)this);
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
        printf("Running SSL server...\n");
        service = MHD_start_daemon (MHD_USE_INTERNAL_POLLING_THREAD | MHD_USE_TLS, port, NULL,
                      NULL, &answer_connection_https, params,
                      MHD_OPTION_HTTPS_MEM_KEY, key_pem,
                      MHD_OPTION_HTTPS_MEM_CERT, cert_pem, MHD_OPTION_END);
    }
    else
    {//running http protocol
        printf("Running SSL server...\n");
        service = MHD_start_daemon (MHD_USE_INTERNAL_POLLING_THREAD, port, NULL, NULL,
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

void Server::enable_header_kind()
{
    kind = (MHD_ValueKind)(kind | MHD_HEADER_KIND);
}
void Server::enable_cookies_kind()
{
    kind = (MHD_ValueKind)(kind | MHD_COOKIE_KIND);
}
void Server::enable_post_kind()
{
    kind = (MHD_ValueKind)(kind | MHD_POSTDATA_KIND);
}
void Server::enable_get_kind()
{
    kind = (MHD_ValueKind)(kind | MHD_GET_ARGUMENT_KIND);
}
void Server::enable_footer_kind()
{
    kind = (MHD_ValueKind)(kind | MHD_FOOTER_KIND);
}
void Server::set_kind(MHD_ValueKind k)
{
    kind = k;
}

void Server::load_certificate(char* file, char* key)
{
    certificate_file = file;
    certificate_file_key = key;
    cert_pem = load_file (certificate_file);
    key_pem = load_file (certificate_file_key);
}
