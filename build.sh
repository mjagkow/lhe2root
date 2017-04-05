g++ -std=c++14 -m64 -march=native -mavx -O3 pugixml/src/pugixml.cpp lhe2root.cpp -o lhe2root `root-config --libs --cflags`
