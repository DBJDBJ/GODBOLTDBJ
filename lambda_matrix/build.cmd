@echo on
@cls
@setlocal

@REM @set "INCLUDE=D:\machine_wide;%INCLUDE%"

call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
set CLANG="C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\Llvm\x64\bin\clang-cl.exe"

@if [%1] == [clean] goto clean
@REM @if [%1] == [init] goto init
@if [%1] == [release] goto release
goto debug
@goto exit

@REM :init
@REM @rem Setup VS2019
@REM @rem change to your local path, if need be
@REM @rem C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE
@REM call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
@REM @goto exit

:debug
%CLANG% /std:c++17 /MTd /Zi main.cpp /o out/lambdamatrix.exe
@goto exit

:release
%CLANG% /std:c++17 /MT /O2 main.cpp /o out/lambdamatrix.exe
@goto exit


:clean
@del out\*.pdb
@del out\*.ilk
@del out\*.exe
@goto exit

:exit
@endlocal
