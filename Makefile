all:
	@g++ -c files.cpp copy.cpp
	@g++ files.o copy.o -o run