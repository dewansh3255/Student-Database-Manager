#ifndef STUDENT_H
#define STUDENT_H

#include "MyString.h"

// Enum to represent student branches
enum Branch { CSE, ECE };

// Enum for the specific mark categories
enum MarkComponent { CLASS_ASSESSMENT, QUIZ, MIDSEM, ENDSEM, NUM_COMPONENTS };

class Student {
private:
    MyString rollNumber;
    MyString name;
    Branch branch;
    float componentMarks[NUM_COMPONENTS]; // Use the enum for a fixed-size array

public:
    // Constructors and a virtual destructor
    Student();
    // Constructor now doesn't need totalComponents
    Student(const char* roll, const char* studentName, Branch studentBranch); 
    Student(const Student& other);
    virtual ~Student();

    // Operator overloading for assignment
    Student& operator=(const Student& other);

    // --- Core Functionality ---
    virtual MyString getLevel() const = 0;
    float calculateTotalMarks() const;
    MyString toString() const;

    // --- Getters and Setters ---
    const MyString& getRollNumber() const;
    const MyString& getName() const;
    Branch getBranch() const;
    float getComponentMark(int index) const;
    // getNumComponents is no longer needed as it's fixed
    
    void setComponentMark(int index, float mark);
};

// --- Derived Classes ---
// (Constructors updated)

class BTechStudent : public Student {
public:
    BTechStudent(const char* roll, const char* name, Branch branch);
    virtual MyString getLevel() const override;
};

class MTechStudent : public Student {
public:
    MTechStudent(const char* roll, const char* name, Branch branch);
    virtual MyString getLevel() const override;
};

class PhDStudent : public Student {
public:
    PhDStudent(const char* roll, const char* name, Branch branch);
    virtual MyString getLevel() const override;
};


#endif // STUDENT_H