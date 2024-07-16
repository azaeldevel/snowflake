

#ifndef OCTETOS_SNOWFLAKE_TESTS_V0_HH
#define OCTETOS_SNOWFLAKE_TESTS_V0_HH

#include <CUnit/Basic.h>
#include <curl/curl.h>
#if defined(__linux__)
#elif defined(MSYS2)
#elif defined(_WIN32) || defined(_WIN64)
#else
	#error "Plataforma desconocida"
#endif

#include "../0/server.hh"
#include "../../core/3/numbers.hh"
namespace core = oct::core::v3;

struct MemoryStruct
{
    char *memory;
    size_t size;
};


extern Server serv_devel;
int v0_init(void);
int v0_clean(void);
void v0_developing();


void curl_query(CURL*,const char*,MemoryStruct*);
void curl_build1(CURL*,const char* url,MemoryStruct* chunk);

#endif
