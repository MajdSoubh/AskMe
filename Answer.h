#ifndef ANSWER_H
#define ANSWER_H
#include <string>
#include <memory>
#include "utility.h"
#include "Question.h"
class Answer : public Comparable
{
private:
    int id;
    std::string answer;
    int questionId;
    Date date;

public:
    Answer(const int, const int, const std::string &, const Date); //constructor take id, question id, answer, date object
    std::string getAnswer() const;                                 //return the answer
    Date getDate() const;                                          //return the date for this object
    int getQuestionId() const;                                     //return question ptr
    int getId() const;                                             //return the id of this current object
    bool compare(std::shared_ptr<Comparable>) const override;      //utility function for compare between objects
    friend std::ostream &operator<<(std::ostream &, const Answer &);        //operator overloading for output answer details
};

#endif