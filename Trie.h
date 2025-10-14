#ifndef TRIE_H
#define TRIE_H

#include "MyString.h"

// The number of possible characters (a-z, case-insensitive)
const int ALPHABET_SIZE = 26;

// Represents a single node in the Trie
struct TrieNode {
    TrieNode* children[ALPHABET_SIZE];
    bool isEndOfWord;

    TrieNode(); // Constructor to initialize children to nullptr
};


class Trie {
private:
    TrieNode* root;

    // A recursive helper function to perform the depth-first traversal
    // and collect the names in alphabetical order.
    void collectAllNames(TrieNode* node, char* prefix, int length, MyString*& namesList, int& count, int& capacity) const;

    // A helper to resize the dynamic array of names
    void resizeNamesList(MyString*& list, int& capacity) const;
    
    // A helper to recursively delete all nodes
    void destroy(TrieNode* node);


public:
    Trie();
    ~Trie();

    // Inserts a name into the Trie.
    void insert(const MyString& name);

    // Returns a dynamic array of all names in the Trie, sorted alphabetically.
    // The caller is responsible for deleting the returned array.
    MyString* getAllNamesSorted(int& out_count) const;
};

#endif // TRIE_H