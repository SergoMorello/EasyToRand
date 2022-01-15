#!/bin/bash
windres app.rc app.res -F pe-i386 -O coff
g++ -ID:/c++/include -static -m32  main.cpp -lcrypto -lssl app.res -o test.exe -s -mwindows
./test.exe