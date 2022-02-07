#include "Account.h"
using namespace std;

Account::Account(const int id, const string &username, const string &password, const string &email) : id(id), username(username), password(password), email(email)
{
}

string Account::getEmail() const
{
    return email;
}
string Account::getUsername() const
{
    return username;
}
string Account::getPassword() const
{
    return password;
}
int Account::getId() const
{
    return id;
}

bool Account::compare(shared_ptr<Comparable> c) const
{
    shared_ptr<Account> account;
    if (account = dynamic_pointer_cast<Account>(c))
    {
        string c_username = account->getUsername();
        return tie(username) < tie(c_username);             //alphaptical sort
    }
    else
    {
        throw bad_cast();
    }
}