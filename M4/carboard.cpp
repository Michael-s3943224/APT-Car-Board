// Please go to M4/readme.txt for code description
// I don't want to pollute my code file

#include "game.h"
#include "helper.h"

//student information
#define STUD_NAME   "Michael Xie"
#define STUD_ID     "3943224"
#define STUD_EMAIL  "s3943224@student.rmit.edu.au"

//max size of input string when selecting choice
#define CHOICE_MAX_SIZE 50 

//prompt for entering choice
#define CHOICE_PROMPT "Please enter your choice: "

//choice options
enum Choice
{
    CHOICE_PLAY = 1,
    CHOICE_INFO = 2,
    CHOICE_QUIT = 3
};

using std::string;

/*
The loop displaying the menu alongside accepting inputs from the player
*/
void menuLoop();

/*
Display the main menu along with the options the user can choose from
*/
void showMenu();

/*
Display the students' name, id and email
*/
void showStudentInformation(string name, string id, string email);

/**
 * Prompt the user to enter an integer:
 * 
 * Returns whether the operation was a success along side the integer the user entered
 * Operation fails if:
 * -The string entered exceeds CHOICE_MAX_SIZE
 * -The string contains non-numeric characters (after trimming the spaces)
 * -An error occurs when converting the string to an integer
 */
StatusResponseValue<int> promptUserInt(const std::string& prompt);

int main()
{
    menuLoop();
    return EXIT_SUCCESS;
}

void menuLoop()
{
    bool menuLoop = true;

    while(menuLoop)
    {
        showMenu();

        StatusResponseValue<int> choiceStatusResponse = promptUserInt(CHOICE_PROMPT);
        bool parseSuccess = choiceStatusResponse.isSuccess();
        int choice = choiceStatusResponse.getValue();

        if (std::cin.eof())
        {
            //please go away end of file
            menuLoop = false;
        }
        else if (parseSuccess)
        {
            if (choice == CHOICE_PLAY)
            {
                Game game;
                game.start();

                //if we reached end of file inside game, exit the menu loop
                menuLoop = !std::cin.eof();
            }
            else if (choice == CHOICE_INFO)
            {
                showStudentInformation(STUD_NAME, STUD_ID, STUD_EMAIL);
            }
            else if (choice == CHOICE_QUIT)
            {
                menuLoop = false;
            }
        }
    }
}

void showMenu()
{
    std::cout << "Welcome to Car Board" << std::endl;
    std::cout << "--------------------" << std::endl;
    std::cout << "1. Play game" << std::endl;
    std::cout << "2. Show student's information" << std::endl;
    std::cout << "3. Quit" << std::endl;
    std::cout << std::endl;
}

void showStudentInformation(string name, string id, string email){
    std::cout << "----------------------------------" << std::endl;
    std::cout << "Name: " << name << std::endl;
    std::cout << "No: " << id << std::endl;
    std::cout << "Email: " << email << std::endl;
    std::cout << "----------------------------------" << std::endl;
    std::cout << std::endl;
}

StatusResponseValue<int> promptUserInt(const std::string& prompt)
{
    bool success = false;
    //there will be no error message, this is just here to prevent magic numbers
    std::string errorMessage = "";
    int choice = 0;

    std::cout << prompt;

    std::string choiceStr = Helper::readInput();

    //the length check is done before trimming
    success = choiceStr.size() <= CHOICE_MAX_SIZE;

    if (success)
    {
        choiceStr = Helper::stringTrim(choiceStr);

        StatusResponseValue<int> statusResponse = Helper::tryParseInt(choiceStr);
        success = statusResponse.isSuccess();
        choice = statusResponse.getValue();
    }

    return StatusResponseValue<int>(success, errorMessage, choice);
}


