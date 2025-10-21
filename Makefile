CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pthread -O2

TARGETS = exercise1 exercise2 exercise3 exercise4 exercise5

all: $(TARGETS)

exercise1: exercise1.cpp ThreadSafeQueue.h
	$(CXX) $(CXXFLAGS) -o exercise1 exercise1.cpp

exercise2: exercise2.cpp
	$(CXX) $(CXXFLAGS) -o exercise2 exercise2.cpp

exercise3: exercise3.cpp ThreadSafeQueue.h ThreadSafeStack.h
	$(CXX) $(CXXFLAGS) -o exercise3 exercise3.cpp

exercise4: exercise4.cpp
	$(CXX) $(CXXFLAGS) -o exercise4 exercise4.cpp

exercise5: exercise5.cpp
	$(CXX) $(CXXFLAGS) -o exercise5 exercise5.cpp

clean:
	rm -f $(TARGETS)

run1: exercise1
	./exercise1

run2: exercise2
	./exercise2

run3: exercise3
	./exercise3

run4: exercise4
	./exercise4

run5: exercise5
	./exercise5

run_all: all
	@echo "=== Running Exercise 1 ==="
	@./exercise1
	@echo "\n=== Running Exercise 2 ==="
	@./exercise2
	@echo "\n=== Running Exercise 3 ==="
	@./exercise3
	@echo "\n=== Running Exercise 4 ==="
	@./exercise4
	@echo "\n=== Running Exercise 5 ==="
	@./exercise5

.PHONY: all clean run1 run2 run3 run4 run5 run_all