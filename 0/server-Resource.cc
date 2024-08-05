
/**
 *
 *  This file is part of snowflake.
 *  snowflake is C/C++ Library to create a Web Server.
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
*\brief Solo main
**/


#include "server.hh"
#include "../../core/3/string.hh"

namespace core = oct::core::v3;


Resource::Resource(const Resource& r) : name_string(r.name_string),container(r.container),container_size(r.container_size),type(r.type),identify(r.identify),branch(r.branch)
{
}
Resource::Resource(const std::string& n,HANDLER_SIMPLE call,bool i) : name_string(n),container((void*)call),container_size(0),type(container_type::handler_simple),identify(i)
{
}
Resource::Resource(const std::string& n,HANDLER_FULL call,bool i) : name_string(n),container((void*)call),container_size(0),type(container_type::handler_full),identify(i)
{
}
Resource::Resource(const std::string& n,HANDLER_WITH_CONNECTIONS call,bool i) : name_string(n),container((void*)call),container_size(0),type(container_type::handler_with_connections),identify(i)
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
    switch(type)
    {
    case container_type::handler_simple:
        {
            HANDLER_SIMPLE call = (HANDLER_SIMPLE) container;
            return call(conn);
        }
    default:
        return default_page(conn);
    }

    return default_page(conn);
}

Resource* Resource::find(const char* url)
{
    //printf("URL : '/'\n");
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


void Resource::set_handler(HANDLER_SIMPLE h)
{
    type = container_type::handler_simple;
    container = (void*) h;
}
void Resource::set_handler(HANDLER_FULL h)
{
    type = container_type::handler_full;
    container = (void*) h;
}

