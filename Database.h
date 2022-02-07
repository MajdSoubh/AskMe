#ifndef DATABASE_H
#define DATABASE_H
#include "Account.h"
#include "Question.h"
#include "utility.h"
#include "Answer.h"
#include <string>
#include <cassert>
#include <iostream>
#include <algorithm>
#include <memory>
#include <vector>
#include <sstream>
#include <fstream>
#include <utility>
#include <optional>
class Database
{

private:
    std::string accountFile;
    std::string questionFile;
    std::string answerFile;
    char delimiter;
    static Database *db;
    Database(const std::string &, const std::string &, const std::string &, const char); //constructor ;take account file,question file, answer file paths
    std::vector<std::string> readFile(const std::string &) const;                        //take a path for a file and read it line by line then return vector contain all lines
    std::vector<std::string> splitString(const std::string &, const char) const;         //utility function for split line from file to his attribute and returned by vector
    void writeEndFile(const std::string &, const std::string &);                        //utility function for write line to the end of file
    std::string initToWrite(const std::string &) const; //take a string ;replace evey spaces in the string to delimiter then returned

public:
    static bool compare(std::shared_ptr<Comparable>, std::shared_ptr<Comparable>);                                                              //generic function to compare between two object with same type
    static Database getInstance(const std::string &, const std::string &, const std::string &, const char);                                    //function allow to get one instance from this class
    ~Database();                                                                                                                                //destructor
    static void freeInstance();                                                                                                                 //deallocate the active instance
    int getLastUserId() const;                                                                                                                  //utility function for get max User id in account file
    int getLastQuestionId() const;                                                                                                              //utility function for get max Question id in account file
    int getLastAnswerId() const;                                                                                                                //utility function for get max Answer id in account file
    bool isUserExist(const std::string &username) const;                                                                                        //take a username ;return if this username exist in the database
    std::shared_ptr<Account> getAccount(const std::string &) const;                                                                             //take a username ;return the two values first one return true if account is exist and false if not ,second one return the account object if exist
    bool authenticateUser(const std::string &, const std::string &) const;                                                                      //take a username ;password ;return true plus pointer to account object if exist or false plus null pointer if not exist
    std::vector<std::shared_ptr<Account>> getAllAccount() const;                                                                                //return all registerd accounts
    std::vector<std::pair<std::shared_ptr<Question>, std::vector<std::shared_ptr<Answer>>>> getQuestionsAskedToUser(const std::string &) const; //take a username ;return all questions asked to this user with answers if exist
    std::vector<std::shared_ptr<Answer>> getAnswersToQuestion(const int);                                                                       //take question id ; return all answers to question
    std::vector<std::pair<std::shared_ptr<Question>, std::vector<std::shared_ptr<Answer>>>> getQuestionsAskedByUser(const std::string &) const; //take a username ;return all questions asked by this user with answers if exist
    std::shared_ptr<Question> getQuestionById(const int) const;                                                                                 //return question ptr associated with specific id
    std::shared_ptr<Answer> getAnswerById(const int) const;                                                                                     //return Answer ptr associated with specific id
    std::vector<std::shared_ptr<Question>> getQuestionsAskedToUserBriefly(const std::string &) const;                                           //take username ;return questions asked to user without answers
    std::vector<std::shared_ptr<Question>> getQuestionsAskedByUserBriefly(const std::string &) const;                                           //take username ;return questions asked by user without answers
    std::shared_ptr<Question> addQuestion(const std::string &, const std::string &, const std::string &, const bool);                           //add new question to database take , asker username , answerer username , question , is anonymous ; return the question ptr object
    std::shared_ptr<Account> addAccount(const std::string &, const std::string &, const std::string &);                                         //Create new Account
    std::shared_ptr<Answer> addAnswer(const int, const std::string &);                                                                          //take question id , answer text ; adding new answer for specific question
    void deleteQuestion(const int);                                                                                                             //delete question from question file
    void deleteAnswer(const int);                                                                                                               //delete answer from answer file
};

#endif