#include "Student.h"
#include "basicIO.h" 

// --- NEW HELPER FUNCTION ---
// Converts a string representation of a float into an actual float.
float stringToFloat(const MyString& s) {
    const char* cstr = s.c_str();
    if (!cstr) return 0.0f;

    float result = 0.0f;
    int i = 0;
    bool isNegative = false;

    if (cstr[i] == '-') {
        isNegative = true;
        i++;
    }

    // Parse integer part
    while (cstr[i] >= '0' && cstr[i] <= '9') {
        result = result * 10.0f + (cstr[i] - '0');
        i++;
    }

    // Parse fractional part
    if (cstr[i] == '.') {
        i++;
        float power = 0.1f;
        while (cstr[i] >= '0' && cstr[i] <= '9') {
            result = result + (cstr[i] - '0') * power;
            power *= 0.1f;
            i++;
        }
    }

    return isNegative ? -result : result;
}


// Helper to convert float to MyString (already exists, no changes needed here)
MyString floatToMyString(float f) {
    if (f == 0.0f) return MyString("0.0");
    char buffer[32];
    int pos = 0;
    if (f < 0) {
        buffer[pos++] = '-';
        f = -f;
    }
    int integerPart = (int)f;
    float fractionalPart = f - integerPart;
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
    fractionalPart *= 100;
    int fractionalInt = (int)fractionalPart;
    if (fractionalInt < 10) buffer[pos++] = '0';
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
    for (int j = i - 1; j >= 0; --j) buffer[pos++] = temp[j];
    buffer[pos] = '\0';
    return MyString(buffer);
}

// Helper to get the name of the mark component (unchanged)
const char* getComponentName(int index) {
    switch (index) {
        case CLASS_ASSESSMENT: return "Class Assessment";
        case QUIZ: return "Quiz";
        case MIDSEM: return "Midsem";
        case ENDSEM: return "Endsem";
        default: return "Unknown";
    }
}

// The rest of Student.cpp is unchanged
// --- Base Student Class Implementation ---
Student::Student() : rollNumber(""), name(""), branch(CSE) {
    for (int i = 0; i < NUM_COMPONENTS; ++i) componentMarks[i] = 0.0f;
}
Student::Student(const char* roll, const char* studentName, Branch studentBranch)
    : rollNumber(roll), name(studentName), branch(studentBranch) {
    for (int i = 0; i < NUM_COMPONENTS; ++i) {
        componentMarks[i] = 0.0f; // Initialize marks to 0
    }
}
Student::Student(const Student& other)
    : rollNumber(other.rollNumber), name(other.name), branch(other.branch) {
    for (int i = 0; i < NUM_COMPONENTS; ++i) {
        componentMarks[i] = other.componentMarks[i];
    }
}
Student::~Student() {}
Student& Student::operator=(const Student& other) {
    if (this == &other) return *this;
    rollNumber = other.rollNumber;
    name = other.name;
    branch = other.branch;
    for (int i = 0; i < NUM_COMPONENTS; ++i) {
        componentMarks[i] = other.componentMarks[i];
    }
    return *this;
}
float Student::calculateTotalMarks() const {
    float total = 0.0f;
    for (int i = 0; i < NUM_COMPONENTS; ++i) {
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
    details.concat(this->getLevel());
    details.concat("\nBranch: ");
    details.concat(branch == CSE ? "CSE" : "ECE");
    details.concat("\nMarks:\n");
    for (int i = 0; i < NUM_COMPONENTS; ++i) {
        details.concat("  - ");
        details.concat(getComponentName(i));
        details.concat(": ");
        details.concat(floatToMyString(componentMarks[i]));
        details.concat("\n");
    }
    details.concat("Total: ");
    details.concat(floatToMyString(calculateTotalMarks()));
    details.concat("\n-----------------\n");
    return details;
}
const MyString& Student::getRollNumber() const { return rollNumber; }
const MyString& Student::getName() const { return name; }
Branch Student::getBranch() const { return branch; }
float Student::getComponentMark(int index) const {
    if (index >= 0 && index < NUM_COMPONENTS) {
        return componentMarks[index];
    }
    return 0.0f;
}
void Student::setComponentMark(int index, float mark) {
    if (index >= 0 && index < NUM_COMPONENTS) {
        componentMarks[index] = mark;
    }
}
BTechStudent::BTechStudent(const char* roll, const char* name, Branch branch)
    : Student(roll, name, branch) {}
MyString BTechStudent::getLevel() const { return MyString("BTech"); }
MTechStudent::MTechStudent(const char* roll, const char* name, Branch branch)
    : Student(roll, name, branch) {}
MyString MTechStudent::getLevel() const { return MyString("MTech"); }
PhDStudent::PhDStudent(const char* roll, const char* name, Branch branch)
    : Student(roll, name, branch) {}
MyString PhDStudent::getLevel() const { return MyString("PhD"); }