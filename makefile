
all: clean compile link


link:
	g++ -o bin/warehouse bin/Action.o bin/Customer.o bin/main.o bin/Order.o bin/Volunteer.o bin/WareHouse.o


compile:
	g++ -g -Wall -Weffc++ -std=c++11 -I./include -c -o bin/Action.o src/Action.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -I./include -c -o bin/Customer.o src/Customer.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -I./include -c -o bin/main.o src/main.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -I./include -c -o bin/Order.o src/Order.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -I./include -c -o bin/Volunteer.o src/Volunteer.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -I./include -c -o bin/WareHouse.o src/WareHouse.cpp


clean:
	rm -f bin/*.o
