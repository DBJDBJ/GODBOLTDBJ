@echo on
@cls
@setlocal

@REM @set "INCLUDE=D:\machine_wide;%INCLUDE%"

@if [%1] == [clean] goto clean

call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
set CLANG="C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\Llvm\x64\bin\clang-cl.exe"

@if [%1] == [release] goto release
goto debug
@goto exit

:debug
%CLANG% /std:c++20 /MTd /Zi lambdamatrix.cpp /o out/lambdamatrix.exe
@goto exit

:release
%CLANG% /std:c++20 /MT /O2 lambdamatrix.cpp /o out/lambdamatrix.exe
@goto exit


:clean
@del out\*.pdb
@del out\*.ilk
@del out\*.exe
@goto exit

:exit
@endlocal
