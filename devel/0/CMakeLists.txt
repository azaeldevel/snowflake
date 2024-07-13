# CMakeList.txt: proyecto de CMake para developing v0, incluya el origen y defina
# la lógica específica del proyecto aquí.
#
cmake_minimum_required (VERSION 3.8)

# Habilite Recarga activa para los compiladores de MSVC si se admiten.
if (POLICY CMP0141)
  #cmake_policy(SET CMP0141 NEW)
  #set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("developing")

# Agregue un origen al ejecutable de este proyecto.
add_executable (hellobrowser "hellobrowser.c")
add_executable (simplepost "simplepost.c")
add_executable (basicauthentication "basicauthentication.c")
add_executable (tlsauthentication "tlsauthentication.c")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  #set_property(TARGET helloword PROPERTY CXX_STANDARD 20)
endif()

add_definitions(-DCMAKE_BASED)
#include (packages.cmake)
##shoul run vcpkg install to build the tree installed
target_link_libraries(hellobrowser "vcpkg_installed/x64-windows/lib/libmicrohttpd-dll")
target_include_directories(hellobrowser PUBLIC "vcpkg_installed/x64-windows/include")
target_link_libraries(simplepost "vcpkg_installed/x64-windows/lib/libmicrohttpd-dll")
target_include_directories(simplepost PUBLIC "vcpkg_installed/x64-windows/include")
target_link_libraries(basicauthentication "vcpkg_installed/x64-windows/lib/libmicrohttpd-dll")
target_include_directories(basicauthentication PUBLIC "vcpkg_installed/x64-windows/include")
target_link_libraries(tlsauthentication "vcpkg_installed/x64-windows/lib/libmicrohttpd-dll")
target_include_directories(tlsauthentication PUBLIC "vcpkg_installed/x64-windows/include")