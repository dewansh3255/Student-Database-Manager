#include "basicIO.h"
#include "StudentDatabase.h"
#include "Exceptions.h"

const char* DATABASE_FILE = "students.csv";

// --- Helper Functions for User Interaction ---

void printMenu() {
    io.outputstring("\n--- Student Database Menu ---\n");
    io.outputstring("1. Add a new Student\n");
    io.outputstring("2. Modify a Student's Marks\n");
    io.outputstring("3. Display all Students\n");
    io.outputstring("4. Sort Database\n");
    io.outputstring("5. Save Data to File\n"); // New option
    io.outputstring("6. Exit\n");              // Exit is now 6
    io.outputstring("Enter your choice: ");
}

void addNewStudent(StudentDatabase& db) {
    char rollBuffer[50], nameBuffer[100];
    int levelChoice, branchChoice;

    try {
        io.outputstring("Enter Roll Number: ");
        io.inputstring(rollBuffer, 50);
        if (MyString(rollBuffer).size() < 5) {
            throw InvalidRollNumberException("Error: Roll number is too short.");
        }
        if (db(rollBuffer) != nullptr) {
            throw ValidationException("Error: A student with this roll number already exists.");
        }
        io.outputstring("Enter Full Name: ");
        io.inputstring(nameBuffer, 100);
        if (MyString(nameBuffer).find(" ") == -1) {
            throw InvalidNameException("Error: Please provide at least a first and last name.");
        }
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

        db += newStudent;
        io.outputstring("\nSuccessfully added the new student.\n");

    } catch (const InputException& e) {
        io.errorstring("\nOperation failed. Reason: ");
        io.errorstring(e.getMessage().c_str());
        io.errorstring("\n");
    }
}

// COMPLETELY REVISED FUNCTION
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
    
    // Show the marks editing menu
    io.outputstring("Which mark to edit?\n");
    io.outputstring("1. Class Assessment\n");
    io.outputstring("2. Quiz\n");
    io.outputstring("3. Midsem\n");
    io.outputstring("4. Endsem\n");
    io.outputstring("Enter your choice: ");
    int choice = io.inputint();

    if (choice < 1 || choice > 4) {
        io.errorstring("Invalid choice.\n");
        return;
    }

    // The enum values start from 0, so we subtract 1 from the user's choice
    int componentIndex = choice - 1; 

    io.outputstring("Enter the new mark: ");
    float mark = (float)io.inputint(); 
    studentToModify->setComponentMark(componentIndex, mark);
    
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
        case 1: db.sortByRollNumber(); io.outputstring("Database sorted by Roll Number.\n"); break;
        case 2: db.sortByName(); io.outputstring("Database sorted by Name.\n"); break;
        case 3: db.sortByTotalMarks(); io.outputstring("Database sorted by Total Marks.\n"); break;
        default: io.errorstring("Invalid sorting choice.\n"); break;
    }
}

// --- Main Application Loop ---

int main() {
    StudentDatabase* db = new StudentDatabase(DATABASE_FILE);
    int choice = 0;

    do {
        printMenu();
        choice = io.inputint();

        switch (choice) {
            case 1: addNewStudent(*db); break;
            case 2: modifyStudent(*db); break;
            case 3: io.outputstring("\n--- All Student Records ---\n"); db->displayAll(); break;
            case 4: sortingMenu(*db); break;
            case 5: db->saveToFile(); break; // New case for saving
            case 6: io.outputstring("Exiting program.\n"); break; // New case for exiting
            default: io.errorstring("Invalid choice. Please try again.\n"); break;
        }

    } while (choice != 6); // Loop until user chooses 6

    delete db;
    return 0;
}