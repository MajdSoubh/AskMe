#include "Answer.h"
using namespace std;
Answer::Answer(const int _id, const int _questionId, const std::string &_answer, const Date _date) : id(_id), questionId(_questionId), answer(_answer), date(_date)
{
}

string Answer::getAnswer() const
{
    return answer;
}

int Answer::getId() const
{
    return id;
}

Date Answer::getDate() const
{
    return date;
}
int Answer::getQuestionId() const
{
    return questionId;
}
bool Answer::compare(shared_ptr<Comparable> c) const
{
    shared_ptr<Answer> answer;
    if (answer = dynamic_pointer_cast<Answer>(c))
    {
        Date d2 = answer->getDate();
        return tie(date) < tie(d2);
    }
    else
    {
        throw bad_cast();
    }
}
ostream &operator<<(ostream &out, const Answer &q)
{
    out << setiosflags(ios::left) << setw(6) << q.getId() << " " << q.getDate().getYear() << ":" << q.getDate().getMon() << ":" << q.getDate().getDay() << "    " << setw(14) << q.getAnswer();
    return out;
}