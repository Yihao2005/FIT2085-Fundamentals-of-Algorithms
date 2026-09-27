#include <iostream>
#include <string>

#include "hash_table.h"

using namespace std;

// Record of the followers of one user
struct FollowersRecord {
    // The number of followers this record represents
    int n = 0;
    // The string array of those followers' usernames
    string* followers=nullptr;

    string serialise() {
        // ...
        if (n == 0)
        {
            return "";
        }

        string result = "";
        for (int i = 0; i < n-1; i++)
        {
            result += followers[i];
            result += "-";
        }
        result += followers[n-1];
        return result;
    }

    void deserialise(string record) {
        // ...
        delete[] followers;
        followers = nullptr;

        if (record == "")
        {
            n = 0;
            return;
        }

        n = 1;

        for (int i = 0; i < record.length(); i++)
        {
            if (record[i] == '-')
            {
                n++;
            }
        }

        followers = new string[n];
        int follower_index = 0;

        for (int i = 0; i < record.length(); i++)
        {
            if (record[i] == '-')
            {
                follower_index++;
                continue;
            }
            followers[follower_index] += record[i];
        }
    }
};

struct FollowersDatabase {
    HashTable ht;

    void add_follower(string username, string follower) {
        // ...
        string followers;

        try
        {
            followers = ht.search(username);
        }
        catch (string error)
        {
            ht.insert(username, follower);
            return;
        }


        FollowersRecord followers_record;
        followers_record.deserialise(followers);

        if (followers_record.n == 0)
        {
            ht.insert(username, follower);
            return;
        }

        for (int i = 0; i < followers_record.n; i++)
        {
            if (followers_record.followers[i] == follower)
            {
                return;
            }
        }

        string result = followers_record.serialise();
        result+= "-"+follower;
        ht.insert(username, result);
    }

    void delete_follower(string username, string follower) {
        // ...
        //string followers = ht.search(username);

        string followers;

        try
        {
            followers = ht.search(username);
        }catch (string error)
        {
            cout<< "User does not have any followers";
            return;
        }

        FollowersRecord followers_record;
        followers_record.deserialise(followers);

        if (followers_record.n == 0)
        {
            return;
        }

        bool could_delete = false;

        for (int i = 0; i < followers_record.n; i++)
        {
            if (followers_record.followers[i] == follower)
            {
                could_delete = true;
                break;
            }
        }

        if (!could_delete)
        {
            return;
        }



        if (followers_record.n == 1)
        {
            ht.insert(username, "");
            return;
        }

        string result = "";
        for (int i = 0; i < followers_record.n-1; i++)
        {
            if (followers_record.followers[i] == follower) continue;
            result += followers_record.followers[i];
            result += "-";
        }

        if (followers_record.followers[followers_record.n - 1] != follower)
        {
            result += followers_record.followers[followers_record.n-1];
        }else
        {
            result.pop_back();
        }
        ht.insert(username, result);
    }

    void print_followers(string username) {
        // ...
        // At some point, you will need the follow print statement:
        // cout << "User does not have any followers" << endl;
        string followers;
        try
        {
            followers = ht.search(username);
        }catch (string error)
        {
            cout<< "User does not have any followers"<<endl;
            return;
        }
        FollowersRecord followers_record;
        followers_record.deserialise(followers);
        if (followers_record.n == 0)
        {
            cout << "User does not have any followers" << endl;
            return;
        }
        for (int i = 0; i < followers_record.n; i++)
        {
            cout << followers_record.followers[i] << endl;
        }
    }
};

int ontrack_tests() {
    FollowersDatabase db;

    string menu = "1. Add new follower for a user\n2. Remove a follower from a user\n3. Print all followers of a user\n4. See the menu\n5. Quit";

    cout << menu << endl;
    int command = 0;
    cout << "Enter command (1-5):" << endl;
    cin >> command;

    while (command != 5) {
        // The hash table methods throw when they are empty / non existant, so we wrap
        // each action in a try/catch and report the error instead of crashing.
        try {
            if (command == 1) {
                cout << "Enter the username of the user who was followed:" << endl;
                string username;
                cin >> username;

                cout << "Enter the username of the follower:" << endl;
                string follower;
                cin >> follower;

                db.add_follower(username, follower);
            } else if (command == 2) {
                cout << "Enter the username of the user who was unfollowed:" << endl;
                string username;
                cin >> username;

                cout << "Enter the username of the unfollower:" << endl;
                string unfollower;
                cin >> unfollower;

                db.delete_follower(username, unfollower);
            } else if (command == 3) {
                cout << "Enter a username to see their followers:" << endl;
                string username;
                cin >> username;

                db.print_followers(username);
            } else if (command == 4) {
                cout << menu << endl;
            } else {
                cout << "Unknown command." << endl;
            }
        } catch (const string error) {
            cout << "Error: " << error << endl;
        }

        cout << "Enter command (1-5):" << endl;
        cin >> command;
        if (!cin) {
            cerr << "ERROR: Could not read next command" << endl;
            return 1;
        }
    }
    return 0;
}

int main(int argc, char** argv) {
    if (argc == 2) {
        return ontrack_tests();
    }

    //test
    FollowersDatabase db;
    db.ht.insert("A", "");
    db.ht.insert("B", "");

    //Test add_follower()
    cout<< "Test add_follower()"<<endl;
    cout<< "Add A1 for A:" << endl;
    db.add_follower("A","A1");
    db.print_followers("A");

    cout << "\nAdd B1, B2, B3 for B:"<<endl;
    db.add_follower("B","B1");
    db.add_follower("B","B2");
    db.add_follower("B","B3");
    db.print_followers("B");

    //Test delete_follower()
    cout<< "\nTest delete_follower()"<<endl;
    cout<< "Delete B2 for B:" << endl;
    db.delete_follower("B","B2");
    db.print_followers("B");


    return 0;
}