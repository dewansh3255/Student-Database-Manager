#include "basicIO.h"
#include "Student.h"
#include "StudentDatabase.h"
#include "Exceptions.h"
#include "MyString.h"

const char* DATABASE_FILE = "students.csv";

void displayStudentDetails(const Student* s);
void validateRollNumber(const MyString& roll, StudentDatabase& db);
void validateName(const MyString& name);
void printMenu();
void addNewStudent(StudentDatabase& db);
void modifyStudent(StudentDatabase& db);
void sortingMenu(StudentDatabase& db);

int main() {
    StudentDatabase* db = new StudentDatabase(DATABASE_FILE);
    int choice = 0;

    do {
        printMenu();
        choice = io.inputint();

        switch (choice) {
            case 1: 
                addNewStudent(*db); 
                break;
            case 2: 
                modifyStudent(*db); 
                break;
            case 3: 
                io.outputstring("\n--- All Student Records ---\n"); 
                db->displayAll();
                break;
            case 4: 
                sortingMenu(*db); 
                break;
            case 5: 
                db->saveToFile(); 
                break; 
            case 6: 
                io.outputstring("Exiting program.\n"); 
                break;
            default: 
                io.errorstring("Invalid choice. Please try again.\n"); 
                break;
        }

    } while (choice != 6);

    delete db;
    return 0;
}


// --- Helper Function Implementations ---


// --- FIX IS HERE ---
void displayStudentDetails(const Student* s) {
    if (s == 0) {
        io.outputstring("Student pointer is null.\n");
        return;
    }

    io.outputstring("------------------------\n");
    io.outputstring("Name: ");
    io.outputstring(s->getName().c_str());
    io.outputstring("\n");

    io.outputstring("Roll Number: ");
    io.outputstring(s->getRollNumber().c_str());
    io.outputstring("\n");

    io.outputstring("Level: ");
    io.outputstring(s->getLevel().c_str());
    io.outputstring("\n");

    io.outputstring("Branch: ");
    io.outputstring(s->getBranch() == CSE ? "CSE" : "ECE");
    io.outputstring("\n");

    io.outputstring("Marks:\n");
    // Use floatToMyString (from Student.h) to print decimals
    io.outputstring("  Class Assessment: ");
    io.outputstring(floatToMyString(s->getComponentMark(CLASS_ASSESSMENT)).c_str());
    io.outputstring("\n");
    
    io.outputstring("  Quiz: ");
    io.outputstring(floatToMyString(s->getComponentMark(QUIZ)).c_str());
    io.outputstring("\n");

    io.outputstring("  Mid-Sem: "); // UNIFIED
    io.outputstring(floatToMyString(s->getComponentMark(MIDSEM)).c_str());
    io.outputstring("\n");

    io.outputstring("  End-Sem: "); // UNIFIED
    io.outputstring(floatToMyString(s->getComponentMark(ENDSEM)).c_str());
    io.outputstring("\n");

    io.outputstring("Total Marks: ");
    io.outputstring(floatToMyString(s->calculateTotalMarks()).c_str());
    io.outputstring("\n");

    io.outputstring("Grade: ");
    char grade[2];
    grade[0] = s->getGrade();
    grade[1] = '\0';
    io.outputstring(grade);
    io.outputstring("\n");
    
    io.outputstring("------------------------\n");
}
// --- END FIX ---

void validateRollNumber(const MyString& roll, StudentDatabase& db) {
    if (roll.size() == 0) {
        throw InvalidRollNumberException("Roll number cannot be empty.");
    }
    if (roll.size() > 20) {
        throw BufferOverflowException("Roll number is too long (max 20).");
    }

    const char* r = roll.c_str();
    for (int i = 0; i < roll.size(); i++) {
        bool isLetter = (r[i] >= 'A' && r[i] <= 'Z') || (r[i] >= 'a' && r[i] <= 'z');
        bool isNumber = (r[i] >= '0' && r[i] <= '9');
        
        if (!isLetter && !isNumber) {
            throw InvalidRollNumberException("Roll number contains unrecognized characters.");
        }
    }

    if (db(roll) != nullptr) {
        throw ValidationException("Error: A student with this roll number already exists.");
    }
}

void validateName(const MyString& name) {
    if (name.size() == 0) {
        throw InvalidNameException("Name cannot be empty.");
    }
    if (name.size() > 50) {
        throw BufferOverflowException("Name is too long (max 50).");
    }

    int spaceIndex = name.find(" ");
    if (spaceIndex == -1) {
        throw InvalidNameException("Name must include a first and second name.");
    }

    const char* n = name.c_str();
    for (int i = spaceIndex + 1; i < name.size(); i++) {
        bool isLetter = (n[i] >= 'A' && n[i] <= 'Z') || (n[i] >= 'a' && n[i] <= 'z');
        bool isSpace = (n[i] == ' ');
        
        if (!isLetter && !isSpace) {
            throw InvalidNameException("Second name contains numbers or special characters.");
        }
    }
}

void printMenu() {
    io.outputstring("\n--- Student Database Menu ---\n");
    io.outputstring("1. Add a new Student\n");
    io.outputstring("2. Modify a Student's Marks\n");
    io.outputstring("3. Display all Students\n");
    io.outputstring("4. Sort Database\n");
    io.outputstring("5. Save Data to File\n");
    io.outputstring("6. Exit\n");
    io.outputstring("Enter your choice: ");
}

void addNewStudent(StudentDatabase& db) {
    char rollBuffer[21], nameBuffer[51];
    int levelChoice, branchChoice;

    try {
        io.outputstring("Enter Roll Number: ");
        io.inputstring(rollBuffer, 21);
        MyString roll(rollBuffer);
        validateRollNumber(roll, db);

        io.outputstring("Enter Full Name: ");
        io.inputstring(nameBuffer, 51);
        MyString name(nameBuffer);
        validateName(name);

        io.outputstring("Enter Level (1: BTech, 2: MTech, 3: PhD): ");
        levelChoice = io.inputint();
        if (levelChoice < 1 || levelChoice > 3) throw ValidationException("Error: Invalid level choice.");

        io.outputstring("Enter Branch (1: CSE, 2: ECE): ");
        branchChoice = io.inputint();
        if (branchChoice < 1 || branchChoice > 2) throw ValidationException("Error: Invalid branch choice.");
        
        Branch branch = (branchChoice == 1) ? CSE : ECE;
        
        Student* newStudent = nullptr;
        if (levelChoice == 1) {
            newStudent = new BTechStudent(rollBuffer, nameBuffer, branch);
        } else if (levelChoice == 2) {
            newStudent = new MTechStudent(rollBuffer, nameBuffer, branch);
        } else {
            newStudent = new PhDStudent(rollBuffer, nameBuffer, branch);
        }

        char markBuffer[20]; 

        io.outputstring("Enter Class Assessment marks: ");
        io.inputstring(markBuffer, 20);
        newStudent->setComponentMark(CLASS_ASSESSMENT, stringToFloat(MyString(markBuffer)));

        io.outputstring("Enter Quiz marks: ");
        io.inputstring(markBuffer, 20);
        newStudent->setComponentMark(QUIZ, stringToFloat(MyString(markBuffer)));

        io.outputstring("Enter Mid-Sem marks: ");
        io.inputstring(markBuffer, 20);
        newStudent->setComponentMark(MIDSEM, stringToFloat(MyString(markBuffer)));

        io.outputstring("Enter End-Sem marks: ");
        io.inputstring(markBuffer, 20);
        newStudent->setComponentMark(ENDSEM, stringToFloat(MyString(markBuffer)));

        db += newStudent;
        io.outputstring("\nSuccessfully added the new student.\n");

    } catch (const InputException& e) {
        io.errorstring("\nOperation failed. Reason: ");
        io.errorstring(e.getMessage().c_str());
        io.errorstring("\n");
    }
}

void modifyStudent(StudentDatabase& db) {
    char rollBuffer[50];
    io.outputstring("Enter the Roll Number of the student to modify: ");
    io.inputstring(rollBuffer, 50);

    Student* studentToModify = db(rollBuffer);

    if (studentToModify == nullptr) {
        io.errorstring("Error: Student with that Roll Number not found.\n");
        return;
    }

    io.outputstring("\n--- Modifying Student: ");
    io.outputstring(studentToModify->getName().c_str());
    io.outputstring(" ---\n");
    
    io.outputstring("Which mark to edit?\n");
    io.outputstring("1. Class Assessment\n");
    io.outputstring("2. Quiz\n");
    io.outputstring("3. Mid-Sem\n");
    io.outputstring("4. End-Sem\n");
    io.outputstring("Enter your choice: ");
    int choice = io.inputint();

    if (choice < 1 || choice > 4) {
        io.errorstring("Invalid choice.\n");
        return;
    }

    int componentIndex = choice - 1; 

    char markBuffer[20];
    io.outputstring("Enter the new mark: ");
    io.inputstring(markBuffer, 20);
    float newMark = stringToFloat(MyString(markBuffer));
    
    studentToModify->setComponentMark(componentIndex, newMark);
    
    io.outputstring("\nMark updated successfully.\n");
}
void sortingMenu(StudentDatabase& db) {
    io.outputstring("\n--- Sort Options ---\n");
    io.outputstring("1. Sort by Roll Number (Ascending)\n");
    io.outputstring("2. Sort by Name (Alphabetical)\n");
    io.outputstring("3. Sort by Total Marks (Descending)\n");
    io.outputstring("Enter your choice: ");
    int choice = io.inputint();

    switch (choice) {
        case 1: 
            db.sortByRollNumber(); 
            io.outputstring("Database sorted by Roll Number.\n"); 
            break;
        case 2: 
            db.sortByName(); 
            // The function itself now prints the success message
            break;
        case 3: 
            db.sortByTotalMarks(); 
            io.outputstring("Database sorted by Total Marks.\n"); 
            break;
        default: 
            io.errorstring("Invalid sorting choice.\n"); 
            break;
    }
}