
#include <iostream>
#include <CUnit/Basic.h>


#ifdef OCTETOS_CORE_V2
	#include "v2.hh"
#endif
#ifdef OCTETOS_CORE_V3_TDD
	#include "v3.hh"
#endif

int main(int argc, char *argv[])
{
	/* initialize the CUnit test registry */
	if (CUE_SUCCESS != CU_initialize_registry()) return CU_get_error();

#ifdef OCTETOS_CORE_V2
    //std::cout << "Step 2\n";
	bdir = "";
	std::string option;
	for(int i = 1 ; i < argc; i++)
	{
		option = argv[i];
		//bdir
		int bdirIndex = option.compare(0, 7, "--bdir=");
		if(!bdirIndex)
		{
			bdir = option.substr(7,option.size());
			//std::cout << "bdir es '" << bdir << "'.\n";
		}
		int helpIndex = option.compare("--help");
		if(!helpIndex)
		{
			std::cout << "--bdir=[directory]		Directorio de contrución";
		}
	}
	oct::core::v2::Artifact packinfo;
	oct::core::v2::getPackageInfo(packinfo);

	//std::cout << "Step 3\n";

	int majorNumber = 2;
	if(majorNumber != packinfo.version.getMajor())
	{
		std::cerr << "Cree un nuevo archivo para la version '" << majorNumber << "' estas en la version'" << (std::string)packinfo.version << "'\n";
		return EXIT_FAILURE;
	}

	//std::cout << "Step 3\n";

	//std::string& pkName = packinfo.name;
	//std::string headerTest = pkName + " " + (std::string)packinfo.version + "\n" + packinfo.licence.getText() + "\n" + packinfo.brief + "\n";
	CU_pSuite pSuite_v2 = CU_add_suite("Octetos core v2", v2_init, v2_clean);
	if (NULL == pSuite_v2)
	{
		CU_cleanup_registry();
		return CU_get_error();
	}

	if ((NULL == CU_add_test(pSuite_v2, "Criterios de comparación semver v1.0.0", v2_testComparators_v100)))
	{
		CU_cleanup_registry();
		return CU_get_error();
	}

	if ((NULL == CU_add_test(pSuite_v2, "Validacion de parseo semver v1.0.0", v2_testParseString_v100)))
	{
		CU_cleanup_registry();
		return CU_get_error();
	}
	if ((NULL == CU_add_test(pSuite_v2, "Validacion de Operaciones semver v1.0.0", v2_testImports_v100)))
	{
		CU_cleanup_registry();
		return CU_get_error();
	}


	if ((NULL == CU_add_test(pSuite_v2, "Developing..", v2_developing)))
	{
		CU_cleanup_registry();
		return CU_get_error();
	}
#endif
#ifdef OCTETOS_CORE_V3_TDD
	//std::cout << "Step 2\n";
	CU_pSuite pSuite_v3 = CU_add_suite("Octetos Core v3", v3_init, v3_clean);
	if (NULL == pSuite_v3)
	{
		CU_cleanup_registry();
		return CU_get_error();
	}
	if ((NULL == CU_add_test(pSuite_v3, "C++ ..", v3_cpp)))
	{
		CU_cleanup_registry();
		return CU_get_error();
	}
	if ((NULL == CU_add_test(pSuite_v3, "Developing..", v3_developing)))
	{
		CU_cleanup_registry();
		return CU_get_error();
	}
	if ((NULL == CU_add_test(pSuite_v3, "Array class..", v3_array)))
	{
		CU_cleanup_registry();
		return CU_get_error();
	}
	if ((NULL == CU_add_test(pSuite_v3, "Tree class..", v3_tree)))
	{
		CU_cleanup_registry();
		return CU_get_error();
	}
	if ((NULL == CU_add_test(pSuite_v3, "Trails..", v3_trails)))
	{
		CU_cleanup_registry();
		return CU_get_error();
	}
	if ((NULL == CU_add_test(pSuite_v3, "Clase Real..", v3_Number)))
	{
		CU_cleanup_registry();
		return CU_get_error();
	}
	if ((NULL == CU_add_test(pSuite_v3, "to_number funtion ..", v3_to_number)))
	{
		CU_cleanup_registry();
		return CU_get_error();
	}
	if ((NULL == CU_add_test(pSuite_v3, "Table (performace)..", v3_table_performace_1)))
	{
		CU_cleanup_registry();
		return CU_get_error();
	}
#ifdef OCTETOS_CORE_V3_PERFORMANCE
	if ((NULL == CU_add_test(pSuite_v3, "Performance..", v3_performance)))
	{
		CU_cleanup_registry();
		return CU_get_error();
	}
#endif
    #ifdef OCTETOS_CORE_V3_OCC
	if ((NULL == CU_add_test(pSuite_v3, "Lenguaje Collection, inner Semver..", v3_lc_innier_semver)))
	{
		CU_cleanup_registry();
		return CU_get_error();
	}
	if ((NULL == CU_add_test(pSuite_v3, "Lenguaje Collection, Semver 1..", v3_lc_semver)))
	{
		CU_cleanup_registry();
		return CU_get_error();
	}
	if ((NULL == CU_add_test(pSuite_v3, "Lenguaje Collection, Semver 2..", v3_lc_semver2)))
	{
		CU_cleanup_registry();
		return CU_get_error();
	}
	/*if ((NULL == CU_add_test(pSuite_v3, "Lenguaje Collection, Semver rechazos..", v3_lc_semver_rejects)))
	{
		CU_cleanup_registry();
		return CU_get_error();
	}*/
	if ((NULL == CU_add_test(pSuite_v3, "Lexer Tipo C class..", v3_TTC)))
	{
		CU_cleanup_registry();
		return CU_get_error();
	}
	#endif
#endif

	/* Run all tests using the CUnit Basic interface */
	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();
	CU_cleanup_registry();
	return CU_get_error();
}
