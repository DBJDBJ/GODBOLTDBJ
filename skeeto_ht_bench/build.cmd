@echo on
@cls
@setlocal

@REM @set "INCLUDE=D:\machine_wide;%INCLUDE%"
@set "sourcename=skeeto_ht_bench"

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
set CLANG="C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\Llvm\x64\bin\clang-cl.exe"
%CLANG% /std:c++20 /MTd /Zi /D_HAS_EXCEPTIONS=0 %sourcename%.cpp /GR- /o ../bin/%sourcename%.exe
@goto exit

:release
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
set CLANG="C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\Llvm\x64\bin\clang-cl.exe"
%CLANG% /std:c++20 /MT %sourcename%.cpp /D_HAS_EXCEPTIONS=0 /GR- /o ../bin/%sourcename%.exe
@goto exit


:clean
@del out\*.pdb
@del out\*.ilk
@del out\*.exe
@goto exit

:exit
@endlocal
