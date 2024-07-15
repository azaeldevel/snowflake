

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


#include <sys/types.h>
#ifndef _WIN32
#include <sys/select.h>
#include <sys/socket.h>
#else
#include <winsock2.h>
#endif
#include <microhttpd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#if defined linux
    #include <mysql.h>
#elif defined MSYS2
    #include <mysql/mysql.h>
#else

#endif
#include <map>
#include <string>
#include <vector>

//#define PORT 8081

#define REALM     "\"Maintenance\""


enum class erros_code
{
    unknow,
    incomplete_input_parameters,
    not_found_certificate,
    not_found_certificate_key,
    not_found_api_key,
    not_found_api_pasword,
    unknow_resource,
};

enum class container_type
{
    none,
    buffer,
    callback,
    buffer_external,

    callback_external,
};

typedef MHD_Result (*external)(MHD_Connection*) ;
MHD_Result default_page(MHD_Connection* connection);
MHD_Result default_logout(MHD_Connection* connection);
MHD_Result default_loging(MHD_Connection* connection);
MHD_Result error_page (MHD_Connection *connection);
MHD_Result unauthorized_access (MHD_Connection *connection);
MHD_Result unknow_resource (MHD_Connection *connection);
MHD_Result favicon_request(MHD_Connection* connection);

/**
*\brief Representa un Recurso en la URL
**/
struct Resource
{
    Resource() = default;
    Resource(const Resource&);
    Resource(const std::string&,external,bool);
    ~Resource();
    /**
    *\brief Nombre del recurso
    **/
    std::string name_string;

    /**
    *\brief su uso depende de la utima llada a la funcion que lo asigna(from(...))
    *\private
    **/
    void* container;

    /**
    *\brief su uso depende de la utima llada a la funcion que lo asigna(from(...))
    *\private
    **/
    size_t container_size;

    /**
    *\brief su uso depende de la utima llada a la funcion que lo asigna(from(...))
    *\private
    **/
    container_type type;

    /**
    *\brief Requiere que se identifique el cliente
    *\private
    **/
    bool identify;

    std::map<std::string,Resource> branch;

    /**
    *\brief El recurso se va a contruir a partir de un callback
    **/
    MHD_Result reply(MHD_Connection*);

    /**
    *\brief El recurso se va a contruir a partir de un callback
    **/
    Resource* find(const char* url);
    /**
    *\brief El recurso se va a contruir a partir de un callback
    **/
    Resource* find(const std::vector<std::string>& rcs);
    /**
    *\brief El recurso se va a contruir a partir de un callback
    **/
    Resource* find(const std::vector<std::string>& rcs,size_t);

};
extern Resource* root;

struct Databox
{
    std::map<std::string,std::string> HEADER;
    std::map<std::string,std::string> COOKIES;
    std::map<std::string,std::string> POST;
    std::map<std::string,std::string> GET;
    std::map<std::string,std::string> FOOTER;


};

struct Server
{
    MHD_Daemon* service;
    char* certificate_file;
    char* certificate_file_key;
    Resource root;
    char *key_pem;
    char *cert_pem;
    size_t params_size;
    void** params;
    MHD_ValueKind kind;
    int port;

#if OCTETOS_SNOWFLAKE_V0_TDD
    Databox* databox;
#endif // OCTETOS_SNOWFLAKE_V0_TDD

    Server();
    ~Server();

    void start();
    void stop();
    int read_params(int argc, char* argv[]);

    void enable_header_kind();
    void enable_cookies_kind();
    void enable_post_kind();
    void enable_get_kind();
    void enable_footer_kind();
    void set_kind(MHD_ValueKind);

    void load_certificate(char* file,char* key);
};



MHD_Result secret_page (struct MHD_Connection *connection);
MHD_Result answer_connection_http(void *cls, struct MHD_Connection *connection,
                      const char *url, const char *method,
                      const char *version, const char *upload_data,
                      size_t *upload_data_size, void **con_cls);
MHD_Result answer_connection_https(void *cls, struct MHD_Connection *connection,
                      const char *url, const char *method,
                      const char *version, const char *upload_data,
                      size_t *upload_data_size, void **con_cls);


long get_file_size (const char *filename);
char * load_file (const char *filename);



char * string_to_base64 (const char *message);
bool is_authenticated_https (struct MHD_Connection *connection);
bool is_authenticated_http (struct MHD_Connection *connection);
/**
*\brief Requiere autentificacion para servidor SSL
**/
MHD_Result ask_for_authentication (struct MHD_Connection *connection, const char *realm);
/**
*\brief Requiere autentificacion para servidor no-SSL
**/
MHD_Result ask_for_authentication (struct MHD_Connection *connection);


/**
*\brief Returna una conexion valida a la base de datos
*/
MYSQL* create_conection();
/**
*\brief Verifica
*/
bool verify_authentication(MYSQL*,const char* autho);
/**
*\brief Verifica
*/
bool verify_authentication(MYSQL*,const char* u,const char* p);


/**
*\brief Busca el siguiente recource en 'string' comenzando en 'begin'. Todos los slash son ignorado.
*\return si hay un siguiente resource return la un puntero a la primer posicion de otra forma returna NULL
*/
const char* next_resource(const char* string,const char* begin);


MHD_Result iterator(void *cls, enum MHD_ValueKind kind, const char *key, const char *value);
