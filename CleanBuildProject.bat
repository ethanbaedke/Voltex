del *.vs
del /s *.vcxproj*
rmdir /s /q bin
rmdir /s /q bin-int
premake5 vs2022
pause