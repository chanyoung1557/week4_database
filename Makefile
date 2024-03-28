datavase.exe:datavase.o d1.o d2.o
	g++ ${FLAGES} -o datavase.o d1.o d2.o
datavase.o: datavase.cpp
	g++ -c datavase.cpp
d1.o: d1.cpp
d2.o: d2.cpp
