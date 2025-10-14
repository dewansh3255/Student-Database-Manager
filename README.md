# 🎓 Student Database Management System

A comprehensive C++ application for managing, storing, and sorting student records with a unique architectural constraint: **zero dependency on standard C++ and C libraries**. All core functionalities are implemented from scratch using direct Linux system calls.

## ✨ Features

### Core Capabilities
- **📚 Interactive Management**: Command-line interface for adding, modifying, and displaying student records
- **💾 Data Persistence**: Automatic loading and manual saving to CSV format (`students.csv`)
- **🏗️ Custom Data Structures**: Built entirely on custom classes without standard library dependencies
- **🔄 Polymorphic Design**: Base `Student` class with derived classes for different student types
- **🔢 Advanced Sorting**: Multiple sorting algorithms for Roll Number, Name, and Total Marks
- **❗ Exception Handling**: Robust custom exception hierarchy for input validation
- **🚫 Standard Library Free**: Pure C++ implementation using Linux syscalls for all I/O operations

### Student Information Management
The system tracks the following data for each student:

| Field | Description |
|-------|-------------|
| **Roll Number** | Unique student identifier |
| **Full Name** | Student's complete name |
| **Level** | BTech, MTech, or PhD |
| **Branch** | CSE or ECE |
| **Marks** | Class Assessment, Quiz, Midsem, Endsem |

### Available Operations
1. **Add Student**: Create new student records with validation
2. **Modify Marks**: Update specific mark categories
3. **Display Records**: View all student information
4. **Sort Database**: 
   - By Roll Number (ascending)
   - By Name (alphabetical)
   - By Total Marks (descending)
5. **Save Data**: Persist changes to CSV file

## 📂 Project Structure

```
.
├── Makefile                # Build configuration
├── MyString.h / .cpp       # Custom string implementation
├── basicIO.h / .cpp        # Low-level file I/O operations
├── Trie.h / .cpp          # Trie data structure for sorting
├── Exceptions.h           # Custom exception hierarchy
├── Student.h / .cpp       # Student class hierarchy
├── StudentDatabase.h / .cpp # Database management
├── main.cpp               # Application entry point
└── students.csv           # Data storage file
```

## 🚀 Getting Started

### Prerequisites
- **C++ Compiler**: g++ (recommended)
- **Build Tool**: GNU Make
- **Operating System**: Linux (required for syscall compatibility)

### Compilation

Build the project using the provided Makefile:

```bash
make
```

This generates an executable named `student_manager`.

### Execution

Run the application:

```bash
./student_manager
```

The system automatically loads existing data from `students.csv` if available.

### Cleaning Build Files

Remove compiled objects and executables:

```bash
make clean
```

## 🛠️ Technical Implementation

### Core Concepts Demonstrated

#### 1. **Object-Oriented Design**
- Logical class decomposition (`Student`, `StudentDatabase`, `Trie`, `MyString`)
- Encapsulation of data and behavior
- High code reusability and maintainability

#### 2. **Manual Memory Management**
- Complete implementation of the **Rule of Three**:
  - Custom destructor
  - Copy constructor
  - Copy assignment operator
- Prevention of memory leaks through careful resource handling

#### 3. **Operator Overloading**
- `+=` operator for adding students to database
- `()` operator for searching students by roll number

#### 4. **Polymorphism**
- Abstract `Student` base class with virtual functions
- Derived classes: `BTechStudent`, `MTechStudent`, `PhDStudent`
- Unified interface for heterogeneous student types

#### 5. **Advanced Data Structures**
- **Custom Dynamic String**: `MyString` class with full manipulation capabilities
- **Trie Structure**: Efficient case-insensitive alphabetical sorting
- **Dynamic Arrays**: Manual memory management for variable-sized collections

#### 6. **Custom Exception Handling**
Complete exception hierarchy inheriting from `InputException`:
- Roll number validation errors
- Duplicate entry detection
- Name format validation
- Invalid menu choice handling

#### 7. **Low-Level System Programming**
- Direct Linux syscall usage for all I/O operations
- Implementation of file reading/writing without standard libraries
- Console interaction through system-level calls

### String Management
The `MyString` class provides:
- Dynamic memory allocation
- String comparison and concatenation
- Trimming and splitting operations
- Substring searching
- Case-insensitive operations

### File Processing
The `basicIO` class handles:
- Low-level file reading using syscalls
- CSV parsing and generation
- Efficient data persistence

### Sorting Algorithms

| Method | Algorithm | Use Case |
|--------|-----------|----------|
| **Roll Number Sort** | Bubble Sort | Ascending numerical order |
| **Name Sort** | Trie-based | Alphabetical, case-insensitive |
| **Marks Sort** | Bubble Sort | Descending by total marks |

## 📊 Data Format

Student records are stored in CSV format with the following structure:

```csv
RollNumber,FullName,Level,Branch,ClassAssessment,Quiz,Midsem,Endsem
B21CS001,John Doe,BTech,CSE,85,90,88,92
M22EC002,Jane Smith,MTech,ECE,78,82,80,85
```

## ⚠️ Input Validation

The system enforces strict validation rules:

- **Roll Number**: Must meet minimum length requirements
- **Full Name**: Must contain at least first and last name (space required)
- **Duplicate Detection**: Prevents duplicate roll number entries
- **Mark Ranges**: Validates numerical input for all mark categories
- **Menu Choices**: Validates user selections

## 🎯 Design Philosophy

This project emphasizes:
- **Learning by Building**: Understanding C++ fundamentals by implementing standard functionality
- **System-Level Understanding**: Direct interaction with OS through syscalls
- **Robust Architecture**: Clean separation of concerns across modules
- **Memory Safety**: Careful resource management without garbage collection
- **Educational Value**: Demonstrating core CS concepts in practical implementation

## 🤝 Contributing

This is an educational project demonstrating fundamental C++ concepts. Contributions that enhance the learning experience while maintaining the no-standard-library constraint are welcome.

## 📄 License

This project is created for educational purposes.

## 🙏 Acknowledgments

Built as a demonstration of:
- Low-level C++ programming techniques
- Object-oriented design principles
- Data structure implementation
- System programming concepts

---

**Note**: This application requires a Linux environment due to its reliance on POSIX syscalls. It will not compile or run on Windows or macOS without significant modifications.