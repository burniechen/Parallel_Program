EXENAME = main
CC = g++ -std=c++11

ARMA_POS = /usr/local/include
ARMA_FL = -I $(ARMA_POS) -DARMA_DONT_USE_WRAPPER

all: main

main: main.cpp
	$(CC) main.cpp -o $(EXENAME) -O2 $(ARMA_FL) -lblas -llapack -lpthread

clean: 
	rm -f $(EXENAME)
