main: main.o graphe.o solution.o
	g++ -o main main.o graphe.o solution.o -O3

graphe.o: graphe.cpp graphe.hpp
	g++ -c graphe.cpp -O3

solution.o: solution.cpp solution.hpp
	g++ -c solution.cpp -O3

main.o: main.cpp graphe.hpp solution.hpp
	g++ -c main.cpp -O3 