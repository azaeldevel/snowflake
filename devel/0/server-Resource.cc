/**
*\brief Solo main
**/


#include "server.hh"
#include "../../../core/3/string.hh"

namespace core = oct::core::v3;


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

Resource* Resource::find(const char* url)
{
    printf("URL : '/'\n");
    size_t length = strlen(url);
    if(length == 1 and url[0] == '/')
    {
        //printf("\nResource : '/'\n");
        return this;
    }

    //
    auto rcs = core::split(url,"/");
    //printf("\nSize : '%llu'\n",rcs.size());
    return find(rcs);
}
Resource* Resource::find(const std::vector<std::string>& rcs)
{
    auto it = branch.find(rcs[0]);
    if(it != branch.end())
    {
        //printf("\nResource : '%s'\n",(*it).first.c_str());
        if(1 < rcs.size())
        {
            return ((*it).second.find(rcs, 1));
        }
        else
        {
            return &(*it).second;
        }
    }

    return NULL;
}
Resource* Resource::find(const std::vector<std::string>& rcs,size_t index)
{
    //if(index >= rcs.size()) return NULL;
    /*printf("\t'%llu'-> '%s'\n",index,rcs[index].c_str());
    printf("\tmap size : '%llu\n",branch.size());
    for(auto const& r : branch)
    {
        printf("\tkey : %s\n",r.first.c_str());
        printf("\tvalue : %s\n",r.second.name_string.c_str());
    }*/
    auto it = branch.find(rcs[index]);
    if(it != branch.end())
    {
        //printf("\tResource : '%s'\n",(*it).first.c_str());
        //printf("\tstring : '%s'\n",(*it).first.c_str());
        if(index + 1 < rcs.size())
        {
            return ((*it).second.find(rcs, index + 1));
        }
        else
        {
            return &(*it).second;
        }
    }

    return NULL;
}
