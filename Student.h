#ifndef STUDENT_H
#define STUDENT_H

#include "MyString.h"

// --- Global Helper Prototypes ---
float stringToFloat(const MyString& s);
MyString floatToMyString(float f);
// --- End Helper Prototypes ---

// Enum to represent student branches
enum Branch { CSE, ECE };

// Enum for the specific mark categories
enum MarkComponent { CLASS_ASSESSMENT, QUIZ, MIDSEM, ENDSEM, NUM_COMPONENTS };

class Student {
private:
    MyString rollNumber;
    MyString name;
    Branch branch;
    float componentMarks[NUM_COMPONENTS];

public:
    // Constructors and a virtual destructor
    Student();
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
    char getGrade() const;    
    void setComponentMark(int index, float mark);
};

// --- Derived Classes ---

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