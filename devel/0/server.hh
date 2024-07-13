
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
#elif defined(MSYS2)
    #include <mysql/mysql.h>
#else

#endif
#include <map>
#include <string>

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

/**
*\brief Representa un Recurso en la URL
**/
struct Resource
{
    Resource() = default;
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
    *\brief El recurso se va a contruir a partir de callback
    **/
    void from(MHD_AccessHandlerCallback);
    /**
    *\brief El recurso se va a contruir a partir de un buffer
    **/
    void from(const char*);
    /**
    *\brief El recurso se va a contruir a partir de un buffer externo
    **/
    void from(const char**);
    /**
    *\brief El recurso se va a contruir a partir de un callback
    **/
    void from(MHD_Result* (*)(MHD_Connection*));
    /**
    *\brief El recurso se va a contruir a partir de un callback
    **/
    void from(MHD_Result* (*)(MHD_Connection*,const char*));

    /**
    *\brief El recurso se va a contruir a partir de un callback
    **/
    MHD_Response* build(const char**);


    /**
    *\brief El recurso se va a contruir a partir de un callback
    **/
    MHD_Result reply(MHD_Connection*);


    /**
    *\brief El recurso se va a contruir a partir de un callback
    **/
    MHD_Result reply(MHD_Connection*,const char**);

    /**
    *\brief El recurso se va a contruir a partir de un callback
    **/
    MHD_Result reply(MHD_Connection*,MHD_Result (*)(MHD_Connection*));

};


extern Resource root;




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
MHD_Result answer_connection(void *cls, struct MHD_Connection *connection,
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
