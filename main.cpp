#include "AskMe.h"

std::string accountFile = "Account.txt";
std::string questionFile = "Question.txt";
std::string answerFile = "Answer.txt";
char delimiter = ',';

int main()
{
    Database dp =Database::getInstance(accountFile, questionFile, answerFile, delimiter);//singleton design for database object
    AskMe service(dp);
    service.run();
    Database::freeInstance();

    std::cin.get();
}