@echo off
python generate.py
start "" ".\build\vs2017\helloworld.vcxproj"
pause