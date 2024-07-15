
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

MHD_Result iterator (void *cls, MHD_ValueKind kind, const char *key, const char *value)
{
    Databox* box = (Databox*)cls;
    //printf("server/iterator -> '%llu'\n",cls);
    if(kind & MHD_HEADER_KIND)
    {
        //printf ("%s: %s\n", key, value);
        box->HEADER[key] = value;
    }
    if(kind & MHD_COOKIE_KIND)
    {
        //printf ("%s: %s\n", key, value);
        box->COOKIES[key] = value;
    }
    if(kind & MHD_POSTDATA_KIND)
    {
        //printf ("%s: %s\n", key, value);
        box->POST[key] = value;
    }
    if(kind & MHD_GET_ARGUMENT_KIND)
    {
        printf ("%s: %s\n", key, value);
        box->GET[key] = value;
    }
    if(kind & MHD_FOOTER_KIND)
    {
        //printf ("%s: %s\n", key, value);
        box->FOOTER[key] = value;
    }

    return MHD_YES;
}
