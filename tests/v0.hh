

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

extern Server serv_devel;
int v0_init(void);
int v0_clean(void);
void v0_developing();
CURL* query(const char*);

#endif
