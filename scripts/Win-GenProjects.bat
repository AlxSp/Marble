@echo off
pushd %~dp0\..\
call thirdParty\bin\premake\Windows\premake5.exe vs2019
popd
PAUSE