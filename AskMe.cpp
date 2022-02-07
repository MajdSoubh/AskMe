#include "AskMe.h"

using namespace std;
enum class SystemOption
{
    LOGIN = 1,
    SIGNUP
};
AskMe::AskMe(Database &__db) : db(__db)
{
}

void AskMe::run()
{
    while (1)
    {
        cout << "AskMe\n\n";
        displayMenu(vector<string>{"login", "signup"});
        cout << "\nYour Choice : ";
        int choice = inputDigit(1, 3);

        SystemOption option = static_cast<SystemOption>(choice);

        switch (option)
        {
        case SystemOption::LOGIN:
            login();
            break;
        case SystemOption::SIGNUP:
            signUp();
            break;
        default:
            break;
        }
    }
}

void AskMe::login()
{
    system("cls");
    string username;
    string password;
    cout << "Username : ";
    username = inputString(15);
    cout << "Password : ";
    password = inputString(16);

    if (!db.authenticateUser(username, password))
    {
        system("cls");
        cout << "Credential is not correct \n\n";
        return;
    }
    System service(username, db);
    system("cls");
    service.run();
    return;
}
void AskMe::signUp()
{
    system("cls");
    string username;
    string password;
    string email;
    cout << "Username : ";
    username = inputString(30);
    if (db.isUserExist(username))
    {
        cout << "\nthis username is not available !\n";
        return;
    }
    cout << "Password : ";
    password = inputString(16);
    cout << "email : ";
    email = inputString(30);
    db.addAccount(username, password, email);
    System service(username, db);
    system("cls");
    service.run();
    return;
}
void AskMe::displayMenu(const vector<string> &menu) const
{
    for (int i = 0; i < (int)menu.size(); i++)
    {
        cout << i + 1 << "- " << menu[i] << endl;
    }
}
int AskMe::inputDigit(int low, int high) const
{
    int number;
    while (1)
    {
        string input;
        bool valid = 1;
        getline(cin, input);
        if (input.size() == 0)
            continue;
        if (input.size() > 9)
        {
            valid = 0;
        }
        for (size_t i = 0; i < input.size() && valid; i++)
        {
            if (isdigit(input[i]) == false)
            {
                valid = 0;
                break;
            }
        }
        if (valid)
        {

            number = stoi(input);
            if (number < low || number > high)
                valid = 0;
        }
        if (valid)
        {
            break;
        }
        else
        {
            cout << "\nPlease input number between " << low << "-" << high << " : ";
        }
    }
    return number;
}
string AskMe::inputString(int size) const
{
    string input;
    while (1)
    {
        bool valid = 1;
        getline(cin, input);
        if (input.size() == 0)
        {
            continue;
        }
        else if (input.size() > size)
        {
            valid = 0;
        }
        for (int i = 0; i < input.size() && valid; i++)
        {
            if (input[i] == ' ')
            {
                valid = 0;
                break;
            }
        }
        if (valid)
        {
            return input;
        }
        else
        {
            cout << "\ninvalid input please input data without spaces and with size equal or less than " << size << " !\n";
        }
    }
}