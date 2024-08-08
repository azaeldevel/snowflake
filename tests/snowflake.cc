
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

#include "../0/server.hh"

//https://www.gnu.org/software/libmicrohttpd/tutorial.html
//https://www.gnu.org/software/libmicrohttpd/manual/libmicrohttpd.html#SEC_Contents
int main (int argc, char* argv[])
{
    printf("\nIniciando servidor de desarrollo..\n");
    Server serv;
    Resource favicon("favicon.ico",favicon_request,false);
    //serv.root.branch.insert(std::pair(favicon.name_string,favicon));
    Resource loging("loging",default_loging,false);
    Resource tdd("tdd",TDD,false);
    Resource rcincrement("increment",hincrement,false);
    Resource rcap02("ap02",hap02,false);
    Resource rcap03("ap03",hap03,false);
    Resource prueba1("prueba1",default_page,true);
    Resource prueba2("prueba2",default_page,true);
    Resource prueba21("prueba21",default_page,true);
    Resource prueba22("prueba22",default_page,true);
    //serv.root.branch.insert(std::pair(loging.name_string,loging));
    //serv.root.branch.insert(std::pair(tdd.name_string,tdd));
    //serv.root.branch.insert(std::pair(rcincrement.name_string,rcincrement));
    //serv.root.branch.insert(std::pair(rcap03.name_string,rcap03));
    //prueba2.branch.insert(std::pair(prueba21.name_string,prueba21));
    //prueba2.branch.insert(std::pair(prueba22.name_string,prueba22));
    //serv.root.branch.insert(std::pair(prueba1.name_string,prueba1));
    //serv.root.branch.insert(std::pair(prueba2.name_string,prueba2));

    serv.add("/favicon.ico",favicon);
    serv.add("/loging",loging);
    serv.add("/increment",rcincrement);
    serv.add("/ap02",rcap02);
    serv.add("/ap03",rcap03);
    serv.add("/prueba1",prueba1);
    serv.add("/prueba2",prueba2);
    serv.add("/prueba21",prueba21);
    serv.add("/prueba22",prueba22);

    //serv.load_certificate("tests/server.pem","tests/server.key");
    serv.start();

    if(serv.service)
    {
        (void)getchar();
    }
    else
    {
        printf("\nFallo la creacion del servicio..\n");
    }

    serv.stop();
    printf("\nFinalizando servidor de desarrollo..\n");

    return EXIT_SUCCESS;
}
