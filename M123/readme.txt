go to M4/readme.txt for summary of M4

Things worth noting in my approach:
-Each command is handled with their respective handle<name>Command function (in game.cpp)
-Created a BOARD_EMPTY constant to represent an empty board (in board.cpp)
-Turning the player left decrements direction and turning right increments direction (wraps around if <0 or >=4) (turnDirection in player.cpp)
-To parse the command, the following series of steps are applied (readCommand in game.cpp)
    1. Trim the string to remove leading and trailing whitespaces
    2. Convert the entire string to lowercase
    3. Split the string into command name and parameters parts at the first whitespace (if no whitespace, entire string is command name)
    4. Split the parameters string by commas and store it into a vector of strings
    5. Go through each element of the string vector and trim each element
-CommandInfo class is used to hold the command name and the vector of parameters (in game.cpp)
-StatusResponse and StatusResponseValue are classes used to tell us if an error occurred in a function (has success, error message and maybe a value field) (in helper.cpp)
-tryParseInt is successful if the string has no decimal point, has only numeric characters and std::stoi doesn't throw an error (probably from overflowing) (in helper.cpp)

Approaching M1:
-Main Menu:
    -Used a bunch of cout statements to print out the Menu
    -Trimmed any inputs of whitespace before attempting to parse into an integer
    -Used enums to represent the different options the player could choose from (play, info, quit)
    -The menu loop will continue while variable menuLoop is true
    -If an end of file character is encountered, menuLoop is set to false and the rest of the code is not executed to prevent an infinite loop
    -If the player choose to play, the game starts. Once it ends or is interrupted, the EOF character is checked.
    -If there was an EOF character, menuLoop is set to false to also prevent infinite loops
    -If the player chooses to display student info, it is displayed and the menu is redisplayed
    -If the player chooses to quit, menuLoop is set to false, the program terminates and the menu is not displayed
-Displaying the Board:
    -The header row is printed by looping through 0 to 9 and printing each of those digits
    -To print the rest of the board a nested for loop was used (rows as outer and columns as inner loops)
    -For each inner iteration, the row name is first printed, then the rest of the cells for that row

-Loading the Game Boards:
    -Phases are triggered in the body of the previous phases' if statement (that checks whether the phase exited successfully)
    -This way when a phase is exited unsuccessfully (false is returned), the flow of the program goes immediately to the bottom of Game::play
    -The game board is initialised as EMPTY_BOARD on the heap, a constant which has been defined (A board with all empty cells)
    -When the player chooses to load a board, the game board's set by first dereferencing it and then assigning the loading board's value to it
    -A range check is done on the supplied board id, whether its between 1 and 2
    -If the previous requirement was done properly, then the loaded boards should be shown correctly
-Initialise Game:
    -The position is checked whether it exists within the bounds of the board
    -Then position is checked whether a block does not exist at that position
    -The direction is checked whether it is a valid (north, east, south, west)
    -If all of these cases are satisfied, the player is placed on the board facing the specified direction
-Input Validation:
    -Main menu choice has a limit size of 50 characters and for Game commands, the limit is 100
    -Numeric values are checked if they are within a correct range, don't cause an overflow exception and whether they are integers
    -Direction strings are checked if they truly represent a valid direction (north, east, south, west)

Appraching M2:
-Play Game:
    -Turning left decrements direction and turning right increments it (explained above)
    -When the player moves forward, the position in front of the player (relative to its direction) is checked
    -The front position is checked whether it exists within the bounds of the board
    -Next the position is checked whether a block does not exist at that position
    -If these cases are met, the player moves forward relative to its direction
    -Else an error message is displayed (different depending on which case)
    -Since this validation was handled in seperate function, a class was needed to be defined to return both validation success status and the error message
    -Hence the StatusResponse class was defined (or if a value is needed to be returned, StatusResponseValue)

Approaching M3:
-Stopping at the Edges:
    -If the front position x and y is both between 0 and 9, then the player can move forward (if theres no block)
-Quit Main Menu:
    -^C terminates without an issues, as expected
    -^D in menu loop and in game will both set menuLoop to false, stopping the menu loop and allowing the program to exit successfully
    -quiting sets the menuLoop variable to false, stopping the menu loop which allows the program to successfully exit
Return to Main Menu:
    -The Game object is created and method Game::start() is called inside the menu loop
    -When the game ends, the flow of the program returns back to the body of the menu loop
    -This allows the player to return back to the main menu when the game ends


Issues that were encountered were: 
-End of file character putting the program in an infinite loop
-Decimal inputs can still be read as integers (there should be an error)
-High numeric values causing overflow exceptions, 
-Commands with extra spaces or even tabs (left, right ends or in between tokens) are getting read as invalid input
-Commands in uppercase not being considered as valid commands.
-Getting templates working for my StatusResponseValue<T> class which was used to "return more than one value" and avoid side effects
-Following the C++ style guide, especially only including single return stattements in functions, no magic numbers and no side effects
-Some of my code lines actually exceed 80 characters. If I lose marks for this then so be it. I really can't be bothered
-Sometimes got mixed up with accessing the cell for board. Sometimes I did (*board)[x][y] which is incorrect.
-Once forgot that as y increases, we go down the board, and when it decreases, we go up

Improvements that could be made: 
-loadBoard(), initializePlayer() and play() have a very similar structure and thus
there might be a way to move the similar code into another method to prevent repeating myself
-The commands could be stored in a dictionary where the key is the command name and the value is a function pointer
to their respective "handleCommand" function. This can help remove the chain of if statements checking if its a valid command name
(we also might need to check if the command exist in an array of commands available for the current phase of the game)
-I should be moving the classes I defined into a seperate file (Were we allowed to create extra files? Also scared you guys may forget to compile the extra files)
-If the flushing from std::endl is really causing a problem with performce, we can replace it with \n

Improvements to the starter code:
-Helper class:
    -Function splitString accepts a string vector (tokens) which is passed as a reference and modified inside. Isn't this an example of a side effect?
    -Furthermore, I think string arguments s and delimiter should passed in as constant references instead, as we don't want to make unecesary copies of strings (str_copy is being used to copy s anyways)
    -Function isNumber (in helper.cpp) uses breaks which goes against the C++ style guide
-Position class:
    -the x and y fields are public and can be directly accessed and modified. This goes against OOP principles
-Player class:
    -Fields position, direction and moves are public and can be directly accessed and modified. This goes against OOP principles
    -Method initialisePlayer accepts a pointer to Position (position), this should be a const reference, because now the method can modify this object
    -Method updatePosition accepts a Position object (position), this should be a const reference so we don't make unnecessary copies
-Board class:
    -Method placePlayer accepts a Position object (position), this should also be a const reference so we don't make unncessary copies
    -Method display accepts a pointer to Player (player), this should also be a const reference to ensure we dont modify the player inside the method
    -Method movePlayerForward accepts a pointer to Player (player) which we were instructed to use to modify the player. 
    -I think the Board class should not be modifying the player, it should be the job of the Game class who manages it.

    


