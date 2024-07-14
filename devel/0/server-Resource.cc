/**
*\brief Solo main
**/


#include "server.hh"

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
