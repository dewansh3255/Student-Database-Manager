#ifndef MYSTRING_H
#define MYSTRING_H

class MyString {
private:
    char* data;
    int length;

    // internal helpers (no std functions)
    int computeLength(const char* s) const;
    void copyChars(char* dest, const char* src) const;
    int compareChars(const char* a, const char* b) const;
    bool isSpaceChar(char c) const;
    char toLowerChar(char c) const;

public:
    // Rule of Three
    MyString();
    MyString(const char* cstr);
    MyString(const MyString& other);
    ~MyString();

    MyString& operator=(const MyString& other);

    // access
    const char* c_str() const;
    int size() const;

    // comparisons
    bool operator==(const MyString& other) const;
    bool operator!=(const MyString& other) const;
    bool operator<(const MyString& other) const;

    // modifiers
    void toLowercase();     
    void trim();            
    void concat(const MyString& other); // append other to this

    // utilities
    int find(const char* substr) const; 
    void clear(); 
    int split(const char* delimiter, MyString*& parts) const;
};
#endif 