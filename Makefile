%.o: %.cpp 
    g++ -Werror -std=c++11 -c $< -o $@

main.exe: main.o database.o
    g++ $^ -o $@