CC = g++
FLAGS = -Ofast -DNDEBUG -std=c++11 -DHAVE_CXX0X -march=native -fpic -w -ftree-vectorize
prog = hnsw
test_prog = test_prog
objects = hnsw.o
test_objects = test.o

all: $(prog) $(test_prog)

$(prog): $(objects)
	$(CC) $(objects) -o $(prog) $(FLAGS)

$(test_prog): $(test_objects)
	$(CC) $(test_objects) -o $(test_prog) $(FLAGS)

hnsw.o: hnsw.cpp src/*.hpp
	$(CC) -c hnsw.cpp -o hnsw.o $(FLAGS)

test.o: test.cpp src/*.hpp
	$(CC) -c test.cpp -o test.o $(FLAGS)

grade: $(prog)
ifeq ($(OS), Windows_NT)
	@.\hnsw.exe
else
	@./hnsw
endif

test: $(test_prog)
ifeq ($(OS), Windows_NT)
	@.\test_prog.exe
else
	@./test_prog
endif

clean:
ifeq ($(OS), Windows_NT)
	@del /q .\*.o 2>nul
	@del /q .\$(prog).exe 2>nul
	@del /q .\$(test_prog).exe 2>nul
else
	@rm -rf ./*.o $(prog)
	@rm -rf ./*.o $(test_prog)
endif
