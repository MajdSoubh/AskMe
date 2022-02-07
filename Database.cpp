#include "Database.h"
#include <iostream>

using namespace std;
Database *Database::db = nullptr;

Database::Database(const string &accountFile, const string &questionFile, const string &answerFile, const char delimiter)
{
    this->accountFile = accountFile;
    this->questionFile = questionFile;
    this->answerFile = answerFile;
    this->delimiter = delimiter;
}
Database Database::getInstance(const string &accountFile, const string &questionFile, const string &answerFile, const char delimiter)
{
    if (db == nullptr)
    {
        db = new Database(accountFile, questionFile, answerFile, delimiter);
    }

    return *db;
}
Database::~Database()
{
    freeInstance();
}
void Database::freeInstance()
{
    if (db != nullptr)
    {
        delete db;
    }
}
vector<string> Database::readFile(const string &file_path) const
{
    vector<string> file_lines;
    file_lines.reserve(50);
    string line;
    fstream in(file_path.c_str(), ios::in);
    if (!in.is_open())
    {
        fstream fi(file_path.c_str(), ios::out);
        fi.close();
        in.clear();
        in.open(file_path.c_str(), ios::in);
    }

    while (!in.eof())
    {

        getline(in, line);
        if (line == "")
            continue;
        file_lines.emplace_back(line);
    }
    in.close();
    return file_lines;
}
void Database::writeEndFile(const string &file, const string &line)
{

    fstream fi(file.c_str(), ios::app);
    fi << line;
    fi.close();
}
string Database::initToWrite(const string &data) const
{
    string temp = "";
    string result = "";
    stringstream ss(data);
    while (!ss.eof())
    {
        getline(ss, temp, delimiter);
        result += temp + delimiter;
    }
    result.pop_back();
    result.push_back('\n');
    return result;
}
int Database::getLastUserId() const
{

    vector<string> lines = readFile(accountFile);
    int max_id = -1;
    for (int i{0}; i < lines.size(); i++)
    {
        vector<string> attribute = splitString(lines[i], delimiter);
        int temp_id = stoi(attribute[0]);
        if (max_id < temp_id)
        {
            max_id = temp_id;
        }
    }
    return max_id;
}
int Database::getLastQuestionId() const
{

    vector<string> lines = readFile(questionFile);
    int max_id = -1;
    for (int i{0}; i < lines.size(); i++)
    {
        vector<string> attribute = splitString(lines[i], delimiter);
        int temp_id = stoi(attribute[0]);
        if (max_id < temp_id)
        {
            max_id = temp_id;
        }
    }
    return max_id;
}
int Database::getLastAnswerId() const
{

    vector<string> lines = readFile(answerFile);
    int max_id = -1;
    for (int i{0}; i < lines.size(); i++)
    {
        vector<string> attribute = splitString(lines[i], delimiter);
        int temp_id = stoi(attribute[0]);
        if (max_id < temp_id)
        {
            max_id = temp_id;
        }
    }
    return max_id;
}
vector<string> Database::splitString(const string &line, const char delimiter) const
{

    vector<string> result;
    result.reserve(50);
    istringstream is(line);
    string data;
    while (!is.eof())
    {

        getline(is, data, delimiter);

        result.emplace_back(data);
    }
    return result;
}
bool Database::compare(shared_ptr<Comparable> first, shared_ptr<Comparable> second)
{
    try
    {

        return first->compare(second);
    }
    catch (exception &e)
    {
        cout << "Compare Function : " << e.what() << endl;
        throw;
    }
}
vector<shared_ptr<Question>> Database::getQuestionsAskedToUserBriefly(const string &username) const
{
    vector<shared_ptr<Question>> questions;
    vector<string> lines = readFile(questionFile);
    for (int i{0}; i < (int)lines.size(); i++)
    {
        vector<string> attribute = splitString(lines[i], delimiter);
        if (attribute[3] == username)
        {
            shared_ptr<Account> askedBy = getAccount(attribute[2]);
            shared_ptr<Account> askedTo = getAccount(attribute[3]);
            vector<string> date_attribute = splitString(attribute[5], ':');
            Date date(stoi(date_attribute[5]), stoi(date_attribute[4]), stoi(date_attribute[3]), stoi(date_attribute[2]), stoi(date_attribute[1]), stoi(date_attribute[0]));

            questions.push_back(make_shared<Question>(stoi(attribute[0]), attribute[1], askedBy->getUsername(), askedTo->getUsername(), stoi(attribute[4]), date));
        }
    }
    sort(questions.begin(), questions.end(), compare);
    return questions;
}
vector<pair<shared_ptr<Question>, vector<shared_ptr<Answer>>>> Database::getQuestionsAskedToUser(const string &username) const
{

    vector<pair<shared_ptr<Question>, vector<shared_ptr<Answer>>>> result;

    vector<vector<shared_ptr<Answer>>> Answers(100);
    vector<string> lines = readFile(answerFile);
    for (int i{0}; i < (int)lines.size(); i++)
    {
        vector<string> attribute = splitString(lines[i], delimiter);

        int answer_id = stoi(attribute[0]);
        int question_id = stoi(attribute[1]);
        string answer = attribute[2];
        vector<string> date_attribute = splitString(attribute[3], ':');
        Date date(stoi(date_attribute[5]), stoi(date_attribute[4]), stoi(date_attribute[3]), stoi(date_attribute[2]), stoi(date_attribute[1]), stoi(date_attribute[0]));
        Answers[question_id].push_back(make_shared<Answer>(answer_id, question_id, answer, date));
    }

    lines.clear();

    vector<shared_ptr<Question>> questions;
    lines = readFile(questionFile);
    for (int i{0}; i < (int)lines.size(); i++)
    {
        vector<string> attribute = splitString(lines[i], delimiter);
        if (attribute[3] == username)
        {
            shared_ptr<Account> askedBy = getAccount(attribute[2]);
            shared_ptr<Account> askedTo = getAccount(attribute[3]);
            vector<string> date_attribute = splitString(attribute[5], ':');
            Date date(stoi(date_attribute[5]), stoi(date_attribute[4]), stoi(date_attribute[3]), stoi(date_attribute[2]), stoi(date_attribute[1]), stoi(date_attribute[0]));

            questions.push_back(make_shared<Question>(stoi(attribute[0]), attribute[1], askedBy->getUsername(), askedTo->getUsername(), stoi(attribute[4]), date));
        }
    }
    sort(questions.begin(), questions.end(), compare);
    for (int i{0}; i < (int)questions.size(); i++)
    {

        pair<shared_ptr<Question>, vector<shared_ptr<Answer>>> pr = make_pair(questions[i], Answers[questions[i]->getId()]);
        result.push_back(pr);
    }
    return result;
}
vector<pair<shared_ptr<Question>, vector<shared_ptr<Answer>>>> Database::getQuestionsAskedByUser(const string &username) const
{
    vector<pair<shared_ptr<Question>, vector<shared_ptr<Answer>>>> result;

    vector<vector<shared_ptr<Answer>>> Answers(100);
    vector<string> lines = readFile(answerFile);
    for (int i{0}; i < (int)lines.size(); i++)
    {
        vector<string> attribute = splitString(lines[i], delimiter);

        int answer_id = stoi(attribute[0]);
        int question_id = stoi(attribute[1]);
        string answer = attribute[2];
        vector<string> date_attribute = splitString(attribute[3], ':');
        Date date(stoi(date_attribute[5]), stoi(date_attribute[4]), stoi(date_attribute[3]), stoi(date_attribute[2]), stoi(date_attribute[1]), stoi(date_attribute[0]));
        Answers[question_id].push_back(make_shared<Answer>(answer_id, question_id, answer, date));
    }

    lines.clear();

    vector<shared_ptr<Question>> questions;
    lines = readFile(questionFile);
    for (int i{0}; i < (int)lines.size(); i++)
    {
        vector<string> attribute = splitString(lines[i], delimiter);
        if (attribute[2] == username)
        {
            shared_ptr<Account> askedBy = getAccount(attribute[2]);
            shared_ptr<Account> askedTo = getAccount(attribute[3]);
            vector<string> date_attribute = splitString(attribute[5], ':');
            Date date(stoi(date_attribute[5]), stoi(date_attribute[4]), stoi(date_attribute[3]), stoi(date_attribute[2]), stoi(date_attribute[1]), stoi(date_attribute[0]));

            questions.push_back(make_shared<Question>(stoi(attribute[0]), attribute[1], askedBy->getUsername(), askedTo->getUsername(), stoi(attribute[4]), date));
        }
    }
    sort(questions.begin(), questions.end(), compare);
    for (int i{0}; i < (int)questions.size(); i++)
    {

        pair<shared_ptr<Question>, vector<shared_ptr<Answer>>> pr = make_pair(questions[i], Answers[questions[i]->getId()]);
        result.push_back(pr);
    }
    return result;
}
bool Database::isUserExist(const string &username) const
{
    vector<string> attribute;
    vector<string> lines = readFile(accountFile);
    for (int i = 0; i < lines.size(); i++)
    {
        attribute = splitString(lines[i], delimiter);
        if (attribute[1] == username)
        {
            return 1;
        }
    }
    return 0;
}
shared_ptr<Account> Database::getAccount(const string &username) const
{
    vector<string> attribute;
    shared_ptr<Account> account;
    vector<string>
        lines = readFile(accountFile);
    for (int i = 0; i < lines.size(); i++)
    {
        attribute = splitString(lines[i], delimiter);
        if (attribute[1] == username)
        {
            account = make_shared<Account>(stoi(attribute[0]), attribute[1], attribute[2], attribute[3]);
            break;
        }
    }
    return account;
}
shared_ptr<Account> Database::addAccount(const std::string &username, const std::string &password, const std::string &email)
{
    int id = getLastUserId() + 1;
    shared_ptr<Account> account = make_shared<Account>(id, username, password, email);
    string line = to_string(id) + delimiter + username + delimiter + password + delimiter + email + '\n';
    writeEndFile(accountFile, line);
    return account;
}
bool Database::authenticateUser(const string &username, const string &password) const
{
    vector<string> attribute;
    vector<string>
        lines = readFile(accountFile);
    for (int i = 0; i < lines.size(); i++)
    {
        attribute = splitString(lines[i], delimiter);
        if (attribute[1] == username)
        {
            if (attribute[2] == password)
                return 1;
            break;
        }
    }
    return 0;
}
vector<shared_ptr<Account>> Database::getAllAccount() const
{
    vector<string> attribute;
    vector<shared_ptr<Account>> accounts;
    vector<string>
        lines = readFile(accountFile);
    for (int i = 0; i < lines.size(); i++)
    {
        attribute = splitString(lines[i], delimiter);

        shared_ptr<Account> account = make_shared<Account>(stoi(attribute[0]), attribute[1], attribute[2], attribute[3]);
        accounts.push_back(account);
    }
    sort(accounts.begin(), accounts.end(), compare);
    return accounts;
}
shared_ptr<Question> Database::addQuestion(const string &from, const string &to, const string &question, bool isAnonymous)
{
    int id = getLastQuestionId() + 1;
    shared_ptr<Account> _from = getAccount(from);
    shared_ptr<Account> _to = getAccount(to);
    assert(_from);
    assert(_to);
    Date date = Date::Now();
    shared_ptr<Question> q = make_shared<Question>(id, question, from, to, isAnonymous, date);
    string line = to_string(id) + delimiter + question + delimiter + from + delimiter + to + delimiter + to_string(isAnonymous) + delimiter + to_string(date.getYear()) + ':' + to_string(date.getMon()) + ':' + to_string(date.getDay()) + ':' + to_string(date.getHour()) + ':' + to_string(date.getMin()) + ':' + to_string(date.getSec()) + '\n';
    writeEndFile(questionFile, line);
    return q;
}
shared_ptr<Answer> Database::addAnswer(const int question_id, const string &answer)
{
    int id = getLastAnswerId() + 1;
    Date date = Date::Now();
    shared_ptr<Answer> a = make_shared<Answer>(id, question_id, answer, date);
    string line = to_string(id) + delimiter + to_string(question_id) + delimiter + answer + delimiter + to_string(date.getYear()) + ':' + to_string(date.getMon()) + ':' + to_string(date.getDay()) + ':' + to_string(date.getHour()) + ':' + to_string(date.getMin()) + ':' + to_string(date.getSec()) + '\n';
    writeEndFile(answerFile, line);
    return a;
}
shared_ptr<Question> Database::getQuestionById(const int id) const
{
    vector<string> attribute;
    shared_ptr<Question> question;
    vector<string>
        lines = readFile(questionFile);
    for (int i = 0; i < lines.size(); i++)
    {
        attribute = splitString(lines[i], delimiter);
        if (stoi(attribute[0]) == id)
        {
            shared_ptr<Account> _from = getAccount(attribute[2]);
            shared_ptr<Account> _to = getAccount(attribute[3]);
            assert(_from);
            assert(_to);
            vector<string> date_attribute = splitString(attribute[5], ':');
            Date date(stoi(date_attribute[5]), stoi(date_attribute[4]), stoi(date_attribute[3]), stoi(date_attribute[2]), stoi(date_attribute[1]), stoi(date_attribute[0]));

            question = make_shared<Question>(stoi(attribute[0]), attribute[1], _from->getUsername(), _to->getUsername(), stoi(attribute[4]), date);
            break;
        }
    }
    return question;
}
void Database::deleteQuestion(const int id)
{
    vector<shared_ptr<Question>> vec;
    vector<string> lines = readFile(questionFile);
    vector<string> attribute;
    vector<string> mod_lines;

    for (int i{0}; i < lines.size(); i++)
    {
        attribute = splitString(lines[i], delimiter);
        if (stoi(attribute[0]) != id)
        {
            string line = attribute[0] + delimiter + attribute[1] + delimiter + attribute[2] + delimiter + attribute[3] + delimiter + attribute[4] + delimiter + attribute[5];
            mod_lines.push_back(line);
        }
    }
    remove(questionFile.c_str());
    for (int i{0}; i < mod_lines.size(); i++)
    {
        writeEndFile(questionFile, mod_lines[i]);
    }
}
void Database::deleteAnswer(const int id)
{
    vector<shared_ptr<Answer>> vec;
    vector<string> lines = readFile(answerFile);
    vector<string> attribute;
    vector<string> mod_lines;

    for (int i{0}; i < lines.size(); i++)
    {
        attribute = splitString(lines[i], delimiter);
        if (stoi(attribute[0]) != id)
        {
            string line = attribute[0] + delimiter + attribute[1] + delimiter + attribute[2] + delimiter + attribute[3];
            mod_lines.push_back(line);
        }
    }
    remove(answerFile.c_str());
    for (int i{0}; i < mod_lines.size(); i++)
    {
        writeEndFile(answerFile, mod_lines[i]);
    }
}
vector<shared_ptr<Question>> Database::getQuestionsAskedByUserBriefly(const string &username) const
{
    vector<shared_ptr<Question>> questions;
    vector<string> lines = readFile(questionFile);
    for (int i{0}; i < (int)lines.size(); i++)
    {
        vector<string> attribute = splitString(lines[i], delimiter);
        if (attribute[2] == username)
        {
            shared_ptr<Account> askedBy = getAccount(attribute[2]);
            shared_ptr<Account> askedTo = getAccount(attribute[3]);
            assert(askedBy);
            assert(askedTo);
            vector<string> date_attribute = splitString(attribute[5], ':');
            Date date(stoi(date_attribute[5]), stoi(date_attribute[4]), stoi(date_attribute[3]), stoi(date_attribute[2]), stoi(date_attribute[1]), stoi(date_attribute[0]));

            questions.push_back(make_shared<Question>(stoi(attribute[0]), attribute[1], askedBy->getUsername(), askedTo->getUsername(), stoi(attribute[4]), date));
        }
    }
    sort(questions.begin(), questions.end(), compare);
    return questions;
}
vector<shared_ptr<Answer>> Database::getAnswersToQuestion(const int id)
{
    vector<shared_ptr<Answer>> Answers;
    vector<string> lines = readFile(answerFile);
    for (int i{0}; i < (int)lines.size(); i++)
    {
        vector<string> attribute = splitString(lines[i], delimiter);
        if (stoi(attribute[1]) == id)
        {

            int answer_id = stoi(attribute[0]);
            int question_id = stoi(attribute[1]);
            string answer = attribute[2];
            vector<string> date_attribute = splitString(attribute[3], ':');
            Date date(stoi(date_attribute[5]), stoi(date_attribute[4]), stoi(date_attribute[3]), stoi(date_attribute[2]), stoi(date_attribute[1]), stoi(date_attribute[0]));
            Answers.push_back(make_shared<Answer>(answer_id, question_id, answer, date));
        }
    }

    return Answers;
}
shared_ptr<Answer> Database::getAnswerById(const int id) const
{
    vector<string> attribute;
    shared_ptr<Answer> answer;
    vector<string>
        lines = readFile(answerFile);
    for (int i = 0; i < lines.size(); i++)
    {
        attribute = splitString(lines[i], delimiter);
        if (stoi(attribute[0]) == id)
        {

            vector<string> date_attribute = splitString(attribute[3], ':');
            Date date(stoi(date_attribute[5]), stoi(date_attribute[4]), stoi(date_attribute[3]), stoi(date_attribute[2]), stoi(date_attribute[1]), stoi(date_attribute[0]));
            answer = make_shared<Answer>(stoi(attribute[0]), stoi(attribute[1]), attribute[2], date);
            break;
        }
    }
    return answer;
}