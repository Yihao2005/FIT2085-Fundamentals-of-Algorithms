#include <iostream>
#include <string>

using namespace std;

struct User{
    string username;
};

int main()
{
    User jane;
    jane.username = "Jane";

    User* janePointer = &jane;

    cout << jane.username << endl;

    cout << janePointer <<endl;

    cout << janePointer->username << endl;

    cout << (*janePointer).username << endl;

}