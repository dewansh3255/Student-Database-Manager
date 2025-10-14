#ifndef STUDENT_H
#define STUDENT_H

#include "MyString.h"

// Enum to represent student branches
enum Branch { CSE, ECE };

class Student {
private:
    MyString rollNumber;
    MyString name;
    Branch branch;
    float* componentMarks;
    int numComponents;

public:
    // Constructors and a virtual destructor (essential for polymorphism)
    Student();
    Student(const char* roll, const char* studentName, Branch studentBranch, int totalComponents);
    Student(const Student& other); // Copy Constructor
    virtual ~Student(); // Virtual destructor

    // Operator overloading for assignment
    Student& operator=(const Student& other);

    // --- Core Functionality ---
    
    // Pure virtual function to get the student's level (BTech, MTech, etc.)
    // This makes the Student class abstract.
    virtual MyString getLevel() const = 0;

    // Calculates the total marks from all components (fulfills Question 3)
    float calculateTotalMarks() const;
    
    // Generates a string representation of the student's details
    MyString toString() const;

    // --- Getters and Setters ---
    const MyString& getRollNumber() const;
    const MyString& getName() const;
    float getComponentMark(int index) const;
    int getNumComponents() const;

    void setComponentMark(int index, float mark);
};

// --- Derived Classes for Specific Student Levels ---

class BTechStudent : public Student {
public:
    BTechStudent(const char* roll, const char* name, Branch branch, int numComponents);
    virtual MyString getLevel() const override;
};

class MTechStudent : public Student {
public:
    MTechStudent(const char* roll, const char* name, Branch branch, int numComponents);
    virtual MyString getLevel() const override;
};

class PhDStudent : public Student {
public:
    PhDStudent(const char* roll, const char* name, Branch branch, int numComponents);
    virtual MyString getLevel() const override;
};


#endif // STUDENT_H