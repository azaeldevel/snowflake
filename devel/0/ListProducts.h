
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

#define PORT 8081

#define REALM     "\"Maintenance\""
#define USER      "a legitimate user azael"
#define PASSWORD  "and his password"

#ifdef CMAKE_BASED
  #define SERVERKEYFILE "../server.key"
  #define SERVERCERTFILE "../server.pem"
#elif defined(CODEBLOCKS)
  #define SERVERKEYFILE "server.key"
  #define SERVERCERTFILE "server.pem"
#else
  #error "Ambien de Desarrollo desconocido"
#endif



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
