# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -g

# The final executable name
TARGET = student_manager

# List of all object files required for the final executable
OBJS = main.o Student.o StudentDatabase.o Trie.o MyString.o basicIO.o

# The default rule: build the final program
all: $(TARGET)

# Rule to link all object files into the final executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# --- Compilation rules for each source file ---

main.o: main.cpp StudentDatabase.h Exceptions.h basicIO.h
	$(CXX) $(CXXFLAGS) -c main.cpp

StudentDatabase.o: StudentDatabase.cpp StudentDatabase.h Student.h Trie.h
	$(CXX) $(CXXFLAGS) -c StudentDatabase.cpp

Student.o: Student.cpp Student.h MyString.h
	$(CXX) $(CXXFLAGS) -c Student.cpp

Trie.o: Trie.cpp Trie.h MyString.h
	$(CXX) $(CXXFLAGS) -c Trie.cpp

MyString.o: MyString.cpp MyString.h
	$(CXX) $(CXXFLAGS) -c MyString.cpp

basicIO.o: basicIO.cpp basicIO.h
	$(CXX) $(CXXFLAGS) -c basicIO.cpp

# --- Clean rule to remove generated files ---
clean:
	rm -f $(OBJS) $(TARGET)