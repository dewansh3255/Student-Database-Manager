#include "basicIO.h"
#include "StudentDatabase.h"
#include "Exceptions.h"

// Using a constant for the number of assessment components for easy modification.
const int NUM_ASSESSMENTS = 5;

// --- Helper Functions for User Interaction ---

// Displays the main menu to the user.
void printMenu() {
    io.outputstring("\n--- Student Database Menu ---\n");
    io.outputstring("1. Add a new Student\n");
    io.outputstring("2. Modify a Student's Marks\n");
    io.outputstring("3. Display all Students\n");
    io.outputstring("4. Sort Database\n"); // New option
    io.outputstring("5. Exit\n"); // Exit is now 5
    io.outputstring("Enter your choice: ");
}

// Function to handle the process of adding a new student.
// This is where exception handling is critical.
void addNewStudent(StudentDatabase& db) {
    char rollBuffer[50], nameBuffer[100];
    int levelChoice, branchChoice;

    try {
        io.outputstring("Enter Roll Number: ");
        io.inputstring(rollBuffer, 50);
        // Validation example: A simple roll number validation
        if (MyString(rollBuffer).size() < 5) {
            throw InvalidRollNumberException("Error: Roll number is too short.");
        }
        if (db(rollBuffer) != nullptr) {
            throw ValidationException("Error: A student with this roll number already exists.");
        }


        io.outputstring("Enter Full Name: ");
        io.inputstring(nameBuffer, 100);
        // Validation example: Check if the name contains at least one space.
        if (MyString(nameBuffer).find(" ") == -1) {
            throw InvalidNameException("Error: Please provide at least a first and last name.");
        }


        io.outputstring("Enter Level (1: BTech, 2: MTech, 3: PhD): ");
        levelChoice = io.inputint();
        if (levelChoice < 1 || levelChoice > 3) {
            throw ValidationException("Error: Invalid level choice.");
        }

        io.outputstring("Enter Branch (1: CSE, 2: ECE): ");
        branchChoice = io.inputint();
        if (branchChoice < 1 || branchChoice > 2) {
            throw ValidationException("Error: Invalid branch choice.");
        }
        Branch branch = (branchChoice == 1) ? CSE : ECE;
        
        // Polymorphism in action: Create the specific student type based on input,
        // but store it in a base class pointer.
        Student* newStudent = nullptr;
        if (levelChoice == 1) {
            newStudent = new BTechStudent(rollBuffer, nameBuffer, branch, NUM_ASSESSMENTS);
        } else if (levelChoice == 2) {
            newStudent = new MTechStudent(rollBuffer, nameBuffer, branch, NUM_ASSESSMENTS);
        } else {
            newStudent = new PhDStudent(rollBuffer, nameBuffer, branch, NUM_ASSESSMENTS);
        }

        // Use the overloaded += operator to add the student to the database.
        db += newStudent;

        io.outputstring("\nSuccessfully added the new student.\n");

    } catch (const InputException& e) {
        // Catch any of our custom exceptions and display the specific error message.
        io.errorstring("\nOperation failed. Reason: ");
        io.errorstring(e.getMessage().c_str());
        io.errorstring("\n");
    }
}


// Function to modify an existing student's marks.
void modifyStudent(StudentDatabase& db) {
    char rollBuffer[50];
    io.outputstring("Enter the Roll Number of the student to modify: ");
    io.inputstring(rollBuffer, 50);

    // Use the overloaded () operator to find the student.
    Student* studentToModify = db(rollBuffer);

    if (studentToModify == nullptr) {
        io.errorstring("Error: Student with that Roll Number not found.\n");
        return;
    }

    io.outputstring("\n--- Modifying Student: ");
    io.outputstring(studentToModify->getName().c_str());
    io.outputstring(" ---\n");
    
    for (int i = 0; i < studentToModify->getNumComponents(); ++i) {
        io.outputstring("Enter mark for component ");
        io.outputint(i + 1);
        io.outputstring(": ");
        // Here you would use a custom function to read a float if needed.
        // For simplicity, we'll use inputint and cast it.
        float mark = (float)io.inputint(); 
        studentToModify->setComponentMark(i, mark);
    }
    
    io.outputstring("\nMarks updated successfully.\n");
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
            io.outputstring("Database sorted by Name.\n");
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

// --- Main Application Loop ---

int main() {
    StudentDatabase* db = new StudentDatabase();
    int choice = 0;

    // In the 'main' function:
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
            case 4: // The new case for sorting
                sortingMenu(*db);
                break;
            case 5: // The new case for exiting
                io.outputstring("Exiting program.\n");
                break;
            default:
                io.errorstring("Invalid choice. Please try again.\n");
                break;
        }

    } while (choice != 5); // The loop condition is now 5

    // Clean up the dynamically allocated database before exiting.
    delete db;

    return 0;
}