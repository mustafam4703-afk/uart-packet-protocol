clear
cd build
cmake ..
cmake --build . --target tests
Get-ChildItem -Recurse -Filter "tests.exe" | ForEach-Object { & $_.FullName }
cd ..