#ifndef STUDENTDATABASE_H
#define STUDENTDATABASE_H

#include "Student.h"
#include "Trie.h"

class StudentDatabase {
private:
    Student** studentList; 
    int count;             
    int capacity;          
    const char* filename;

    void resize();
    void loadFromFile();

public:
    StudentDatabase(const char* db_filename);
    ~StudentDatabase();

    // --- File Operation ---
    void saveToFile() const; // Now public

    // --- Operator Overloading ---
    void operator+=(Student* newStudent);
    Student* operator()(const MyString& rollNumber);

    // --- Utility Functions ---
    void displayAll() const;
    int getCount() const;
    Student* getStudent(int index) const;
    void swap(int index1, int index2);

    // --- Sorting Functions ---
    void sortByRollNumber();
    void sortByTotalMarks();
    void sortByName();
};

#endif // STUDENTDATABASE_H