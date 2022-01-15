@echo off
call windres app.rc app.res -F pe-i386 -O coff

call g++ -ID:\\c++\include -static -m32  main.cpp -lcrypto -lssl app.res -o test.exe -s -mwindows
call test.exe