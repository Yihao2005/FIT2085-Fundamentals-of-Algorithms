#pragma once

#include <iostream>
#include <string>

using namespace std;

struct SPNode {
    SPNode* next = nullptr;

    string key;
    string value;
};

struct HashTable {
    const int table_size = 10000019;
    SPNode** table = new SPNode*[table_size];

    HashTable() {
        for (int i = 0; i < table_size; i++) {
            table[i] = nullptr;
        }
    }

    int hash(string key) {
        const int base = 271;
        long long hash_value = 0;

        for (int i = 0; i < key.length(); i++) {
            int ascii_code = key[i];
            hash_value = (hash_value * base) + ascii_code;
            hash_value %= table_size; // Using table_size as our mod
        }
        return hash_value;
    }

    void insert(string key, string value) {
        // Find its position
        int table_position = hash(key);

        SPNode* current_node = table[table_position];
        while (current_node != nullptr) {
            if (current_node->key == key) {
                // Found the key! It was already in the chain.
                current_node->value = value;
                // Our job is finished here, exit the function.
                return;
            }
            current_node = current_node->next;
        }

        // Create a new node for this item:
        SPNode* node = new SPNode;
        node->key = key;
        node->value = value;

        // Insert it at the beginning of the chain.
        node->next = table[table_position];
        table[table_position] = node;
    }

    string search(string key) {
        // Get the expected position
        int position = hash(key);

        SPNode* current_node = table[position];
        while (current_node != nullptr) {
            if (current_node->key == key) {
                // Found the key!
                // Return its value.
                return current_node->value;
            }
            current_node = current_node->next;
        }

        throw string("Key not found!");
    }

    void del(string key) {
        // Get the table position
        int position = hash(key);

        SPNode* current_node = table[position];
        SPNode* prev_node = nullptr;
        while (current_node != nullptr) {
            if (current_node->key == key) {
                if (prev_node == nullptr) {
                    // It's the first node of the chain
                    table[position] = current_node->next;
                }
                else {
                    // It's not the first node and a prev node exists
                    prev_node->next = current_node->next;
                }
                // Either way, don't forget to delete the node from the memory
                delete current_node;
                return;
            }
            prev_node = current_node;
            current_node = current_node->next;
        }

        throw string("Key not found!");
    }
};