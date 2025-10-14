#include "StudentDatabase.h"
#include "basicIO.h" 

// --- FIX IS HERE ---
// Forward declarations for the helper functions defined in Student.cpp
float stringToFloat(const MyString& s);
MyString floatToMyString(float f); // This line fixes the error

// Constructor and Destructor (unchanged)
StudentDatabase::StudentDatabase(const char* db_filename) : count(0), capacity(10), filename(db_filename) {
    studentList = new Student*[capacity];
    loadFromFile();
}
StudentDatabase::~StudentDatabase() {
    for (int i = 0; i < count; ++i) {
        delete studentList[i];
    }
    delete[] studentList;
}

// --- saveToFile (Correctly saves marks) ---
void StudentDatabase::saveToFile() const {
    long fd = io.open_file(filename, 0101, 0644); // O_WRONLY | O_CREAT
    if (fd < 0) {
        io.errorstring("Error: Could not open or create the database file for writing.\n");
        return;
    }

    for (int i = 0; i < count; ++i) {
        Student* s = studentList[i];
        MyString line(s->getRollNumber());
        line.concat(",");
        line.concat(s->getName());
        line.concat(",");
        line.concat(s->getLevel());
        line.concat(",");
        line.concat(s->getBranch() == CSE ? "CSE" : "ECE");

        // Append all the marks to the line
        for (int j = 0; j < NUM_COMPONENTS; ++j) {
            line.concat(",");
            line.concat(floatToMyString(s->getComponentMark(j)));
        }
        
        line.concat("\n");
        io.write_file(fd, line.c_str(), line.size());
    }

    io.close_file(fd);
    io.outputstring("Data successfully saved to file.\n");
}

// --- loadFromFile (Correctly loads marks) ---
void StudentDatabase::loadFromFile() {
    long fd = io.open_file(filename, 0, 0); // O_RDONLY = 0
    if (fd < 0) return;

    // Clear existing students to prevent duplicates on load
    for(int i = 0; i < count; ++i) delete studentList[i];
    count = 0;

    char buffer[2048];
    long bytesRead = io.read_file(fd, buffer, 2047);
    io.close_file(fd);

    if (bytesRead <= 0) return;
    buffer[bytesRead] = '\0';
    
    MyString content(buffer);
    MyString* lines = nullptr;
    int numLines = content.split("\n", lines);

    for (int i = 0; i < numLines; ++i) {
        if(lines[i].size() == 0) continue; // Skip empty lines

        MyString* parts = nullptr;
        int numParts = lines[i].split(",", parts);

        // We now expect 4 details + 4 marks = 8 parts
        if (numParts >= 4 + NUM_COMPONENTS) {
            MyString roll(parts[0]);
            MyString name(parts[1]);
            MyString levelStr(parts[2]);
            MyString branchStr(parts[3]);
            Branch branch = (branchStr == "CSE") ? CSE : ECE;
            
            Student* newStudent = nullptr;
            if (levelStr == "BTech") {
                newStudent = new BTechStudent(roll.c_str(), name.c_str(), branch);
            } else if (levelStr == "MTech") {
                newStudent = new MTechStudent(roll.c_str(), name.c_str(), branch);
            } else {
                newStudent = new PhDStudent(roll.c_str(), name.c_str(), branch);
            }

            if (newStudent) {
                // Load the marks from the file
                for (int j = 0; j < NUM_COMPONENTS; ++j) {
                    float mark = stringToFloat(parts[4 + j]);
                    newStudent->setComponentMark(j, mark);
                }
                (*this) += newStudent;
            }
        }
        delete[] parts;
    }
    delete[] lines;

    // Automatically sort by roll number after loading
    this->sortByRollNumber();
}

// ... (The rest of StudentDatabase.cpp is unchanged) ...
void StudentDatabase::resize() { capacity *= 2; Student** newList = new Student*[capacity]; for (int i = 0; i < count; ++i) newList[i] = studentList[i]; delete[] studentList; studentList = newList; }
void StudentDatabase::operator+=(Student* newStudent) { if (count == capacity) resize(); studentList[count++] = newStudent; }
Student* StudentDatabase::operator()(const MyString& rollNumber) { for (int i = 0; i < count; ++i) if (studentList[i]->getRollNumber() == rollNumber) return studentList[i]; return nullptr; }
void StudentDatabase::displayAll() const { if (count == 0) { io.outputstring("The student database is empty.\n"); return; } for (int i = 0; i < count; ++i) io.outputstring(studentList[i]->toString().c_str()); }
int StudentDatabase::getCount() const { return count; }
Student* StudentDatabase::getStudent(int index) const { if (index >= 0 && index < count) return studentList[index]; return nullptr; }
void StudentDatabase::swap(int i, int j) { Student* temp = studentList[i]; studentList[i] = studentList[j]; studentList[j] = temp; }
void StudentDatabase::sortByRollNumber() { for (int i = 0; i < count - 1; ++i) for (int j = 0; j < count - i - 1; ++j) if (studentList[j+1]->getRollNumber() < studentList[j]->getRollNumber()) swap(j, j + 1); }
void StudentDatabase::sortByTotalMarks() { for (int i = 0; i < count - 1; ++i) for (int j = 0; j < count - i - 1; ++j) if (studentList[j]->calculateTotalMarks() < studentList[j+1]->calculateTotalMarks()) swap(j, j + 1); }
void StudentDatabase::sortByName() { Trie nameTrie; for (int i = 0; i < count; ++i) nameTrie.insert(studentList[i]->getName()); int sortedCount = 0; MyString* sortedNames = nameTrie.getAllNamesSorted(sortedCount); Student** sortedStudentList = new Student*[count]; for (int i = 0; i < sortedCount; ++i) for (int j = 0; j < count; ++j) if (studentList[j] != nullptr && studentList[j]->getName() == sortedNames[i]) { sortedStudentList[i] = studentList[j]; studentList[j] = nullptr; break; } delete[] studentList; studentList = sortedStudentList; delete[] sortedNames; }