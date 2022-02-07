#include "Question.h"
using namespace std;
Question::Question(const int _id, const string &_question, const string _askedBy, const string _askedTo, const bool _isAnonymous, const Date _date) : id(_id), question(_question), askedByUser(_askedBy), askedToUser(_askedTo), isAnonymous(_isAnonymous), date(_date)
{
}

int Question::getId() const
{
    return id;
}
bool Question::compare(shared_ptr<Comparable> q) const
{
    shared_ptr<Question> question;
    if (question = dynamic_pointer_cast<Question>(q))
    {
        Date d1 = question->getDate();

        return tie(date) > tie(d1);
    }
    else
    {
        throw bad_cast();
    }
}
string Question::getQuestion() const
{
    return question;
}
string Question::getAsker() const
{
    return askedByUser;
}

string Question::getAskedTo() const
{
    return askedToUser;
}
Date Question::getDate() const
{
    return date;
}
bool Question::getAnonymousity() const
{
    return isAnonymous;
}
ostream &operator<<(ostream &out, const Question &q)
{
    out << setiosflags(ios::left) << setw(6) << q.getId() << " " << q.getDate().getYear() << ":" << q.getDate().getMon() << ":" << q.getDate().getDay() << "    " << setw(14) << ((q.getAnonymousity()) ? "(Anonymous) " : q.getAsker()) << " " << setw(10) << q.getAskedTo() << " " << q.getQuestion();
    return out;
}
