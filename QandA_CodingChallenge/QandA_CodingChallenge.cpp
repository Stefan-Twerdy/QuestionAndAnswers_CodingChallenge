// QandA_CodingChallenge.cpp : A simple programm for a coding challenge. The user should be able to ask the program a question and if the question is
// saved in the system the corresponding answer will be printed, if the question is not in the system a default answer will be printed.
// 
// Since it was not specified what form the "system" should take I decided a class "QuestionAndAnswers":
// With the variables "Question" as a string and a string vector Answers (vector<string>) so that each object of the class QuestionAndAnswers
// contains a question and its answers.
// It also contains the function "printAnswers()" that prints all Answers to the question.
// Since there was no requirement to restrict the access for the variables i decided to leave them public for the sake of simplicity instead of making get and set functions.
// 
// Since input/error handling was not specifically required I will do it selectivly and/or write a comment where it would make sense for input/error (if it was a real application and not a coding challenge).
// Since no requirement was made to the interface a console program was chosen
// 
// Restrictions:
// "A Question is a String with max 255 chars"
// "An Answer is a String with max 255 chars"
// "A Question can have multiple answers (like bullet points)"
// "Adding a question looks like:
//  <question> ? Åg<answer1>Åh Åg<answer2>Åh Åg<answerX>Åh "
// "Char Åg?Åh is the separator between question and answers"
// "Every Question needs to have at least one answer but can have unlimited answers all inside of char Åg
// 
// Each Answer can be 255 chars long therefore a new Q&A set could potentially look like:
// "<255chars>? "<255chars>" "<255chars>" "<255chars>" "<255chars>"
// therefore when parsing the question should be parsed into a string variable
// and the answers into a list of strings since there is an unspecified number
// The variables are not limited to a specific size in the class, the size check happens when before the class object gets added to the vector.
// 
//
// 
// In the comments question will be abreviated to "q" and answer to "a"
//

#include <iostream>
#include <vector>
#include <string>
#include <sys/stat.h>
using namespace std;

#define MAX_QUESTION_LEN 255 //Maximum len of a question
#define MAX_ANSWER_LEN 256 //Maximum len of a answer + \0

class QuestionAndAnswers {
public: //for simplicity in this coding challenge to skip the get & set functions
    string Question;
    vector<string> Answers;

    void printAnswers() {
        for (vector<string>::iterator Answ = begin(Answers); Answ != end(Answers); ++Answ) {
            cout << *Answ + "\n";
        }
    }
};

vector<QuestionAndAnswers> QandA; //for simplicity a global variable

const string sAnswerDelimiter = "\" \""; //input format is <question> ? Åg<answer1>Åh Åg<answer2>Åh Åg<answerX>Åh -> delimiter between answers is Åh Åg
const string defAnswer = "the answer to life, universe and everything is 42\n"; //default answer that will be printed if the asked question is not found within the system.

int selectionMenu();
void standardQuestion();
bool newQuestionAndAnswer(string sQuestion, string sAnswers);
bool checkSavedQuestions(string sQuestion, bool bPrintAnswers = true);

int main()
{
    int iselectedMenu = 0; // stores the return value from selectionMenu()
    int iPos = 0; // for substring to seperate inputs like question and answer
    string sInput; // temporarely stores the input for asking a question & inserting new Q&A
    string sQuestion = "";
    string sAnswers = "";
    string sTmp = "";
    bool bAnswerSaved = false;
    QuestionAndAnswers newQandA;

    standardQuestion();

    cout << "Hello do you want to ask a question or insert a new question and answer into the system?";
    while (true) { //loop will be escaped with break when the user types "exit" at the selection menu
        iselectedMenu = selectionMenu(); // will ask the user what branch of the program should be used and returns a number
        //"1" -> ask q; "2" -> input new Q&A; "3" -> close program; "4" -> start the loop again
        if (iselectedMenu == 0)
            break;

        if (iselectedMenu == 1) {
            cout << "\n Please ask a question with the following format: <question>?\n";

            //cin >> sInput; // only gets one word
            getline(cin >> ws, sInput);
            checkSavedQuestions(sInput);
        }

        if (iselectedMenu == 2) {
            cout << "\n Type a new question and answer(s) with the following format: <question>? \"<answer1>\" \"<answer2>\" \"<answerX>\"\n";
            //1 split q and a
            //2 check if q is already in the system 
            //3 call newQuestionAndAnswer to save the question and answers if at least one answer is valid

            //1:
            //cin >> sInput; // only gets one word
            getline(cin >> ws, sInput);
            iPos = sInput.find("?");
            if (iPos == -1) { //-1 -> invalid input as no "?" was written after the question
                cout << "\n invalid input the input has to following the format: <question>? \"<answer1>\" \"<answer2>\" \"<answerX>\"";
                continue;
            }
            sQuestion = sInput.substr(0, iPos);
            if (sQuestion.length() > MAX_QUESTION_LEN) { //question too long
                cout << "\n invalid input question is too long";
                continue;
            }
            sAnswers = sInput.substr(iPos + 2); // +2 since format is <q>? "<a1>" so that the "? " is cut out from the answers string


            //2:
            //if the question is already in the system print error
            if (checkSavedQuestions(sQuestion, false)) { //dont print answers to the questions only check if it is already in the system
                cout << "\n Question is already saved. Only new questions and answers can be saved.\n";
                continue;
            }
            
            //3:
            bAnswerSaved = newQuestionAndAnswer(sQuestion, sAnswers);
            if (!bAnswerSaved)
                cout << "\nNo valid Answers provided. Question and Answers will not be saved.";

        }

    }
    // clean up variables if necessary
}
/// <summary>
/// Gives the user a prompt to select which part of the program the user wants to use. Possible return values:
/// 1: User wants to ask a question
/// 2: User wants to insert a new question and answer into the system
/// 0: exit the program
/// </summary>
/// <returns>An Integer based on the selected menu option. 0 = exit the program</returns>
int selectionMenu() {
    string sInput = "";
    int iSelection = -1; //Selected Menu option that will be returned. -1 -> invalid input
    while (iSelection < 0)
    {
        cout << "\n \"1\" to ask a question\n \"2\" to input a new Q & A set\n \"exit\" to close the program\n";
        //read input: "1" -> ask q; "2" -> input new Q&A; "exit" -> close program; incorrect input -> print "incorrect input" and start the loop again
        //cin >> sInput; // only gets one word, might get weird behavior with this if user uses a space here
        getline(cin >> ws, sInput);
        if (sInput.compare("exit") == 0)
            return 0;

        try { //for string to int conversion
            iSelection = stoi(sInput); //noticed that if the string "1 2 exit" gets passed to stoi("1 2 exit") the result is 1. For this coding challenge there is no need to "fix" this unexpected behaviour.
            if (iSelection > 2) {
                iSelection = -1;
                cout << "\n invalid input\n";
            }
        }
        catch (...) {
            iSelection = -1;
            cout << "\n invalid input\n";
        }
    }

    return iSelection;
}

/// <summary>
/// Fills the QandA vector with data for testing purposes.
/// </summary>
void standardQuestion() {
    QuestionAndAnswers stdQandA;

    stdQandA.Question = "What is the colour of a ripe tomato";
    stdQandA.Answers.push_back("red");

    QandA.push_back(stdQandA);

    QuestionAndAnswers stdQandA1;

    stdQandA1.Question = "test";
    stdQandA1.Answers.push_back("test");

    QandA.push_back(stdQandA1);

    QuestionAndAnswers stdQandA2;

    stdQandA2.Question = "teest";
    stdQandA2.Answers.push_back("test1");
    stdQandA2.Answers.push_back("test2");

    QandA.push_back(stdQandA2);

    QuestionAndAnswers stdQandA3;

    stdQandA3.Question = "teeest";
    stdQandA3.Answers.push_back("test1");
    stdQandA3.Answers.push_back("test2");
    stdQandA3.Answers.push_back("test3");

    QandA.push_back(stdQandA3);
    return;
}

/// <summary>
/// Takes the split input from the user and checks if any of the answers are within the restrictions (255 chars + \0). 
/// If at least one Answer is valid the new Question and Answer(s) get saved to the QandA vector.
/// Validity of the question should be checked before using this function.
/// Validity of answers will be checked by this function.
/// </summary>
/// <param name="sQuestion">The question of the new set.</param>
/// <param name="sAnswers">A string with all answers in the format of Åg<answer1>Åh Åg<answer2>Åh Åg<answerX>Åh.</param>
/// <returns></returns>
bool newQuestionAndAnswer(string sQuestion, string sAnswers) {
    QuestionAndAnswers newQandA;
    newQandA.Question = sQuestion;

    //char sAnswerDelimiter[5] = "\" \""; //input format is <question> ? Åg<answer1>Åh Åg<answer2>Åh Åg<answerX>Åh -> delimiter between answers is Åh Åg
    int okAnswers = 0;
    vector<string> vsAnswers;
    int iPos = 0;
    string sTmp = "";
    bool bAnswerSaved = true; //gives the back the information if the q and a set was saved or not.

    //with this system there is no check if the answers follow the format "<answer1>" "<answer2>" "<answerX>"
    //so the splitting of the answers will not work properly if the user doesnt add the " every time.
    //Since for this coding challenge there was no requirement for error handling, i left it as it is.
    while (iPos < sAnswers.size()) {
        iPos = sAnswers.find(sAnswerDelimiter);
        vsAnswers.push_back(sAnswers.substr(0, iPos));
        sAnswers.erase(0, iPos + 3); // 3 is the length of the delimiter, " "
    }

    for (vector<string>::iterator Answ = begin(vsAnswers); Answ != end(vsAnswers); ++Answ) {
        sTmp = *Answ;
        sTmp.erase(remove(sTmp.begin(), sTmp.end(), '\"'), sTmp.end()); //remove " from string
        if (sTmp.length() < MAX_ANSWER_LEN) { //check if each answer is at most 255 chars long
            okAnswers++;
            newQandA.Answers.push_back(sTmp);
        } //else invalid input -> do nothing
    }

    if (okAnswers > 0) //at least 1 answer ok -> save the set
        QandA.push_back(newQandA);
    else
        bAnswerSaved = false;

    return bAnswerSaved;
}

/// <summary>
/// Checks if the given question is saved in the QandA vector.
/// If the question is found it will call printAnswers of the QuestionAndAnswers object.
/// </summary>
/// <param name="sQuestion"></param>
/// <param name="bPrintAnswers">should the answers to the questions be printed? default = true</param>
/// <returns></returns>
bool checkSavedQuestions(string sQuestion, bool bPrintAnswers) {
    int iPos = -1; // to check if there is a ? in the question string.
    string sTmp = "";
    bool bQuestionFound = false;

    if (bPrintAnswers) { //not required for false
        iPos = sQuestion.find("?"); //no ? -> no question was asked
        if (iPos == -1) {
            cout << "\n Invalid format. (format: <question>?)\n";
            return bQuestionFound;
        }
        sQuestion = sQuestion.substr(0, iPos); // the saved questions do not have the ? therefore it is removed
    }


    for (int i = 0; i < QandA.size(); i++) {
        sTmp = QandA[i].Question;
        //case sensitive so only look for exact match
        if (sQuestion.compare(sTmp) == 0) { //Question found -> print answer
            bQuestionFound = true;
            if (bPrintAnswers)
                QandA[i].printAnswers();
        }
    }
    if (!bQuestionFound && bPrintAnswers) //no question was found -> print default answer
        cout << defAnswer;
    return bQuestionFound;
}
