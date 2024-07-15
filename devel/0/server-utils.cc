/**
*\brief main
**/


#include "server.hh"
Resource* root = NULL;//Resource root("/",default_page,false);


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
