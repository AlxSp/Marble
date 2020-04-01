#!/bin/bash
pushd ..
echo "\nCreating .vscode dir"
mkdir -p .vscode
cp -r ./scripts/vscProjectFiles/. ./.vscode/
echo "\nGenerating GNU make files"
./thirdParty/bin/premake/Linux/premake5 gmake2
popd
read -p "Press enter to continue"
