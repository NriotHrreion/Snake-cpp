@echo off
mkdir build
cd build
cmake .. -DSDL_SHARED=OFF -DSDL_STATIC=ON
cmake --build .
