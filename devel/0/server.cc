/**
*\brief main
**/


#include "server.hh"
Resource root("/",default_page,false);


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

//https://www.gnu.org/software/libmicrohttpd/tutorial.html
//https://www.gnu.org/software/libmicrohttpd/manual/libmicrohttpd.html#SEC_Contents
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
    Resource favicon("favicon.ico",&favicon_request,false);
    root.branch.insert(std::pair(favicon.name_string,favicon));


    Resource loging("loging",default_loging,false);
    Resource prueba1("prueba1",default_page,true);
    Resource prueba2("prueba2",default_page,true);
    //Resource prueba21("prueba21",default_page,true);
    //Resource prueba22("prueba22",default_page,true);
    root.branch.insert(std::pair(loging.name_string,loging));
    root.branch.insert(std::pair(prueba1.name_string,prueba1));
    root.branch.insert(std::pair(prueba2.name_string,prueba2));
    //prueba2.branch.insert(std::pair(prueba21.name_string,prueba21));
    //prueba2.branch.insert(std::pair(prueba22.name_string,prueba22));
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
        printf("Running SSL server...\n");
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
