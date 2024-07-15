

#ifndef OCTETOS_CORE_TESTS_V3_HH
#define OCTETOS_CORE_TESTS_V3_HH


#include <CUnit/Basic.h>


//#include "tests.hh"
#if defined(__linux__)
    #include "../3/Exception.hh"
#elif defined(_WIN32) || defined(_WIN64)

#else
	#error "Plataforma desconocida"
#endif

int v3_init(void);
int v3_clean(void);
void v3_developing();
void v3_array();
void v3_tree();
void v3_cpp();
void v3_trails();
void v3_Number();
void v3_performance();
void v3_to_number();
void v3_table_performace_1();

#endif
