/**
*\brief Solo main
**/


#include "server.hh"


Resource::Resource(const Resource& r) : name_string(r.name_string),container(r.container),container_size(r.container_size),type(r.type),identify(r.identify),branch(r.branch)
{
}
Resource::Resource(const std::string& n,external call,bool i) : name_string(n),container((void*)call),container_size(0),type(container_type::callback_external),identify(i)
{
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
