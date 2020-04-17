#!/bin/bash
pushd "$(dirname "$0")/.."

echo -e "Creating .vscode dir"
mkdir -p .vscode
cp -r ./scripts/vscProjectFiles/. ./.vscode/

echo "Generating GNU make files"
./thirdParty/bin/premake/Linux/premake5 gmake2

popd
read -p "Press enter to continue"
