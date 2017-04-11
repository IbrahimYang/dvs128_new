# dvs128_new

this is a project get picture from dvs128 based on libcaer

to build this project please use following:

g++ -std=c++11 -pedantic -Wall -Wextra -O2 -o dvs128_simple main.cpp dvs128_class.cpp -D_DEFAULT_SOURCE=1 -lcaer
