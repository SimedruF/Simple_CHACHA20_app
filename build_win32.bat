@REM Build for Visual Studio compiler. Run your copy of vcvars32.bat or vcvarsall.bat to setup command-line compiler.
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
@set OUT_DIR=Debug
@set OUT_EXE=Chacha20App
@set INCLUDES=/I.\imgui\ /I.\imgui\backends\ /I "%DXSDK_DIR%/Include" /I.\chacha_ex\
@set SOURCES= chacha.c main.cpp chacha20.cpp Chacha20App.cpp .\imgui\backends\imgui_impl_dx9.cpp .\imgui\backends\imgui_impl_win32.cpp .\imgui\imgui*.cpp .\chacha_ex\chacha_ex.c .\chacha_ex\hex.c 
@set LIBS=/LIBPATH:"%DXSDK_DIR%/Lib/x86" d3d9.lib
mkdir %OUT_DIR%
cl /nologo /Zi /MD /utf-8 %INCLUDES% /D UNICODE /D _UNICODE %SOURCES% /Fe%OUT_DIR%/%OUT_EXE%.exe /Fo%OUT_DIR%/ /link %LIBS%
