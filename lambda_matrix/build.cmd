@echo on
@cls
@setlocal

@REM @set "INCLUDE=D:\machine_wide;%INCLUDE%"

@if [%1] == [clean] goto clean
@if [%1] == [release] goto release
@if [%1] == [debug] goto debug
@echo.
@echo %0 arguments have to be clean, debug or release
@echo.
@goto exit

:debug
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
set CLANG="C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\Llvm\x64\bin\clang-cl.exe"
%CLANG% /std:c++20 /MTd /Zi lambdamatrix.cpp /o out/lambdamatrix.exe
@goto exit

:release
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
set CLANG="C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\Llvm\x64\bin\clang-cl.exe"
%CLANG% /std:c++20 /MT lambdamatrix.cpp /o out/lambdamatrix.exe
@goto exit


:clean
@del out\*.pdb
@del out\*.ilk
@del out\*.exe
@goto exit

:exit
@endlocal
