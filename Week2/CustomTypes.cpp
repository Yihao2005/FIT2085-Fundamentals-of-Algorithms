#include <iostream>
#include <string>

using namespace std;

struct User
{
    string username;
    string password;
    int created_at;
    bool banned;

    void ban()
    {
        banned = true;
        password = "";
    }

    void reset_password(int length)
    {
        password = random_string_generator(length);
    }

};

int main()
{
    User first_user;
    first_user.reset_password(12);
    first_user.ban();
    // This will print an empty string, because the ban method cleared it.
    cout << first_user.password << endl;
}
