# CMakeList.txt: proyecto de CMake para v0, incluya el origen y defina
# la lógica específica del proyecto aquí.
#
cmake_minimum_required (VERSION 3.8)

# Habilite Recarga activa para los compiladores de MSVC si se admiten.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("v0")

# Agregue un origen al ejecutable de este proyecto.
add_executable (v0 "v0.cpp" "v0.h")
add_executable (helloword "hellobrowser.c")

if (CMAKE_VERSION VERSION_GREATER 3.8)
  set_property(TARGET v0 PROPERTY CXX_STANDARD 20)
endif()


find_library (MICRO NAMES libmicrohttpd PATHS  "C:/Users/Azael/vcpkg/packages/libmicrohttpd_x64-windows/lib")
