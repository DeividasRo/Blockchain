make:
	g++ src/main.cpp src/generator.cpp src/myhash.cpp src/blockchain.cpp -o bchain
clear: 
	-del bchain.exe