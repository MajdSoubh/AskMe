#ifndef QUESTION_H
#define QUESTION_H
#include "Account.h"
#include "utility.h"
#include <iostream>
#include <exception>
#include <memory>
#include <iomanip>
#include <string>
class Question : public Comparable
{
private:
    std::string askedByUser;
    std::string askedToUser;
    Date date;
    std::string question;
    int id;
    bool isAnonymous;

public:
    Question(const int, const std::string &, const std::string, const std::string, const bool, const Date); //constructor take a id, question text,username of account who asked,username of account who asked to,is anonymous or not,Date object
    bool compare(std::shared_ptr<Comparable>) const;                                                        //function for compare between objects
    int getId() const;                                                                                      //return id for this object
    std::string getQuestion() const;                                                                        //return question
    std::string getAsker() const;                                                                           //return question asker username
    std::string getAskedTo() const;                                                                         //return question asked to username
    Date getDate() const;                                                                                   //return date for current object
    bool getAnonymousity() const;                                                                           //return question privacy
    friend std::ostream &operator<<(std::ostream &, const Question &);                                      //operator overloading for output question details
};

#endif