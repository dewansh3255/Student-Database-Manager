#include "StudentDatabase.h"
#include "basicIO.h"
#include "MyString.h"
#include "Trie.h"

MyString floatToMyString(float f);
float stringToFloat(const MyString& s);
void displayStudentDetails(const Student* s);

const int INITIAL_CAPACITY = 10;
const int FILE_BUFFER_SIZE = 4096;
const int MAX_LINES = 500;
const int MAX_LINE_PARTS = 2;


// --- Constructor & Destructor ---
StudentDatabase::StudentDatabase(const char* db_filename) 
    : studentList(new Student*[INITIAL_CAPACITY]), 
      count(0), 
      capacity(INITIAL_CAPACITY), 
      filename(db_filename) {
    loadFromFile();
}

StudentDatabase::~StudentDatabase() {
    for (int i = 0; i < count; ++i) {
        delete studentList[i];
    }
    delete[] studentList;
}

// --- Memory Management ---
void StudentDatabase::resize() {
    capacity *= 2;
    Student** newList = new Student*[capacity];
    for (int i = 0; i < count; ++i) {
        newList[i] = studentList[i];
    }
    delete[] studentList;
    studentList = newList;
}

// --- Operator Overloading ---
void StudentDatabase::operator+=(Student* newStudent) {
    if (count == capacity) {
        resize();
    }
    studentList[count] = newStudent;
    count++;
}

Student* StudentDatabase::operator()(const MyString& rollNumber) {
    for (int i = 0; i < count; ++i) {
        if (studentList[i]->getRollNumber() == rollNumber) {
            return studentList[i];
        }
    }
    return nullptr;
}

// --- Utility Functions ---
void StudentDatabase::displayAll() const {
    if (count == 0) {
        io.outputstring("Database is empty.\n");
        return;
    }
    for (int i = 0; i < count; ++i) {
        displayStudentDetails(studentList[i]);
    }
}

int StudentDatabase::getCount() const { return count; }
Student* StudentDatabase::getStudent(int index) const {
    if (index >= 0 && index < count) {
        return studentList[index];
    }
    return nullptr;
}

void StudentDatabase::swap(int index1, int index2) {
    Student* temp = studentList[index1];
    studentList[index1] = studentList[index2];
    studentList[index2] = temp;
}

// --- File Operations ---
void StudentDatabase::loadFromFile() {
    long fd = io.open_file(filename, 0, 0);
    if (fd < 0) {
        io.errorstring("No existing database file found. A new one will be created on save.\n");
        return;
    }

    char readBuffer[FILE_BUFFER_SIZE + 1];
    MyString content;
    long bytesRead = 0;

    do {
        bytesRead = io.read_file(fd, readBuffer, FILE_BUFFER_SIZE);
        if (bytesRead > 0) {
            readBuffer[bytesRead] = '\0';
            content.concat(MyString(readBuffer));
        }
    } while (bytesRead > 0);

    io.close_file(fd);

    if (content.size() == 0) {
        return;
    }

    MyString* lines = new MyString[MAX_LINES];
    int numLines = 0;
    
    numLines = content.split("\n", lines); 

    Student* currentStudent = nullptr;
    MyString roll, name, level, branchStr;
    Branch branch = CSE;

    for (int i = 0; i < numLines; ++i) {
        MyString line = lines[i];
        line.trim();

        if (line.size() == 0) continue;

        MyString* parts = new MyString[MAX_LINE_PARTS];
        
        int numParts = line.split(":", parts); 

        if (numParts == 2) {
            parts[1].trim(); 

            if (parts[0].find("Roll Number") != -1) {
                roll = parts[1];
            } else if (parts[0].find("Name") != -1) {
                name = parts[1];
            } else if (parts[0].find("Level") != -1) {
                level = parts[1];
            } else if (parts[0].find("Branch") != -1) {
                branchStr = parts[1];
            
            // --- FIX IS HERE ---
            } else if (parts[0].find("Class Assessment") != -1) {
                if (currentStudent) currentStudent->setComponentMark(CLASS_ASSESSMENT, stringToFloat(parts[1]));
            } else if (parts[0].find("Quiz") != -1) {
                if (currentStudent) currentStudent->setComponentMark(QUIZ, stringToFloat(parts[1]));
            } else if (parts[0].find("Mid-Sem") != -1) { // UNIFIED
                if (currentStudent) currentStudent->setComponentMark(MIDSEM, stringToFloat(parts[1]));
            } else if (parts[0].find("End-Sem") != -1) { // UNIFIED
                if (currentStudent) currentStudent->setComponentMark(ENDSEM, stringToFloat(parts[1]));
            }
            // --- END FIX ---
        }

        if (line.find("-----------------") != -1) {
            if (roll.size() > 0) {
                branch = (branchStr == "ECE") ? ECE : CSE;

                if (level == "BTech") currentStudent = new BTechStudent(roll.c_str(), name.c_str(), branch);
                else if (level == "MTech") currentStudent = new MTechStudent(roll.c_str(), name.c_str(), branch);
                else if (level == "PhD") currentStudent = new PhDStudent(roll.c_str(), name.c_str(), branch);
                
                if (currentStudent) {
                    *this += currentStudent;
                }
            }
            currentStudent = nullptr;
            roll.clear(); name.clear(); level.clear(); branchStr.clear();
        }
        delete[] parts;
    }
    delete[] lines;
}


void StudentDatabase::saveToFile() const {
    long fd = io.open_file(filename, 66, 0644);
    if (fd < 0) {
        io.errorstring("Error: Could not open file for writing.\n");
        return;
    }

    for (int i = 0; i < count; ++i) {
        MyString studentData = studentList[i]->toString();
        io.write_file(fd, studentData.c_str(), studentData.size());
    }

    io.close_file(fd);
    io.outputstring("Data successfully saved to file.\n");
}


// --- Sorting Functions  ---

void StudentDatabase::sortByName() {
    if (count == 0) return;

    Trie nameTrie;
    for (int i = 0; i < count; i++) {
        nameTrie.insert(studentList[i]->getName());
    }

    int sortedNameCount = 0;
    MyString* sortedNames = nameTrie.getAllNamesSorted(sortedNameCount);

    Student** sortedStudentList = new Student*[capacity];
    int sortedListIndex = 0;

    for (int i = 0; i < sortedNameCount; i++) {
        MyString sortedNameNoSpace = sortedNames[i];
        
        for (int j = 0; j < count; j++) {
            
            MyString studentNameLower = studentList[j]->getName();
            studentNameLower.toLowercase();
            studentNameLower.trim();

            MyString studentNameNoSpace;
            const char* cstr = studentNameLower.c_str();
            for (int k = 0; k < studentNameLower.size(); k++) {
                if (cstr[k] >= 'a' && cstr[k] <= 'z') {
                    char temp[2] = {cstr[k], '\0'};
                    studentNameNoSpace.concat(MyString(temp));
                }
            }

            if (studentNameNoSpace == sortedNameNoSpace) {
                if (sortedListIndex < capacity) {
                    sortedStudentList[sortedListIndex] = studentList[j];
                    sortedListIndex++;
                }
                break; 
            }
        }
    }

    if (sortedListIndex != count) {
        io.errorstring("Error: Name sort failed. Mismatch in student count. Aborting sort.\n");
        delete[] sortedStudentList;
        delete[] sortedNames;
        return;
    }

    delete[] studentList;
    studentList = sortedStudentList;
    delete[] sortedNames;

    io.outputstring("Database sorted by Name.\n");
}


void StudentDatabase::sortByRollNumber() {
    for (int i = 0; i < count - 1; ++i) {
        for (int j = 0; j < count - i - 1; ++j) {
            if (studentList[j+1]->getRollNumber() < studentList[j]->getRollNumber()) {
                swap(j, j + 1);
            }
        }
    }
}

void StudentDatabase::sortByTotalMarks() {
    for (int i = 0; i < count - 1; ++i) {
        for (int j = 0; j < count - i - 1; ++j) {
            if (studentList[j]->calculateTotalMarks() < studentList[j+1]->calculateTotalMarks()) {
                swap(j, j + 1);
            }
        }
    }
}