#include "MyString.h"
#include "basicIO.h" // only used if you want to debug via print_str (not required)
 
// ------------------ internal helpers ------------------
int MyString::computeLength(const char* s) const {
    if (!s) return 0;
    int n = 0;
    while (s[n] != '\0') n++;
    return n;
}

void MyString::copyChars(char* dest, const char* src) const {
    int i = 0;
    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

int MyString::compareChars(const char* a, const char* b) const {
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] != b[i]) return (a[i] - b[i]);
        i++;
    }
    return (a[i] - b[i]); // handles end-of-string differences
}

bool MyString::isSpaceChar(char c) const {
    return (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v' || c == '\f');
}

char MyString::toLowerChar(char c) const {
    if (c >= 'A' && c <= 'Z') return (char)(c - 'A' + 'a');
    return c;
}

// ------------------ constructors / destructor ------------------
MyString::MyString() {
    data = new char[1];
    data[0] = '\0';
    length = 0;
}

MyString::MyString(const char* cstr) {
    if (!cstr) {
        data = new char[1];
        data[0] = '\0';
        length = 0;
    } else {
        length = computeLength(cstr);
        data = new char[length + 1];
        copyChars(data, cstr);
    }
}

MyString::MyString(const MyString& other) {
    length = other.length;
    data = new char[length + 1];
    copyChars(data, other.data);
}

MyString::~MyString() {
    delete[] data;
}

// ------------------ assignment ------------------
MyString& MyString::operator=(const MyString& other) {
    if (this == &other) return *this;
    delete[] data;
    length = other.length;
    data = new char[length + 1];
    copyChars(data, other.data);
    return *this;
}

// ------------------ accessors ------------------
const char* MyString::c_str() const {
    return data;
}

int MyString::size() const {
    return length;
}

// ------------------ comparisons ------------------
bool MyString::operator==(const MyString& other) const {
    return compareChars(data, other.data) == 0;
}

bool MyString::operator!=(const MyString& other) const {
    return !(*this == other);
}

bool MyString::operator<(const MyString& other) const {
    return compareChars(data, other.data) < 0;
}

// ------------------ modifiers ------------------
void MyString::toLowercase() {
    for (int i = 0; i < length; ++i) data[i] = toLowerChar(data[i]);
}

void MyString::trim() {
    if (length == 0) return;
    int start = 0;
    while (start < length && isSpaceChar(data[start])) start++;
    int end = length - 1;
    while (end >= start && isSpaceChar(data[end])) end--;
    int newLen = (start <= end) ? (end - start + 1) : 0;
    char* newData = new char[newLen + 1];
    for (int i = 0; i < newLen; ++i) newData[i] = data[start + i];
    newData[newLen] = '\0';
    delete[] data;
    data = newData;
    length = newLen;
}

void MyString::concat(const MyString& other) {
    if (other.length == 0) return;
    int newLen = length + other.length;
    char* newData = new char[newLen + 1];
    for (int i = 0; i < length; ++i) newData[i] = data[i];
    for (int j = 0; j < other.length; ++j) newData[length + j] = other.data[j];
    newData[newLen] = '\0';
    delete[] data;
    data = newData;
    length = newLen;
}

// ------------------ utilities ------------------
int MyString::find(const char* substr) const {
    if (!substr || substr[0] == '\0') return -1;
    int subLen = computeLength(substr);
    if (subLen == 0) return -1;
    for (int i = 0; i <= length - subLen; ++i) {
        int j = 0;
        while (j < subLen && data[i + j] == substr[j]) j++;
        if (j == subLen) return i;
    }
    return -1;
}

void MyString::clear() {
    delete[] data;
    data = new char[1];
    data[0] = '\0';
    length = 0;
}

int MyString::split(const char* delimiter, MyString*& parts) const {
    parts = nullptr;
    if (!delimiter || delimiter[0] == '\0') return 0;
    int dlen = computeLength(delimiter);
    if (dlen == 0) return 0;

    // count occurrences
    int count = 1;
    for (int i = 0; i <= length - dlen; ++i) {
        int j = 0;
        while (j < dlen && data[i + j] == delimiter[j]) j++;
        if (j == dlen) {
            count++;
            i += dlen - 1;
        }
    }

    parts = new MyString[count];
    int start = 0;
    int partIndex = 0;
    int i = 0;
    while (i <= length) {
        bool match = false;
        if (i <= length - dlen) {
            int j = 0;
            while (j < dlen && data[i + j] == delimiter[j]) j++;
            if (j == dlen) match = true;
        }
        if (match || i == length) {
            int tokenLen = i - start;
            char* temp = new char[tokenLen + 1];
            for (int k = 0; k < tokenLen; ++k) temp[k] = data[start + k];
            temp[tokenLen] = '\0';
            parts[partIndex] = MyString(temp);
            delete[] temp;
            partIndex++;
            start = i + dlen;
            i = start;
        } else {
            i++;
        }
    }
    return count;
}