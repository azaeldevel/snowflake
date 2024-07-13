/* Feel free to use this example code in any way
   you see fit (Public Domain) */


#include "server.hh"
Resource root{"/",NULL,0,container_type::callback_external,true};

char * string_to_base64 (const char *message)
{
  const char *lookup =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  unsigned long l;
  size_t i;
  char *tmp;
  size_t length = strlen (message);

  tmp = (char*)malloc (length * 2 + 1);
  if (NULL == tmp)
    return NULL;
  tmp[0] = 0;
  for (i = 0; i < length; i += 3)
  {
    l = (((unsigned long) message[i]) << 16)
        | (((i + 1) < length) ? (((unsigned long) message[i + 1]) << 8) : 0)
        | (((i + 2) < length) ? ((unsigned long) message[i + 2]) : 0);


    strncat (tmp, &lookup[(l >> 18) & 0x3F], 1);
    strncat (tmp, &lookup[(l >> 12) & 0x3F], 1);

    if (i + 1 < length)
      strncat (tmp, &lookup[(l >> 6) & 0x3F], 1);
    if (i + 2 < length)
      strncat (tmp, &lookup[l & 0x3F], 1);
  }

  if (length % 3)
    strncat (tmp, "===", 3 - length % 3);

  return tmp;
}


long get_file_size (const char *filename)
{
  FILE *fp;

  fp = fopen (filename, "rb");
  if (fp)
  {
    long size;

    if ((0 != fseek (fp, 0, SEEK_END)) || (-1 == (size = ftell (fp))))
      size = 0;

    fclose (fp);

    return size;
  }
  else
    return 0;
}


char * load_file (const char *filename)
{
  FILE *fp;
  char *buffer;
  long size;

  size = get_file_size (filename);
  if (0 == size)
    return NULL;

  fp = fopen (filename, "rb");
  if (! fp)
    return NULL;

  buffer = (char*)malloc (size + 1);
  if (! buffer)
  {
    fclose (fp);
    return NULL;
  }
  buffer[size] = '\0';

  if (size != (long) fread (buffer, 1, size, fp))
  {
    free (buffer);
    buffer = NULL;
  }

  fclose (fp);
  return buffer;
}


int is_authenticated (struct MHD_Connection *connection, const char *username, const char *password)
{
    const char *headervalue;
    char *expected_b64;
    char *expected;
    const char *strbase = "Basic ";
    int authenticated;
    size_t slen;

    headervalue = MHD_lookup_connection_value (connection, MHD_HEADER_KIND,"Authorization");
    if (NULL == headervalue) return 0;
    if (0 != strncmp (headervalue, strbase, strlen (strbase))) return 0;

    slen = strlen (username) + 1 + strlen (password) + 1;
    if (NULL == (expected = (char*)malloc (slen))) return 0;
    snprintf (expected, slen, "%s:%s", username, password);//expected = "usario:password"
    expected_b64 = string_to_base64 (expected);
    printf("Header Value : %s\n", headervalue);
    printf("Expected : %s\n", expected_b64);
    free (expected);
    if (NULL == expected_b64) return 0;

    authenticated = (strcmp (headervalue + strlen (strbase), expected_b64) == 0);
    free (expected_b64);
    return authenticated;
}

bool is_authenticated_https (struct MHD_Connection *connection)
{
    const char *headervalue;
    const char *strbase = "Basic ";

    headervalue = MHD_lookup_connection_value (connection, MHD_HEADER_KIND,"Authorization");
    if (NULL == headervalue) return false;
    if (0 != strncmp (headervalue, strbase, strlen (strbase))) return false;
    //printf("Header Value : %s\n", headervalue);
    auto conn = create_conection();
    if(verify_authentication(conn,headervalue))
    {
        mysql_close(conn);
        //printf("User : %s",MHD_digest_auth_get_username(connection));
        return true;
    }

    mysql_close(conn);
    return false;
}
bool is_authenticated_http (struct MHD_Connection *connection)
{
    bool fail;
    char * pass = NULL;
    char *user = MHD_basic_auth_get_username_password (connection,&pass);
    if(user and pass)
    {
        auto mysql = create_conection();
        fail = not verify_authentication(mysql,user,pass);
    }
    else
    {
        fail = true;
    }
    if (user) MHD_free (user);
    if (pass) MHD_free (pass);

    return fail;
}

int main (int argc, char* argv[])
{
    struct MHD_Daemon *daemon;
    char *key_pem = NULL;
    char *cert_pem = NULL;
    char *certificate_file = NULL;
    char *certificate_file_key = NULL;
    /*if(argc <= 1)
    {
        printf("Uso del comando:\n");
        printf("\t ListProduct -c path_to_certificate -k path_to_key\n");
        printf("\t ListProduct --certificate path_to_certificate --key path_to_key\n");
        return EXIT_FAILURE;
    }
    */
    Resource loging{"loging",NULL,0,container_type::none,true};
    Resource logout{"logout",NULL,0,container_type::none,true};
    root.branch.insert(std::pair(loging.name_string,loging));
    root.branch.insert(std::pair(logout.name_string,logout));
    printf("Map size : %llu\n",root.branch.size());

    /*for(auto const& r : root.branch)
    {
        printf("\tkey : %s\n",r.first.c_str());
        printf("\tvalue : %s\n",r.second.name_string);
    }*/

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
#if OCTETOS_SNOWFLAKE_V0_DEVEL
        //printf("Certificado path: %s\n",cert_pem);
        //printf("Key path : %s\n",key_pem);
#endif // OCTETOS_SNOWFLAKE_V0_DEVEL




    if (key_pem and cert_pem)
    {//running https protocol
        daemon = MHD_start_daemon (MHD_USE_INTERNAL_POLLING_THREAD | MHD_USE_TLS, PORT, NULL,
                      NULL, &answer_connection, NULL,
                      MHD_OPTION_HTTPS_MEM_KEY, key_pem,
                      MHD_OPTION_HTTPS_MEM_CERT, cert_pem, MHD_OPTION_END);
    }
    else
    {//running http protocol
        daemon = MHD_start_daemon (MHD_USE_INTERNAL_POLLING_THREAD, PORT, NULL, NULL,
                             &answer_connection, NULL, MHD_OPTION_END);
    }


    if (NULL == daemon)
    {
        printf ("%s\n", cert_pem);

        if(key_pem) free (key_pem);
        if(cert_pem) free (cert_pem);

        return EXIT_FAILURE;
    }

    (void) getchar ();

    MHD_stop_daemon (daemon);
    free (key_pem);
    free (cert_pem);

    return EXIT_SUCCESS;
}

const char* next_resource(const char* string,const char* begin)
{
    size_t offset = string - begin;
    //printf("offset : %i\n",offset);
    size_t length = strlen(string);

    //el final del actual resource
    size_t actual;
    for(actual = offset; actual < length; actual++)
    {
        if(string[actual] == '/') break;
    }
    //
    if(string[actual + 1] == '\0') return NULL;//si se apunta al final de la cadena
    if(actual - length - 1 == 0) return NULL;//si se apunta al final de la cadena

    return &string[actual + 1];
}



Resource::~Resource()
{
    if(type == container_type::buffer)
    {
        if(container)
        {
            free(container);
            container = NULL;
        }
    }
}

MHD_Result Resource::reply(MHD_Connection* conn)
{
    external call = (external) container;
    switch(type)
    {
    case container_type::callback_external:
        return call(conn);
        break;
    default:
        return default_page(conn);
    }

    return default_page(conn);
}
