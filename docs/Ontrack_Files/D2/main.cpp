#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct ContactBook;
struct ContactBookNode;

// You may assume that the User->nickname always matches the nickname it is inserted under.
struct User {
    string nickname;
    string display_name;
    int user_score;
};

struct ContactBookNode {
    string nickname;
    User* associated_user;
    // This will remain a nullptr if we don't see a collision.
    ContactBook* recursed_book = nullptr;

    // Define this after the ContactBook struct is defined.
    ~ContactBookNode();
};

struct ContactBook {
    const int table_size = 27;
    // Depth determines how the hash table functions
    int depth = 0;
    int size = 0;
    ContactBookNode** table = new ContactBookNode*[table_size];

    ContactBook() : ContactBook(0) {}
    ContactBook(int start_depth) : depth(start_depth)
    {
        for (int i = 0; i < table_size; i++)
        {
            table[i] = nullptr;
        }
    }
    ~ContactBook() {
        for (int i=0; i<table_size; i++) {
            delete table[i];
        }
        delete[] table;
    }

    int hash(const string& nickname) {
        // Use the depth parameter!
        if (depth >= nickname.length())
        {
            return 26;
        }

        return nickname[depth] - 'a';
    }

    void insert_user(const string& nickname, User* user_data) {
        // Task 1
        int index = hash(nickname);

        // if table[index] is nullptr, we create a new Contact BookNode
        if (table[index] == nullptr)
        {
            table[index] = new ContactBookNode;
            table[index]->nickname = nickname;
            table[index]->associated_user = user_data;

            size++;
            return;
        }

        // if table[index] is not nullptr and the recursed_book is not nullptr, we insert_user into the recursed_book
        if (table[index]->recursed_book != nullptr)
        {
            table[index]->recursed_book->insert_user(nickname, user_data);

            size++;
            return;
        }

        // if table[index] is not nullptr and the recursed_book is nullptr, we firstly record the old_nickname and the old_user
        string old_nickname = table[index]->nickname;
        User* old_user = table[index]->associated_user;

        // Subsequently, we create a recursed_book inside current contact book
        table[index]->recursed_book = new ContactBook(depth + 1);

        // we change the current contact book's name into "" and associated_user into nullptr
        this->table[index]->nickname = "";
        this->table[index]->associated_user = nullptr;

        // At the last, we insert the old_nickname and old_user into corresponding recursed book
        this->table[index]->recursed_book->insert_user(old_nickname, old_user);
        this->table[index]->recursed_book->insert_user(nickname, user_data);

        size++;

    }

    User* retrieve_user(const string& nickname) {
        // Task 1
        int index = hash(nickname);

        // the table[index] is nullptr, we return nullptr
        if (table[index] == nullptr)
        {
            return nullptr;
        }

        // if the table[index] has recursed_book, we retrieve user from its recursed_book
        if (table[index]->recursed_book != nullptr)
        {
            return table[index]->recursed_book->retrieve_user(nickname);
        }

        // if the table[index] doesn't have recursed_book, we double check if the nickname of that is the same as nickname we want.
        // If it is, we return the corresponding user
        if (table[index]->nickname == nickname)
        {
            return table[index]->associated_user;
        }

        return nullptr;
    }

    // Used to get the only user from the ContactBook whose size is 1
    User* get_only_user(string& nickname) {
        for (int i = 0; i < table_size; i++)
        {
            if (table[i] == nullptr)
            {
                continue;
            }

            if (table[i]->recursed_book != nullptr)
            {
                return table[i]->recursed_book->get_only_user(nickname);
            }

            nickname = table[i]->nickname;
            return table[i]->associated_user;
        }

        return nullptr;
    }

    void delete_user(const string& nickname) {
        // Task 3
        int index = hash(nickname);

        // if table[index] is nullptr, just return
        if (table[index] == nullptr)
        {
            return;
        }

        // if table[index] has recursed book, we execute delete_user inside the corresponding recursed book
        if (table[index]->recursed_book != nullptr)
        {
            table[index]->recursed_book->delete_user(nickname);
            size--;

            // After the element is deleted, we check if the size of recursed_book inside is 1 or not.
            // If the size is 1, we collapse the recursed book and change the the current table[index]'s nick name and
            // associated user into remaining nickname and remaining user.
            if (table[index]->recursed_book->size == 1)
            {
                string remaining_nickname;
                User* remaining_user = table[index]->recursed_book->get_only_user(remaining_nickname);

                delete table[index]->recursed_book;

                table[index]->recursed_book = nullptr;
                table[index]->nickname = remaining_nickname;
                table[index]->associated_user = remaining_user;
            }

            return;
        }

        if (table[index]->nickname == nickname)
        {
            delete table[index];
            table[index] = nullptr;
            size--;
        }
    }

    bool search(const string& nickname) {
        // Task 4
        int index = hash(nickname);

        if (table[index] == nullptr)
        {
            return false;
        }

        if (depth == nickname.length() - 1)
        {
            return true;
        }

        if (table[index]->recursed_book != nullptr)
        {
            return table[index]->recursed_book->search(nickname);
        }

        string stored_nickname = table[index]->nickname;

        if (nickname.length() > stored_nickname.length())
        {
            return false;
        }

        for (int i = 0; i < nickname.length(); i++)
        {
            if (nickname[i] != stored_nickname[i])
            {
                return false;
            }
        }

        return true;
    }
};

ContactBookNode::~ContactBookNode() {
    // This needs to be defined after the ContactBook definition.
    delete recursed_book;
}

// Nicknames are only allowed to contain the lowercase characters a-z, so anything
// else would send hash() outside the bounds of the table. main() checks this before
// it hands anything to the ContactBook.
bool valid_nickname(const string& nickname) {
    if (nickname.empty()) {
        return false;
    }
    for (int i = 0; i < (int)nickname.length(); i++) {
        if (nickname[i] < 'a' || nickname[i] > 'z') {
            return false;
        }
    }
    return true;
}

int main() {
    ContactBook book;

    // The ContactBook only ever borrows a User*: insert_user stores the pointer, and
    // delete_user frees its own node but never the User behind it. So main() owns every
    // User it allocates, and is responsible for deleting them again.
    vector<User*> users;

    string menu = "1. Add a contact\n2. Retrieve a contact\n3. Remove a contact\n4. Search by prefix\n5. Show the number of contacts\n6. See the menu again\n7. Quit";
    int command = 6;
    while (command != 7) {
        if (command == 1) {
            cout << "Enter the nickname:" << endl;
            string nickname;
            cin >> nickname;

            if (!valid_nickname(nickname)) {
                cout << "Nicknames must use the lowercase letters a-z only" << endl;
            } else if (book.retrieve_user(nickname) != nullptr) {
                // Checked here so that insert_user is only ever called on a nickname
                // we know is free, and never has to throw.
                cout << "The nickname '" << nickname << "' is already taken" << endl;
            } else {
                cout << "Enter the display name:" << endl;
                string display_name;
                cin >> display_name;

                cout << "Enter the user score:" << endl;
                int user_score;
                cin >> user_score;

                User* user = new User;
                user->nickname = nickname;
                user->display_name = display_name;
                user->user_score = user_score;

                book.insert_user(nickname, user);
                users.push_back(user); // main() keeps hold of it so it can free it later
                cout << "Added " << display_name << " under the nickname '" << nickname << "'" << endl;
            }
        } else if (command == 2) {
            cout << "Enter the nickname to retrieve:" << endl;
            string nickname;
            cin >> nickname;

            if (!valid_nickname(nickname)) {
                cout << "Nicknames must use the lowercase letters a-z only" << endl;
            } else {
                User* user = book.retrieve_user(nickname);
                if (user == nullptr) {
                    cout << "No contact is saved under '" << nickname << "'" << endl;
                } else {
                    cout << "Nickname: " << user->nickname
                         << " | Display name: " << user->display_name
                         << " | Score: " << user->user_score << endl;
                }
            }
        } else if (command == 3) {
            cout << "Enter the nickname to remove:" << endl;
            string nickname;
            cin >> nickname;

            User* user = valid_nickname(nickname) ? book.retrieve_user(nickname) : nullptr;
            if (user == nullptr) {
                // Checked here so that delete_user is only ever called on a nickname
                // we know is present, and never has to throw.
                cout << "No contact is saved under '" << nickname << "'" << endl;
            } else {
                book.delete_user(nickname);

                // The book has dropped its node, so now main() frees the User itself.
                for (int i = 0; i < (int)users.size(); i++) {
                    if (users[i] == user) {
                        users.erase(users.begin() + i);
                        break;
                    }
                }
                delete user;
                cout << "Removed the contact '" << nickname << "'" << endl;
            }
        } else if (command == 4) {
            cout << "Enter the prefix to search for:" << endl;
            string prefix;
            cin >> prefix;

            if (!valid_nickname(prefix)) {
                cout << "Prefixes must use the lowercase letters a-z only" << endl;
            } else if (book.search(prefix)) {
                cout << "Yes - at least one contact has the prefix '" << prefix << "'" << endl;
            } else {
                cout << "No contact has the prefix '" << prefix << "'" << endl;
            }
        } else if (command == 5) {
            cout << "The contact book holds " << book.size << " contact(s)" << endl;
        } else if (command == 6) {
            cout << menu << endl;
        }

        cout << "Enter command (1-7):" << endl;
        if (!(cin >> command)) {
            // End of input, or something that isn't a number - stop rather than loop forever.
            break;
        }
    }

    // Everything main() allocated, main() cleans up.
    for (int i = 0; i < (int)users.size(); i++) {
        delete users[i];
    }
    users.clear();

    return 0;
}