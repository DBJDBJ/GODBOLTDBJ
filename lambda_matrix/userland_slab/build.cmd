@echo on
@cls
@setlocal

@REM build a lib

@if [%1] == [c] goto clean
@if [%1] == [clean] goto clean
@if [%1] == [r] goto release
@if [%1] == [release] goto release
@if [%1] == [d] goto debug
@if [%1] == [debug] goto debug
@echo.
@echo %0 arguments have to be c,clean, d,debug or r,release
@echo.
@goto exit

@REM https://stackoverflow.com/a/65513682/10870835

:debug
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
@REM set CLANG="C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\Llvm\x64\bin\clang-cl.exe"
set CLANG="C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\Llvm\x64\bin\clang"
%CLANG% -std=c17 -c -Wall src/slab.c src/mmap_windows.c -o userlandslab.o
@goto exit

:release
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
set CLANG="C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\Llvm\x64\bin\clang"
%CLANG% -std:c17 -Wall src/slab.c src/mmap_windows.c -c -o out/userlandslab.o
@goto exit


:clean
@del out\*.pdb
@del out\*.ilk
@del out\*.exe
@goto exit

:exit
@endlocal
