# dvs128_new

Ibrahim Yang, CBICR, Tsinghua University

this is a c++ project get video from dvs128 based on libcaer

to build this project please use following:

## build(within opencv)
g++ -std=c++11 -pedantic -Wall -Wextra -O2 -o dvs128_simple main.cpp dvs128_class.cpp -D_DEFAULT_SOURCE=1 `pkg-config --cflags --libs opencv` -lcaer

# build(without opencv)
g++ -std=c++11 -pedantic -Wall -Wextra -O2 -o dvs128_simple main.cpp dvs128_class.cpp -D_DEFAULT_SOURCE=1 -lcaer
