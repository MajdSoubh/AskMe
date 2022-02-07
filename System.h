#ifndef SYSTEM_H
#define SYSTEM_H
#include "Database.h"
#include "Account.h"
#include <iostream>

class System
{

public:
    System(const std::string &, const Database &); //constructor take current username ,database object
    ~System();                      //destructor
    void run();                  //start Askme show the menu of options
    void askQuestion();          //for asking new question
    void questionFromMe() const; //print all question asked to current user
    void viewUserInbox() const;  //print all question current user is asked to other users
    void answerQuestion();       //answer questions for current user
    void deleteQuestion();       //delete questions for current user
    void deleteAnswer();         //delete answers for current user

private:
    std::string currentUsername;
    Database db;
    void displayMenu(const std::vector<std::string> &) const; //utility function for display menu
    int inputDigit(int, int) const;                           //utility function for input digit in range [low,high]
    std::string inputString(int size) const;                  //utility function for input string with no spaces and size equal or less than size
};

#endif