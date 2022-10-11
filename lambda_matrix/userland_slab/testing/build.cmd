@echo on
@cls
@setlocal

@REM @set "INCLUDE=D:\machine_wide;%INCLUDE%"

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
@REM %CLANG% /std:c17 /TC /MTd /Zi /D_HAS_EXCEPTIONS=0 userland_slab_test_main.c /GR- /o out/userland_slab_test_main.exe
set CLANG="C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\Llvm\x64\bin\clang.exe"
%CLANG% -std=c17 -g -D_HAS_EXCEPTIONS=0 userland_slab_test_main.c --output out/userland_slab_test_main.exe
@goto exit

:release
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
set CLANG="C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\Llvm\x64\bin\clang-cl.exe"
%CLANG% /std:c17 /Tc /MT userland_slab_test_main.c /D_HAS_EXCEPTIONS=0 /GR- /o out/userland_slab_test_main.exe
@goto exit


:clean
@del out\*.pdb >NUL
@del out\*.ilk >NUL
@del out\*.exe >NUL
@goto exit

:exit
@endlocal
