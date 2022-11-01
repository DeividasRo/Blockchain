make:
	g++ src/main.cpp src/myhash.cpp src/blockchain.cpp src/helpers.cpp -fopenmp -o bchain
clear: 
	-del bchain.exe