/**
*\brief main
**/


#include "server.hh"
Resource root("/",default_page,false);



//https://www.gnu.org/software/libmicrohttpd/tutorial.html
//https://www.gnu.org/software/libmicrohttpd/manual/libmicrohttpd.html#SEC_Contents
int main (int argc, char* argv[])
{
    Resource favicon("favicon.ico",favicon_request,false);
    root.branch.insert(std::pair(favicon.name_string,favicon));

    Resource loging("loging",default_loging,false);
    Resource prueba1("prueba1",default_page,true);
    Resource prueba2("prueba2",default_page,true);
    Resource prueba21("prueba21",default_page,true);
    Resource prueba22("prueba22",default_page,true);
    prueba2.branch.insert(std::pair(prueba21.name_string,prueba21));
    prueba2.branch.insert(std::pair(prueba22.name_string,prueba22));
    root.branch.insert(std::pair(loging.name_string,loging));
    root.branch.insert(std::pair(prueba1.name_string,prueba1));
    root.branch.insert(std::pair(prueba2.name_string,prueba2));

    Resource* rc1 = Resource::find("/");
    if(rc1) printf("'/' : enconctrada\n");
    else printf("'/' : no enconctrada\n");

    Resource* rc2 = Resource::find("/loging");
    if(rc2) printf("'/loging' : enconctrada\n");
    else printf("'/loging' : no enconctrada\n");

    Resource* rc3 = Resource::find("/prueba1");
    if(rc3) printf("'/prueba1' : enconctrada\n");
    else printf("'/prueba1' : no enconctrada\n");

    Resource* rc4 = Resource::find("/prueba2");
    if(rc4) printf("'/prueba2' : enconctrada\n");
    else printf("'/prueba2' : no enconctrada\n");

    Resource* rc5 = Resource::find("/prueba2/prueba21");
    if(rc5) printf("'/prueba2/prueba21' : enconctrada\n");
    else printf("'/prueba2/prueba21' : no enconctrada\n");

    Resource* rc6 = Resource::find("/prueba2/prueba22");
    if(rc6) printf("'/prueba2/prueba22' : enconctrada\n");
    else printf("'/prueba2/prueba22' : no enconctrada\n");

    Resource* rc7 = Resource::find("/prueba2/prueba22/resps");
    if(rc7) printf("'/prueba2/prueba22/resps' : enconctrada\n");
    else printf("'/prueba2/prueba22/resps' : no enconctrada\n");

    Resource* rc8 = Resource::find("/prueba2/prueba26");
    if(rc8) printf("'/prueba2/prueba26' : enconctrada\n");
    else printf("'/prueba2/prueba26' : no enconctrada\n");


    return EXIT_SUCCESS;
}
