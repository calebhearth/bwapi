@echo off
cd "..\Interface Packer"
del include\BWAPI\*.* /q
"..\debug\interface packer.exe" "BWAPI1.ini"
rem @pause