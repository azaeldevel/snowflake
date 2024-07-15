
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

//https://www.gnu.org/software/libmicrohttpd/tutorial.html
//https://www.gnu.org/software/libmicrohttpd/manual/libmicrohttpd.html#SEC_Contents
int main (int argc, char* argv[])
{
    Server serv;
    /*if(argc <= 1)
    {
        printf("Uso del comando:\n");
        printf("\t ListProduct -c path_to_certificate -k path_to_key\n");
        printf("\t ListProduct --certificate path_to_certificate --key path_to_key\n");
        return EXIT_FAILURE;
    }
    */
    Resource favicon("favicon.ico",favicon_request,false);
    serv.root.branch.insert(std::pair(favicon.name_string,favicon));

    Resource loging("loging",default_loging,false);
    Resource prueba1("prueba1",default_page,true);
    Resource prueba2("prueba2",default_page,true);
    Resource prueba21("prueba21",default_page,true);
    Resource prueba22("prueba22",default_page,true);

    serv.root.branch.insert(std::pair(loging.name_string,loging));
    prueba2.branch.insert(std::pair(prueba21.name_string,prueba21));
    prueba2.branch.insert(std::pair(prueba22.name_string,prueba22));
    serv.root.branch.insert(std::pair(prueba1.name_string,prueba1));
    serv.root.branch.insert(std::pair(prueba2.name_string,prueba2));
    //prueba2.branch.insert(std::pair(prueba21.name_string,prueba21));
    //prueba2.branch.insert(std::pair(prueba22.name_string,prueba22));
    //printf("Map size : %llu\n",root.branch.size());
    /*for(auto const& r : root.branch)
    {
        printf("\tkey : %s\n",r.first.c_str());
        printf("\t\tvalue : %s\n",r.second.name_string.c_str());
        printf("\t\tsize: %llu\n",r.second.branch.size());
    }*/

    if(serv.read_params(argc,argv) == EXIT_FAILURE) return EXIT_FAILURE;
#if OCTETOS_SNOWFLAKE_V0_DEVEL
        //printf("Certificado path: %s\n",cert_pem);
        //printf("Key path : %s\n",key_pem);
#endif // OCTETOS_SNOWFLAKE_V0_DEVEL




    serv.start();

    (void) getchar ();

    serv.stop();

    return EXIT_SUCCESS;
}
