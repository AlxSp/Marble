@echo off
pushd ..\
call thirdParty\bin\premake\premake5.exe vs2019
popd
PAUSE