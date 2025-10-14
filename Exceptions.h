#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include "MyString.h"

// The base class for all input-related exceptions in this project.
class InputException {
protected:
    MyString message;

public:
    // Constructor: Initializes the exception with a specific error message.
    InputException(const char* msg) : message(msg) {}

    // Virtual destructor (good practice for base classes).
    virtual ~InputException() {}

    // A getter to retrieve the error message.
    const MyString& getMessage() const {
        return message;
    }
};

// --- Derived Exception Classes for Specific Errors ---

// Thrown when a roll number has an invalid format.
class InvalidRollNumberException : public InputException {
public:
    InvalidRollNumberException(const char* msg) : InputException(msg) {}
};

// Thrown when a name has an invalid format.
class InvalidNameException : public InputException {
public:
    InvalidNameException(const char* msg) : InputException(msg) {}
};

// Thrown when an input exceeds its designated buffer size.
class BufferOverflowException : public InputException {
public:
    BufferOverflowException(const char* msg) : InputException(msg) {}
};

// A more general exception for other validation failures.
class ValidationException : public InputException {
public:
    ValidationException(const char* msg) : InputException(msg) {}
};


#endif // EXCEPTIONS_H