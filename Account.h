#ifndef ACCOUNT_H
#define ACCOUNT_H
#include <string>
#include <tuple>
#include <memory>
#include "utility.h"
class Account : public Comparable
{

private:
    int id;
    std::string username;
    std::string password;
    std::string email;

public:
    Account(const int, const std::string &, const std::string &, const std::string &); //constructor initialize new instance with id,username ,password ,email
    std::string getUsername() const;                                                   //return username
    std::string getPassword() const;                                                   //return password
    std::string getEmail() const;                                                      //return email                              
    int getId() const; //return id for current object
    bool compare(std::shared_ptr<Comparable>) const override; //utility function for compare between objects
};

#endif