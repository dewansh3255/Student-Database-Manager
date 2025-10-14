#ifndef STUDENTDATABASE_H
#define STUDENTDATABASE_H

#include "Student.h"
#include "Trie.h" // Make sure Trie is included

class StudentDatabase {
private:
    Student** studentList; 
    int count;             
    int capacity;          

    void resize();

public:
    // Constructor and Destructor
    StudentDatabase();
    ~StudentDatabase();

    // --- Operator Overloading ---
    void operator+=(Student* newStudent);
    Student* operator()(const MyString& rollNumber);

    // --- Utility Functions ---
    void displayAll() const;
    int getCount() const;
    Student* getStudent(int index) const;
    void swap(int index1, int index2);

    // --- Sorting Functions (as per Assignment Question 4) ---
    void sortByRollNumber();
    void sortByTotalMarks();
    void sortByName();
};

#endif // STUDENTDATABASE_H