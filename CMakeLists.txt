cmake_minimum_required (VERSION 3.1) # 3.1 adds C_STANDARD
project (my-gnu-formatting LANGUAGES CXX)
add_executable (my-gnu-formatting my-gnu-formatting.cpp)
set_property (TARGET my-gnu-formatting PROPERTY CXX_STANDARD 11)
install (TARGETS my-gnu-formatting DESTINATION bin)
