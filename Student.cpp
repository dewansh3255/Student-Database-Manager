#include "Student.h"
#include "basicIO.h" // For converting numbers to strings in toString

// A helper function to convert a float to MyString (since no std libraries are allowed)
MyString floatToMyString(float f) {
    // This is a simplified implementation. A full implementation is more complex.
    // It handles the integer part and a few decimal places.
    if (f == 0.0f) return MyString("0.0");

    char buffer[32];
    int pos = 0;

    if (f < 0) {
        buffer[pos++] = '-';
        f = -f;
    }

    int integerPart = (int)f;
    float fractionalPart = f - integerPart;

    // Convert integer part to string
    if (integerPart == 0) {
        buffer[pos++] = '0';
    } else {
        char temp[16];
        int i = 0;
        while (integerPart > 0) {
            temp[i++] = (integerPart % 10) + '0';
            integerPart /= 10;
        }
        for (int j = i - 1; j >= 0; --j) {
            buffer[pos++] = temp[j];
        }
    }

    buffer[pos++] = '.';

    // Convert fractional part (2 decimal places)
    fractionalPart *= 100;
    int fractionalInt = (int)fractionalPart;
    if (fractionalInt < 10) {
        buffer[pos++] = '0';
    }
    
    char temp[3];
    int i = 0;
    if (fractionalInt == 0) {
        temp[i++] = '0';
    } else {
         while (fractionalInt > 0) {
            temp[i++] = (fractionalInt % 10) + '0';
            fractionalInt /= 10;
        }
    }
    for (int j = i - 1; j >= 0; --j) {
        buffer[pos++] = temp[j];
    }


    buffer[pos] = '\0';
    return MyString(buffer);
}


// --- Base Student Class Implementation ---

Student::Student()
    : rollNumber(""), name(""), branch(CSE), componentMarks(nullptr), numComponents(0) {}

Student::Student(const char* roll, const char* studentName, Branch studentBranch, int totalComponents)
    : rollNumber(roll), name(studentName), branch(studentBranch), numComponents(totalComponents) {
    if (numComponents > 0) {
        componentMarks = new float[numComponents];
        for (int i = 0; i < numComponents; ++i) {
            componentMarks[i] = 0.0f; // Initialize marks to 0
        }
    } else {
        componentMarks = nullptr;
    }
}

// Copy constructor: deep copies the dynamic array
Student::Student(const Student& other)
    : rollNumber(other.rollNumber), name(other.name), branch(other.branch), numComponents(other.numComponents) {
    if (numComponents > 0) {
        componentMarks = new float[numComponents];
        for (int i = 0; i < numComponents; ++i) {
            componentMarks[i] = other.componentMarks[i];
        }
    } else {
        componentMarks = nullptr;
    }
}

// Virtual destructor: ensures derived class destructors are called
Student::~Student() {
    delete[] componentMarks;
}

// Assignment operator: handles self-assignment and deep copies the array
Student& Student::operator=(const Student& other) {
    if (this == &other) {
        return *this; // Handle self-assignment
    }

    // First, release existing resources
    delete[] componentMarks;

    // Now, copy data from 'other'
    rollNumber = other.rollNumber;
    name = other.name;
    branch = other.branch;
    numComponents = other.numComponents;

    if (numComponents > 0) {
        componentMarks = new float[numComponents];
        for (int i = 0; i < numComponents; ++i) {
            componentMarks[i] = other.componentMarks[i];
        }
    } else {
        componentMarks = nullptr;
    }

    return *this;
}

float Student::calculateTotalMarks() const {
    float total = 0.0f;
    for (int i = 0; i < numComponents; ++i) {
        total += componentMarks[i];
    }
    return total;
}

MyString Student::toString() const {
    MyString details("Roll Number: ");
    details.concat(rollNumber);

    details.concat("\nName: ");
    details.concat(name);
    
    details.concat("\nLevel: ");
    details.concat(this->getLevel()); // Polymorphically calls the correct getLevel()

    details.concat("\nBranch: ");
    details.concat(branch == CSE ? "CSE" : "ECE");

    details.concat("\nMarks: [");
    for (int i = 0; i < numComponents; ++i) {
        details.concat(floatToMyString(componentMarks[i]));
        if (i < numComponents - 1) {
            details.concat(", ");
        }
    }
    details.concat("]");
    
    details.concat("\nTotal: ");
    details.concat(floatToMyString(calculateTotalMarks()));
    
    details.concat("\n-----------------\n");

    return details;
}

// --- Getters and Setters ---
const MyString& Student::getRollNumber() const { return rollNumber; }
const MyString& Student::getName() const { return name; }
int Student::getNumComponents() const { return numComponents; }

float Student::getComponentMark(int index) const {
    if (index >= 0 && index < numComponents) {
        return componentMarks[index];
    }
    return 0.0f; // Return a default value for invalid index
}

void Student::setComponentMark(int index, float mark) {
    if (index >= 0 && index < numComponents) {
        componentMarks[index] = mark;
    }
}


// --- BTechStudent Implementation ---
BTechStudent::BTechStudent(const char* roll, const char* name, Branch branch, int numComponents)
    : Student(roll, name, branch, numComponents) {}

MyString BTechStudent::getLevel() const {
    return MyString("BTech");
}


// --- MTechStudent Implementation ---
MTechStudent::MTechStudent(const char* roll, const char* name, Branch branch, int numComponents)
    : Student(roll, name, branch, numComponents) {}

MyString MTechStudent::getLevel() const {
    return MyString("MTech");
}


// --- PhDStudent Implementation ---
PhDStudent::PhDStudent(const char* roll, const char* name, Branch branch, int numComponents)
    : Student(roll, name, branch, numComponents) {}

MyString PhDStudent::getLevel() const {
    return MyString("PhD");
}