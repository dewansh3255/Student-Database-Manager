#include "Trie.h"

// --- TrieNode Implementation ---

// Constructor for a TrieNode. It initializes all its children pointers to nullptr.
TrieNode::TrieNode() : isEndOfWord(false) {
    for (int i = 0; i < ALPHABET_SIZE; ++i) {
        children[i] = nullptr;
    }
}

// --- Trie Implementation ---

Trie::Trie() {
    root = new TrieNode();
}

// Destructor that kicks off the recursive deletion of nodes.
Trie::~Trie() {
    destroy(root);
}

// Private recursive helper to delete all nodes in the Trie.
void Trie::destroy(TrieNode* node) {
    if (!node) return;
    for (int i = 0; i < ALPHABET_SIZE; ++i) {
        if (node->children[i]) {
            destroy(node->children[i]);
        }
    }
    delete node;
}


void Trie::insert(const MyString& name) {
    TrieNode* currentNode = root;
    MyString lowerName = name;
    lowerName.toLowercase(); // Work with lowercase to be case-insensitive

    for (int i = 0; i < lowerName.size(); ++i) {
        char lowerChar = lowerName.c_str()[i];
        
        // We only consider alphabetic characters for the Trie
        if (lowerChar >= 'a' && lowerChar <= 'z') {
            int index = lowerChar - 'a';
            if (!currentNode->children[index]) {
                currentNode->children[index] = new TrieNode();
            }
            currentNode = currentNode->children[index];
        }
    }
    currentNode->isEndOfWord = true; // Mark the end of the name
}

// Helper to resize the dynamic array used for collecting names.
void Trie::resizeNamesList(MyString*& list, int& capacity) const {
    capacity *= 2;
    MyString* newList = new MyString[capacity];
    for (int i = 0; i < capacity / 2; ++i) {
        newList[i] = list[i];
    }
    delete[] list;
    list = newList;
}

// The recursive function that traverses the Trie and collects all names.
void Trie::collectAllNames(TrieNode* node, char* prefix, int length, MyString*& namesList, int& count, int& capacity) const {
    if (!node) return;

    if (node->isEndOfWord) {
        if (count == capacity) {
            resizeNamesList(namesList, capacity);
        }
        prefix[length] = '\0';
        namesList[count++] = MyString(prefix);
    }

    for (int i = 0; i < ALPHABET_SIZE; ++i) {
        if (node->children[i]) {
            prefix[length] = 'a' + i; // Add the next character
            collectAllNames(node->children[i], prefix, length + 1, namesList, count, capacity);
        }
    }
}

// The public method to get a sorted list of all names.
MyString* Trie::getAllNamesSorted(int& out_count) const {
    int capacity = 10;
    MyString* namesList = new MyString[capacity];
    out_count = 0;
    
    char prefix[100]; // Assuming names won't exceed 99 characters

    collectAllNames(root, prefix, 0, namesList, out_count, capacity);

    return namesList;
}