make:
	g++ src/main.cpp src/myhash.cpp src/blockchain.cpp src/helpers.cpp -o bchain
clear: 
	-del bchain.exe