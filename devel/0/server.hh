
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
#include <mysql.h>
#include <map>

#define PORT 8081

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

struct Resource
{
    const char* name_string;
    size_t name_size;
    std::map<const char*,Resource> branch;

};


static Resource root;




MHD_Result ask_for_authentication (struct MHD_Connection *connection, const char *realm);
MHD_Result secret_page (struct MHD_Connection *connection);
MHD_Result answer_to_connection_https (void *cls, struct MHD_Connection *connection,
                      const char *url, const char *method,
                      const char *version, const char *upload_data,
                      size_t *upload_data_size, void **con_cls);
MHD_Result answer_to_connection_http (void *cls, struct MHD_Connection *connection,
                      const char *url, const char *method,
                      const char *version, const char *upload_data,
                      size_t *upload_data_size, void **con_cls);
MHD_Result error_page (MHD_Connection *connection);


char * string_to_base64 (const char *message);
long get_file_size (const char *filename);
char * load_file (const char *filename);
int is_authenticated (struct MHD_Connection *connection, const char *username,const char *password);
bool is_authenticated_https (struct MHD_Connection *connection);
bool is_authenticated_http (struct MHD_Connection *connection);

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
