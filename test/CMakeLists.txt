
add_executable (example1 example1.cpp)
add_executable (example2 example2.cpp)
add_executable (example3 example3.cpp)
add_executable (copy copy.cpp)
add_executable (comparison comparison.cpp)
add_executable (waterlevel waterlevel.cpp)
add_executable (if_stmt if_stmt.cpp)
add_executable(while_stmt while_stmt.cpp)


target_link_libraries (example1 aadd)
target_link_libraries (example2 aadd)
target_link_libraries (example3 aadd)
target_link_libraries (copy aadd)
target_link_libraries (comparison aadd)
target_link_libraries (waterlevel aadd)
target_link_libraries (if_stmt aadd)
target_link_libraries (while_stmt aadd)