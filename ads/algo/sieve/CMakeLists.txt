set(OBJ_LIB_NAME sieve)

add_library(${OBJ_LIB_NAME}_objs OBJECT sieve.cpp sieve.hpp)

set_lib_build_flags(${OBJ_LIB_NAME}_objs)
