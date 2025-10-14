#include "StudentDatabase.h"
#include "basicIO.h" 

// Constructor
StudentDatabase::StudentDatabase() : count(0), capacity(10) {
    studentList = new Student*[capacity];
}

// Destructor
StudentDatabase::~StudentDatabase() {
    for (int i = 0; i < count; ++i) {
        delete studentList[i];
    }
    delete[] studentList;
}

// Private helper to resize the list
void StudentDatabase::resize() {
    capacity *= 2; 
    Student** newList = new Student*[capacity];
    for (int i = 0; i < count; ++i) {
        newList[i] = studentList[i];
    }
    delete[] studentList;
    studentList = newList;
}

// Overloaded += operator
void StudentDatabase::operator+=(Student* newStudent) {
    if (count == capacity) {
        resize();
    }
    studentList[count] = newStudent;
    count++;
}

// Overloaded () operator
Student* StudentDatabase::operator()(const MyString& rollNumber) {
    for (int i = 0; i < count; ++i) {
        if (studentList[i]->getRollNumber() == rollNumber) {
            return studentList[i]; 
        }
    }
    return nullptr; 
}

// Displays all students
void StudentDatabase::displayAll() const {
    if (count == 0) {
        io.outputstring("The student database is empty.\n");
        return;
    }
    for (int i = 0; i < count; ++i) {
        MyString details = studentList[i]->toString();
        io.outputstring(details.c_str());
    }
}

// Getters and the swap helper for sorting
int StudentDatabase::getCount() const { return count; }
Student* StudentDatabase::getStudent(int index) const {
    if (index >= 0 && index < count) return studentList[index];
    return nullptr;
}
void StudentDatabase::swap(int index1, int index2) {
    if (index1 >= 0 && index1 < count && index2 >= 0 && index2 < count) {
        Student* temp = studentList[index1];
        studentList[index1] = studentList[index2];
        studentList[index2] = temp;
    }
}

// --- Sorting Implementations ---

void StudentDatabase::sortByRollNumber() {
    if (count < 2) return;
    for (int i = 0; i < count - 1; ++i) {
        for (int j = 0; j < count - i - 1; ++j) {
            if (studentList[j+1]->getRollNumber() < studentList[j]->getRollNumber()) {
                swap(j, j + 1);
            }
        }
    }
}

void StudentDatabase::sortByTotalMarks() {
    if (count < 2) return;
    for (int i = 0; i < count - 1; ++i) {
        for (int j = 0; j < count - i - 1; ++j) {
            if (studentList[j]->calculateTotalMarks() < studentList[j+1]->calculateTotalMarks()) {
                swap(j, j + 1);
            }
        }
    }
}

void StudentDatabase::sortByName() {
    if (count < 2) return;
    Trie nameTrie;
    for (int i = 0; i < count; ++i) {
        nameTrie.insert(studentList[i]->getName());
    }
    int sortedCount = 0;
    MyString* sortedNames = nameTrie.getAllNamesSorted(sortedCount);
    Student** sortedStudentList = new Student*[count];
    for (int i = 0; i < sortedCount; ++i) {
        for (int j = 0; j < count; ++j) {
            if (studentList[j] != nullptr && studentList[j]->getName() == sortedNames[i]) {
                sortedStudentList[i] = studentList[j];
                studentList[j] = nullptr;
                break;
            }
        }
    }
    delete[] studentList;
    studentList = sortedStudentList;
    delete[] sortedNames;
}