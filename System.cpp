#include "System.h"
using namespace std;
enum class SystemOptions
{
    INBOX = 1,
    QUESTION_FROM_ME,
    ASK_QUESTION,
    ANSWER_QUESTION,
    DELETE_QUESTION,
    DELETE_ANSWER

};

System::System(const string &username, const Database &_db) : currentUsername(username), db(_db)
{
}
System::~System()
{
}
void System::run()
{
    while (1)
    {
        cout << "AskMe :\n\n";
        displayMenu(vector<string>{"My Inbox", "Question From Me", "Ask Question", "Answer Question", "Delete Question", "Delete Answer"});
        cout << "\nYour Choice : ";
        int choice = inputDigit(1, 7);
        SystemOptions option = static_cast<SystemOptions>(choice);
        switch (option)
        {
        case SystemOptions::INBOX:
            viewUserInbox();
            break;

        case SystemOptions::QUESTION_FROM_ME:
            questionFromMe();
            break;

        case SystemOptions::ASK_QUESTION:
            askQuestion();
            break;

        case SystemOptions::ANSWER_QUESTION:
            answerQuestion();
            break;

        case SystemOptions::DELETE_QUESTION:
            deleteQuestion();
            break;
        case SystemOptions::DELETE_ANSWER:
            deleteAnswer();
            break;
        }
    }
}
void System::deleteQuestion()
{
    system("cls");
    displayMenu(vector<string>{"Select From My Inbox", "Enter Question Id "});
    int choice;
    int qid;
    cout << "\nYour Choice : ";
    choice = inputDigit(1, 2);
    vector<int> selected_questions;
    system("cls");
    if (choice == 1)
    {
        vector<shared_ptr<Question>> questions = db.getQuestionsAskedByUserBriefly(currentUsername);
        vector<string> question_list;
        for (int i{0}; i < questions.size(); i++)
        {
            question_list.push_back("Asked to : " + questions[i]->getAskedTo() + "   " + "Question : " + questions[i]->getQuestion());
        }
        displayMenu(question_list);
        cout << "\nSelected Questions To Delete (input end with 0) : ";

        int selected = -1;
        while (1)
        {
            selected = inputDigit(0, question_list.size());
            if (selected == 0)
                break;
            selected_questions.push_back(questions[selected - 1]->getId());
        }
    }
    else
    {
        cout << "Enter Question IDs (input end with 0) : ";
        string selected = "";
        while (1)
        {
            selected = inputString(10);
            if (selected == "0")
                break;
            if (db.getQuestionById(stoi(selected))->getAsker() != currentUsername)
            {
                cout << "\nYou Can't Delete This Question  ( ID : " << selected << " )" << endl;
            }
            else
            {
                selected_questions.push_back(stoi(selected));
            }
        }
    }
    if (selected_questions.size() == 0)
        return;
    for (int i{0}; i < selected_questions.size(); i++)
    {
        db.deleteQuestion(selected_questions[i]);
    }
    system("cls");
    cout << "Operation Complete\n";
    cout << "Deleted " << selected_questions.size() << " Question/s\n\n";
}
void System::deleteAnswer()
{
    system("cls");

    displayMenu(vector<string>{"Select Answers From Question Menu", "Enter Answers IDs"});
    cout << "\nChoice : ";
    int choice = inputDigit(1, 2);
    system("cls");
    vector<int> selected_answers;

    if (choice == 1)
    {
        vector<string> display_question;
        vector<shared_ptr<Question>> questions = db.getQuestionsAskedToUserBriefly(currentUsername);
        for (int i{0}; i < questions.size(); i++)
        {
            display_question.push_back(questions[i]->getQuestion());
        }
        while (1)
        {

            displayMenu(display_question);
            int selected = -1;
            cout << "\nSelected Question To Delete Answer ( exit with 0 ) : ";
            selected = inputDigit(0, display_question.size());
            if (selected == 0)
                return;
            shared_ptr<Question> q = questions[selected - 1];
            vector<shared_ptr<Answer>> answers = db.getAnswersToQuestion(q->getId());
            if (answers.size() == 0)
            {
                system("cls");
                cout << "There is No Answers for this Question\n";
                continue;
            }
            else
            {
                system("cls");
                vector<string> answer_list;
                for (int i{0}; i < answers.size(); i++)
                {
                    answer_list.push_back(answers[i]->getAnswer());
                }
                displayMenu(answer_list);
                int selected_answer = -1;
                cout << "\nSelected Answers To Delete ( input end with 0 ) : ";

                while (1)
                {
                    selected_answer = inputDigit(0, answer_list.size());
                    if (selected_answer == 0)
                        break;
                    selected_answers.push_back(answers[selected_answer - 1]->getId());
                }
            }
            if (selected_answers.size() != 0)
                break;
        }
    }
    else
    {
        cout << "Enter Answer ID : ";
        int id;
        cin >> id;
        vector<pair<shared_ptr<Question>, vector<shared_ptr<Answer>>>> answers = db.getQuestionsAskedToUser(currentUsername);
        for (int i{0}; i < answers.size(); i++)
        {
            for (int j{0}; j < answers[i].second.size(); j++)
            {
                if (answers[i].second[j]->getId() == id)
                {
                    selected_answers.push_back(answers[i].second[j]->getId());
                    break;
                }
            }
        }
        if (selected_answers.size() == 0)
        {
            system("cls");
            cout << "You Don't have any Answer with this ID ! \n\n";
            return;
        }
    }
    if (selected_answers.size() == 0)
        return;
    for (int i{0}; i < selected_answers.size(); i++)
    {
        db.deleteAnswer(selected_answers[i]);
    }
    system("cls");
    cout << "Operation Complete\n";
    cout << "Deleted " << selected_answers.size() << " Answers \n\n";
}
void System::answerQuestion()
{
    system("cls");
    vector<shared_ptr<Question>> questions = db.getQuestionsAskedToUserBriefly(currentUsername);
    if (questions.size() == 0)
    {
        cout << "You Don't Have any Question ! \n\n";
        return;
    }
    displayMenu(vector<string>{"Select From My Inbox", "Enter Question Id "});
    int choice;
    int qid;
    cout << "\nYour Choice : ";
    choice = inputDigit(1, 2);
    string answer;
    shared_ptr<Question> question;
    system("cls");
    if (choice == 1)
    {
        vector<string> question_list;
        for (int i{0}; i < questions.size(); i++)
        {
            question_list.push_back("Asker : " + ((questions[i]->getAnonymousity()) ? "( Anonymous )" : questions[i]->getAsker()) + "   " + "Question : " + questions[i]->getQuestion());
        }
        displayMenu(question_list);
        cout << "\nChoice ( exit with 0 ): ";
        int selected = inputDigit(0, question_list.size());
        if (selected == 0)
        {
            system("cls");
            return;
        }
        question = questions[selected - 1];
        qid = question->getId();
    }
    else
    {
        cout << "Enter Question ID : ";
        cin >> qid;
        question = db.getQuestionById(qid);
        if (!question || (question && question->getAskedTo() != currentUsername))
        {
            system("cls");
            cout << "\nThis Question Is Not Available !";
            return;
        }
    }
    system("cls");
    cout << "Your Answer : ";
    getline(cin, answer);
    db.addAnswer(qid, answer);

    system("cls");
    cout << "Operation Done \n";
    cout << "Question : " << question->getQuestion();
    cout << "\nRecent Answer : " << answer;
    cout << "\n\n";
}
void System::askQuestion()
{
    system("cls");
    displayMenu(vector<string>{"Select users from table", "Enter users names"});
    vector<string> selected_users;
    string question = "";
    cout << "\nYour Choice : ";
    int choice = inputDigit(1, 2);

    if (choice == 1)
    {
        system("cls");
        vector<string> usersnames;
        vector<shared_ptr<Account>> accounts = db.getAllAccount();
        int a_size = accounts.size();
        for (int i{0}; i < a_size; i++)
        {
            if (accounts[i]->getUsername() != currentUsername)
                usersnames.push_back(accounts[i]->getUsername());
        }
        displayMenu(usersnames);
        cout << "\nEnter number of each user (input end with 0) : ";

        int selected = -1;
        while (1)
        {
            selected = inputDigit(0, a_size);
            if (selected == 0)
                break;
            selected_users.push_back(usersnames[selected - 1]);
        }
    }
    else
    {
        system("cls");
        cout << "Enter Usersnames (input end with 0) : ";
        string input = "";
        while (input != "0")
        {
            input = inputString(30);
            if (!db.isUserExist(input))
            {
                cout << "\nUser ( " << input << " ) is not exist try again ! \n";
            }
            else if (input == currentUsername)
            {
                cout << "\nUser ( " << input << " ) is not available right now ! \n";
            }
            selected_users.push_back(input);
        }
    }

    system("cls");
    if (selected_users.size() == 0)
    {
        return;
    }
    displayMenu(vector<string>{"Public", "Anonymous"});
    cout << "\nYour Choice : ";
    int privacy = inputDigit(1, 2);
    privacy -= 1;
    cout << "Your Question : ";
    getline(cin, question);
    for (int i{0}; i < selected_users.size(); i++)
    {
        db.addQuestion(currentUsername, selected_users[i], question, privacy);
    }
    system("cls");
    cout << "Users : ";
    for (int i{0}; i < selected_users.size(); i++)
    {
        cout << selected_users[i] << " ";
    }
    cout << "\nPrivacy : " << ((privacy) ? "Anonymous" : "Public")
         << "\n";
    cout << "Question : " << question << "\n\n";
}
void System::questionFromMe() const
{
    system("cls");
    vector<pair<shared_ptr<Question>, vector<shared_ptr<Answer>>>> inbox = db.getQuestionsAskedByUser(currentUsername);
    if (inbox.size() == 0)
    {
        cout << "You don't have any questions\n";
        return;
    }
    cout << setiosflags(ios::left) << setw(7) << "ID " << setw(13) << "Date " << setw(15) << "AskedBy " << setw(11) << "AskedTo " << setw(40) << "Question " << endl;
    cout << endl
         << "==========================================================\n\n";

    for (auto &q : inbox)
    {

        cout << *(q.first) << endl
             << endl;
        if (q.second.size() == 0)
        {
            cout << "Not Answered yet.\n";
        }
        else
        {
            cout << "Answers :\n\n";
            for (auto &a : q.second)
            {
                cout << *a << endl;
            }
        }
        cout << endl
             << "==========================================================\n\n";
    }
    cout << endl
         << endl;
}
void System::viewUserInbox() const
{
    system("cls");
    vector<pair<shared_ptr<Question>, vector<shared_ptr<Answer>>>> inbox = db.getQuestionsAskedToUser(currentUsername);
    if (inbox.size() == 0)
    {
        cout << "You don't have any questions\n";
        return;
    }
    cout << setiosflags(ios::left) << setw(7) << "ID " << setw(13) << "Date " << setw(15) << "AskedBy " << setw(11) << "AskedTo " << setw(40) << "Question " << endl;
    cout << endl
         << "==========================================================\n\n";

    for (auto &q : inbox)
    {

        cout << *(q.first) << endl
             << endl;
        if (q.second.size() == 0)
        {
            cout << "Not Answered yet.\n";
        }
        else
        {
            cout << "Answers :\n\n";
            for (auto &a : q.second)
            {
                cout << *a << endl;
            }
        }
        cout << endl
             << "==========================================================\n\n";
    }
    cout << endl
         << endl;
}
void System::displayMenu(const vector<string> &menu) const
{
    for (int i = 0; i < (int)menu.size(); i++)
    {
        cout << i + 1 << "- " << menu[i] << endl;
    }
}
int System::inputDigit(int low, int high) const
{
    int number;
    while (1)
    {
        string input;
        bool valid = 1;
        getline(cin, input);
        if (input.size() == 0)
            continue;
        if (input.size() > 9)
        {
            valid = 0;
        }
        for (size_t i = 0; i < input.size() && valid; i++)
        {
            if (isdigit(input[i]) == false)
            {
                valid = 0;
                break;
            }
        }
        if (valid)
        {

            number = stoi(input);
            if (number < low || number > high)
                valid = 0;
        }
        if (valid)
        {
            break;
        }
        else
        {
            cout << "\nPlease input number between " << low << "-" << high << " : ";
        }
    }
    return number;
}
string System::inputString(int size) const
{
    string input;
    while (1)
    {
        bool valid = 1;
        getline(cin, input);
        if (input.size() == 0)
        {
            continue;
        }
        else if (input.size() > size)
        {
            valid = 0;
        }
        for (int i = 0; i < input.size() && valid; i++)
        {
            if (input[i] == ' ')
            {
                valid = 0;
                break;
            }
        }
        if (valid)
        {
            return input;
        }
        else
        {
            cout << "\ninvalid input please input data without spaces and with size equal or less than " << size << " !\n";
        }
    }
}