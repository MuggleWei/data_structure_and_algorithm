cmake_minimum_required(VERSION 2.8.9)

# source and head file of data structure
file(GLOB DATA_STRUCTURE_CPP_HEADER inc/*.h) 

# install header files
install(FILES ${DATA_STRUCTURE_CPP_HEADER} DESTINATION ${DSAA_INSTALL_INCLUDE_PATH}/cpp)