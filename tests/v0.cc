

#include "v3.hh"

#include <string.h>
#include <iostream>
#include <variant>
#include <random>
#include <chrono>

#include "../3/array.hh"
#include "../3/tree.hh"
//#include <core/3/Semver.hh>
#include "../3/numbers.hh"
#include <algorithm>
#include "../3/Configuration.hh"
#include "../3/platform.hh"
#include "../3/table.hh"
#include "../3/string.hh"


#if defined(__linux__)

#elif defined(_WIN32) || defined(_WIN64)

#else
	#error "Plataforma desconocida"
#endif


namespace core = oct::core::v3;

int v3_init(void)
{
	return 0;
}

int v3_clean(void)
{
	return 0;
}



void v3_developing()
{
    float a = 1.5f;
    float b = 1.5f;
    float c = 1.7f;
    float epsilon = 0.001f;
    float d = b - (epsilon/2.0f);
    float e = b + (epsilon/2.0f);
    CU_ASSERT(core::equal(a,b));
    CU_ASSERT_FALSE(core::equal(a,c));
    CU_ASSERT(core::equal(a,d,epsilon));
    CU_ASSERT(core::equal(a,e,epsilon));

    int f = 2;
    int g = 2;
    int h = 3;
    CU_ASSERT(core::equal(f,g));
    CU_ASSERT_FALSE(core::equal(f,h));
    CU_ASSERT(core::equal(f,h,1));

    core::convertion_to_real<int>::type data = 1.11f;
    CU_ASSERT(core::equal(data,1.11f));


    char* tmpf_config_temp = tmpnam(NULL);
    std::filesystem::path tmpf_config = tmpf_config_temp;
    core::Configuration config1("mps2-conf");


    core::Semver veroct;
    veroct.major=OCTETOS_CORE_VERSION_MAJOR;
    veroct.minor=OCTETOS_CORE_VERSION_MINOR;
    veroct.patch=OCTETOS_CORE_VERSION_PATCH;
    core::Configuration config2;
    CU_ASSERT(config2.get_version().major == 0)
    char* tmpf_config_temp2 = tmpnam(NULL);
    std::filesystem::path tmpf_config2 = tmpf_config_temp2;
    //config2.create(tmpf_config2);
    config2.write(veroct);
    CU_ASSERT(config2.get_version().major == 3)
    config2.save(tmpf_config2);

    core::Configuration config3(tmpf_config2);
    CU_ASSERT(config3.get_version().major == 3)

    core::Semver veroct2;
    core::Configuration config4(tmpf_config2);
    veroct2 = config4.get_version();
    //std::cout << "config4.get_name() : " << config4.get_name() << "\n";


    std::filesystem::path userdir = core::get_user_directory();
    //std::cout << userdir << "\n";

    std::vector<std::string> vec = core::split("user=root&psw=1234567","&");
    /*for(const std::string& s : vec)
    {
        std::cout << s << "\n";
    }*/
    CU_ASSERT(vec.size() == 2);
}

struct elapse
{
    std::chrono::time_point<std::chrono::high_resolution_clock> start,end;

    template<class D = std::chrono::nanoseconds> auto duration()
    {
        return duration_cast<D>(end - start);
    }
};

void v3_table_performace_1(int numbers[],size_t numbers_length, core::table<int>& numbers_octetos, elapse& time)
{
    time.start = std::chrono::high_resolution_clock::now();
    for(size_t i = 0; i < numbers_length;i++)
    {
        numbers_octetos.push_back(numbers[i]);
    }
    time.end = std::chrono::high_resolution_clock::now();
}
void v3_table_performace_1(int numbers[],size_t numbers_length, std::vector<int>& numbers_std, elapse& time)
{
    time.start = std::chrono::high_resolution_clock::now();
    for(size_t i = 0; i < numbers_length;i++)
    {
        numbers_std.push_back(numbers[i]);
    }
    time.end = std::chrono::high_resolution_clock::now();
}

void v3_table_performace_1()
{
    constexpr size_t numbers_length = 1000;
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> numint(1,100000);
    int numbers[numbers_length];
    core::table<int> numbers_octetos;
    std::vector<int> numbers_std;
    elapse e1[numbers_length],e2[numbers_length];
    double t1 = 0,t2 = 0;

    for(size_t i = 0; i < numbers_length;i++)
    {
        numbers[i] = numint(rng);
    }

    for(size_t i = 0; i < numbers_length;i++)
    {
        v3_table_performace_1(numbers,numbers_length,numbers_octetos,e1[i]);
        v3_table_performace_1(numbers,numbers_length,numbers_std,e2[i]);
    }
    for(size_t i = 0; i < numbers_length;i++)
    {
        t1 += e1[i].duration().count();
        t2 += e2[i].duration().count();
    }
    t1 = t1 / double(numbers_length);
    t2 = t2 / double(numbers_length);

    //std::cout << "valor/base = " << duration_1 << "/" << duration_2 << "\n";
    auto speed = t1/t2;
    //std::cout << "valor/base = " << t1 << "/" << t2 << " = " << speed << "\n";
    CU_ASSERT(speed <= 1)
}

void v3_array()
{
    constexpr core::array<int,6> array1 {-1,5,6,9,8,7};
    CU_ASSERT(array1[0] == -1);
    CU_ASSERT(array1[1] == 5);
    CU_ASSERT(array1[2] == 6);
    CU_ASSERT(array1[3] == 9);
    CU_ASSERT(array1[4] == 8);
    CU_ASSERT(array1[5] == 7);

    const int* base1 = array1;
    CU_ASSERT(base1[0] == -1);
    CU_ASSERT(base1[1] == 5);
    CU_ASSERT(base1[2] == 6);
    CU_ASSERT(base1[3] == 9);
    CU_ASSERT(base1[4] == 8);
    CU_ASSERT(base1[5] == 7);

    core::array<int> array2 {-1,5,6,9,8,7};
    CU_ASSERT(array1[0] == -1);
    CU_ASSERT(array1[1] == 5);
    CU_ASSERT(array1[2] == 6);
    CU_ASSERT(array1[3] == 9);
    CU_ASSERT(array1[4] == 8);
    CU_ASSERT(array1[5] == 7);

    int* base2 = array2;
    CU_ASSERT(base2[0] == -1);
    CU_ASSERT(base2[1] == 5);
    CU_ASSERT(base2[2] == 6);
    CU_ASSERT(base2[3] == 9);
    CU_ASSERT(base2[4] == 8);
    CU_ASSERT(base2[5] == 7);

    core::array<int> array3(core::array<int>{-1,5,6,9,8,7});
    CU_ASSERT(array3[0] == -1);
    CU_ASSERT(array3[1] == 5);
    CU_ASSERT(array3[2] == 6);
    CU_ASSERT(array3[3] == 9);
    CU_ASSERT(array3[4] == 8);
    CU_ASSERT(array3[5] == 7);

    int* base3 = array3;
    CU_ASSERT(base3[0] == -1);
    CU_ASSERT(base3[1] == 5);
    CU_ASSERT(base3[2] == 6);
    CU_ASSERT(base3[3] == 9);
    CU_ASSERT(base3[4] == 8);
    CU_ASSERT(base3[5] == 7);

    core::array<int> array4;
    array4 = array3;
    CU_ASSERT(array4[0] == -1);
    CU_ASSERT(array4[1] == 5);
    CU_ASSERT(array4[2] == 6);
    CU_ASSERT(array4[3] == 9);
    CU_ASSERT(array4[4] == 8);
    CU_ASSERT(array4[5] == 7);

    int* base4 = array4;
    CU_ASSERT(base4[0] == -1);
    CU_ASSERT(base4[1] == 5);
    CU_ASSERT(base4[2] == 6);
    CU_ASSERT(base4[3] == 9);
    CU_ASSERT(base4[4] == 8);
    CU_ASSERT(base4[5] == 7);

    core::array<int> array5;
    array5 = {-1,5,6,9,8,7};
    CU_ASSERT(array5[0] == -1);
    CU_ASSERT(array5[1] == 5);
    CU_ASSERT(array5[2] == 6);
    CU_ASSERT(array5[3] == 9);
    CU_ASSERT(array5[4] == 8);
    CU_ASSERT(array5[5] == 7);

    int* base5 = array5;
    CU_ASSERT(base5[0] == -1);
    CU_ASSERT(base5[1] == 5);
    CU_ASSERT(base5[2] == 6);
    CU_ASSERT(base5[3] == 9);
    CU_ASSERT(base5[4] == 8);
    CU_ASSERT(base5[5] == 7);

    constexpr core::array<int,6> array6(core::array<int,6>{-1,5,6,9,8,7});
    CU_ASSERT(array6[0] == -1);
    CU_ASSERT(array6[1] == 5);
    CU_ASSERT(array6[2] == 6);
    CU_ASSERT(array6[3] == 9);
    CU_ASSERT(array6[4] == 8);
    CU_ASSERT(array6[5] == 7);
    CU_ASSERT(array6.back() == 7);

    auto array7 = array6.sub<2,2>();
    //array7.print(std::cout);
    CU_ASSERT(array7.size() == 2)
    CU_ASSERT(array7[0] == 6);
    CU_ASSERT(array7[1] == 9);

    auto array8 = array3.sub(2,2);
    //array8->print(std::cout);
    CU_ASSERT(array8->size() == 2)
    CU_ASSERT(array8->at(0) == 6);
    CU_ASSERT(array8->at(1) == 9);

    core::array<int> array9(base1 + 2,2);
    CU_ASSERT(array9[0] == 6);
    CU_ASSERT(array9[1] == 9);

    core::array<int> array10 {-1,5,6,9,8,7};
    CU_ASSERT(array10[0] == -1);
    CU_ASSERT(array10[1] == 5);
    CU_ASSERT(array10[2] == 6);
    CU_ASSERT(array10[3] == 9);
    CU_ASSERT(array10[4] == 8);
    CU_ASSERT(array10[5] == 7);
    //array10.print(std::cout);
    //std::cout << "\n";
    std::reverse(array10.begin(),array10.end());
    //array10.print(std::cout);
    CU_ASSERT(array10[0] == 7);
    CU_ASSERT(array10[1] == 8);
    CU_ASSERT(array10[2] == 9);
    CU_ASSERT(array10[3] == 6);
    CU_ASSERT(array10[4] == 5);
    CU_ASSERT(array10[5] == -1);
    CU_ASSERT(array10.back() == -1);

    core::array<int,6> array11 {-1,5,6,9,8,7};
    CU_ASSERT(array11[0] == -1);
    CU_ASSERT(array11[1] == 5);
    CU_ASSERT(array11[2] == 6);
    CU_ASSERT(array11[3] == 9);
    CU_ASSERT(array11[4] == 8);
    CU_ASSERT(array11[5] == 7);
    //array11.print(std::cout);
    //std::cout << "\n";
    std::reverse(array11.begin(),array11.end());
    //array11.print(std::cout);
    CU_ASSERT(array11[5] == -1);
    CU_ASSERT(array11[4] == 5);
    CU_ASSERT(array11[3] == 6);
    CU_ASSERT(array11[2] == 9);
    CU_ASSERT(array11[1] == 8);
    CU_ASSERT(array11[0] == 7);

    core::array<float(*)(float)> array12(10);

    core::array<int> array13(array10);
    //array13.print(std::cout);
    CU_ASSERT(array13[0] == 7);
    CU_ASSERT(array13[1] == 8);
    CU_ASSERT(array13[2] == 9);
    CU_ASSERT(array13[3] == 6);
    CU_ASSERT(array13[4] == 5);
    CU_ASSERT(array13[5] == -1);
    array13.push_back(array5);
    CU_ASSERT(array13[0] == 7);
    CU_ASSERT(array13[1] == 8);
    CU_ASSERT(array13[2] == 9);
    CU_ASSERT(array13[3] == 6);
    CU_ASSERT(array13[4] == 5);
    CU_ASSERT(array13[5] == -1);
    CU_ASSERT(array13[6] == -1);
    CU_ASSERT(array13[7] == 5);
    CU_ASSERT(array13[8] == 6);
    CU_ASSERT(array13[9] == 9);
    CU_ASSERT(array13[10] == 8);
    CU_ASSERT(array13[11] == 7);
    //array13.print(std::cout);


    core::array<core::array<int>> array14;
    array14.push_back(array10);
    CU_ASSERT(array14[0][0] == 7);
    CU_ASSERT(array14[0][1] == 8);
    CU_ASSERT(array14[0][2] == 9);
    CU_ASSERT(array14[0][3] == 6);
    CU_ASSERT(array14[0][4] == 5);
    CU_ASSERT(array14[0][5] == -1);
    array14.push_back(array5);
    CU_ASSERT(array14[1][0] == -1);
    CU_ASSERT(array14[1][1] == 5);
    CU_ASSERT(array14[1][2] == 6);
    CU_ASSERT(array14[1][3] == 9);
    CU_ASSERT(array14[1][4] == 8);
    CU_ASSERT(array14[1][5] == 7);

    core::array<core::array<int>> array15 {{1,1,1},{2,2,2},{3,3,3}};
    CU_ASSERT(array15[0][0] == 1);
    CU_ASSERT(array15[0][1] == 1);
    CU_ASSERT(array15[0][2] == 1);
    CU_ASSERT(array15[1][0] == 2);
    CU_ASSERT(array15[1][1] == 2);
    CU_ASSERT(array15[1][2] == 2);
    CU_ASSERT(array15[2][0] == 3);
    CU_ASSERT(array15[2][1] == 3);
    CU_ASSERT(array15[2][2] == 3);

    core::array<core::array<int>> array16 {{4,4,4},{5,5,5},{6,6,6}};

    core::array<core::array<int>> array17;
    array17.push_back(array15);
    array17.push_back(array16);
    CU_ASSERT(array17[0][0] == 1);
    CU_ASSERT(array17[0][1] == 1);
    CU_ASSERT(array17[0][2] == 1);
    CU_ASSERT(array17[1][0] == 2);
    CU_ASSERT(array17[1][1] == 2);
    CU_ASSERT(array17[1][2] == 2);
    CU_ASSERT(array17[2][0] == 3);
    CU_ASSERT(array17[2][1] == 3);
    CU_ASSERT(array17[2][2] == 3);
    CU_ASSERT(array17[3][0] == 4);
    CU_ASSERT(array17[3][1] == 4);
    CU_ASSERT(array17[3][2] == 4);
    CU_ASSERT(array17[4][0] == 5);
    CU_ASSERT(array17[4][1] == 5);
    CU_ASSERT(array17[4][2] == 5);
    CU_ASSERT(array17[5][0] == 6);
    CU_ASSERT(array17[5][1] == 6);
    CU_ASSERT(array17[5][2] == 6);

}
void v3_tree()
{
    enum Types
    {
        data1,
        data2,
        data3,
    };

    core::Node<Types> num1;

}


void v3_cpp()
{
    const char* cstr1 = "Prueba 1...";
    std::string str1 = std::string(cstr1 + 3,5);
    //std::cout << "String : " << str1 << "\n";
    CU_ASSERT(str1.compare("eba 1") == 0);
}

void v3_trails()
{
    //core::convertion_to_real<signed int>::type data1 = std::numeric_limits<signed int>::max();
    //CU_ASSERT((signed int)data1 == std::numeric_limits<signed int>::max());

    //core::convertion_to_real<unsigned int>::type data2 = std::numeric_limits<unsigned int>::max();
    //CU_ASSERT((unsigned int)data2 == (float)std::numeric_limits<unsigned int>::max());

    //core::convertion_to_real<signed long int>::type data3 = std::numeric_limits<signed long int>::max();
    //CU_ASSERT((signed long int)data3 == (float)std::numeric_limits<signed long int>::max());

    //core::convertion_to_real<signed long int>::type data4 = std::numeric_limits<signed long int>::max();
    //CU_ASSERT((signed long int)data4 == (float)std::numeric_limits<signed long int>::max());

    //core::convertion_to_real<unsigned long int>::type data4 = std::numeric_limits<unsigned long int>::max();
    //CU_ASSERT(core::equal(data4, (float)std::numeric_limits<unsigned long int>::max()));

    /*core::convertion_to_real<unsigned long>::type data2 = std::numeric_limits<unsigned long>::max();
    CU_ASSERT(data2 == std::numeric_limits<unsigned long>::max());

    core::convertion_to_real<unsigned long>::type data3 = std::numeric_limits<unsigned long>::max();
    CU_ASSERT(data3 == std::numeric_limits<unsigned long>::max());*/


}

void v3_Number()
{
    core::Number real1 = 1.36f;
    core::Number real2 = -361.6f;
    core::Number real3 = 93.6f;
    CU_ASSERT(core::equal((float)real1,1.36f));
    CU_ASSERT(core::equal((float)real2,-361.6f));
    CU_ASSERT(core::equal((float)real3,93.6f));

    float& real4 = real1;
    real4 = 65.3f;
    CU_ASSERT_FALSE(core::equal((float)real1,1.36f));
    CU_ASSERT(core::equal((float)real1,65.3f));
}


void v3_performance()
{
    constexpr size_t amoung = 1000;
    core::Number numn;
    double numd,res1 = 0,res2 = 0;
    double data[amoung];
    std::random_device rd;
    std::uniform_real_distribution<double> distd(-10.0, 10.0);

    for(size_t i = 0; i < amoung; i++)
    {
        data[i] = distd(rd);
    }
    /*for(size_t i = 0; i < amoung; i++)
    {
        std::cout << i << ".-" << data[i] << "\n";
    }*/

    size_t times = 100000;
    auto start1 = std::chrono::high_resolution_clock::now();
    for(size_t t = 0; t < times; t++)
    {
        for(size_t i = 0; i < amoung; i++)
        {
            numd = data[i];
            res1 += numd;
        }
    }
    auto stop1 = std::chrono::high_resolution_clock::now();
    auto duration1 = std::chrono::duration_cast<std::chrono::milliseconds>(stop1 - start1);
    //std::cout << "Time taken by function: " << duration1.count() << " milliseconds" << std::endl;
    //std::cout << "Resul : " << res1 << "\n";

    res2 = 0;
    auto start2 = std::chrono::high_resolution_clock::now();
    for(size_t t = 0; t < times; t++)
    {
        for(size_t i = 0; i < amoung; i++)
        {
            numn = data[i];
            res2 += (double)numn;
        }
    }
    auto stop2 = std::chrono::high_resolution_clock::now();
    auto duration2 = std::chrono::duration_cast<std::chrono::milliseconds>(stop2 - start2);
    //std::cout << "Time taken by function: " << duration2.count() << " milliseconds" << std::endl;
    //std::cout << "Resul : " << res2 << "\n";
    CU_ASSERT(core::equal(res1,res2))
    double v = (double)duration2.count() / (double)duration1.count();
    //std::cout << "Eficiencia : " << v << "\n";
    CU_ASSERT(v < 1.1)
}
void v3_to_number()
{
    short num1 = core::to_number<short>("1200");
    CU_ASSERT(num1 == 1200);
    short num2 = core::to_number<short>("15300");
    CU_ASSERT(num2 == 15300);
    short num3 = core::to_number<short>("-1200");
    CU_ASSERT(num3 == -1200);
    short num4 = core::to_number<short>("-15300");
    CU_ASSERT(num4 == -15300);
    int num5 = core::to_number<int>("120000");
    CU_ASSERT(num5 == 120000);
    long num6 = core::to_number<long>("12000000");
    CU_ASSERT(num6 == 12000000);

    unsigned short num7 = core::to_number<unsigned short>("1200");
    CU_ASSERT(num7 == 1200);
    unsigned short num8 = core::to_number<unsigned short>("15300");
    CU_ASSERT(num8 == 15300);
}
