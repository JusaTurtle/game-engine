@echo off

mkdir ..\..\build
pushd ..\..\build
call shell
call cl -FC -Zi ..\win32_engine\src\win32_platform.cpp gdi32.lib user32.lib
popd