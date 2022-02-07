#ifndef ASKME_H
#define ASKME_H
#include "Database.h"
#include "System.h"
#include <iostream>
#include <vector>
#include <string>

class AskMe
{
public:
    AskMe(Database &); //constructor ;take database parameter and initialized
    void run();        //start askme
    void login();      //perform login
    void signUp();     //perform signup
private:
    void displayMenu(const std::vector<std::string> &) const; //utility function for display menu
    int inputDigit(int, int) const;                           //utility function for input digit in range [low,high]
    std::string inputString(int size) const;                  //utility function for input string with no spaces and size equal or less than size
    Database db;
};

#endif