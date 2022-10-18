make:
	g++ src/main.cpp src/user.cpp src/generator.cpp src/myhash.cpp -o prog
clear: 
	-del prog.exe