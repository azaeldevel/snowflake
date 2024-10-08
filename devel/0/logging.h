
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
};


MHD_Result ask_for_authentication (struct MHD_Connection *connection, const char *realm);
MHD_Result secret_page (struct MHD_Connection *connection);
MHD_Result answer_to_connection (void *cls, struct MHD_Connection *connection,
                      const char *url, const char *method,
                      const char *version, const char *upload_data,
                      size_t *upload_data_size, void **con_cls);
MHD_Result error_page (MHD_Connection *connection,erros_code);


char * string_to_base64 (const char *message);
long get_file_size (const char *filename);
char * load_file (const char *filename);
int is_authenticated (struct MHD_Connection *connection, const char *username,const char *password);
bool is_authenticated (struct MHD_Connection *connection);

/**
*\brief Returna una conexion valida a la base de datos
*/
MYSQL* create_conection();
/**
*\brief Verifica que consumer key y consumer secret sean validos
*/
bool verify_authentication(MYSQL*,const char* autho);
