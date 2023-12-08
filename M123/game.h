#ifndef GAME_H
#define GAME_H

#include "helper.h"
#include "board.h"
#include "player.h"

// strings used for input validation
#define COMMAND_LOAD "load"
#define COMMAND_INIT "init"
#define COMMAND_FORWARD "forward"
#define COMMAND_FORWARD_SHORTCUT "f"
#define COMMAND_TURN_LEFT "turn_left"
#define COMMAND_TURN_LEFT_SHORTCUT "l"
#define COMMAND_TURN_RIGHT "turn_right"
#define COMMAND_TURN_RIGHT_SHORTCUT "r"
#define COMMAND_QUIT "quit"
#define COMMAND_GENERATE_RANDOM "generate"

// strings for directions
#define DIRECTION_NORTH "north"
#define DIRECTION_EAST "east"
#define DIRECTION_SOUTH "south"
#define DIRECTION_WEST "west"

// the character limit for the player's command
#define COMMAND_SIZE_LIMIT 100

// prompt for command input
#define COMMAND_PROMPT "Enter command: "

// the different phases of the game
enum GamePhase
{
    PHASE_NOT_STARTED,
    PHASE_LOAD,
    PHASE_INIT,
    PHASE_PLAY,
    PHASE_FINISHED
};

/*
 * I want a class that stores the command and its list of parameters
 * This allows me to return both of these values from a function
 */
class CommandInfo
{
    private:
        std::string name;
        std::vector<std::string> params;

    public:
        CommandInfo();
        CommandInfo(const std::string& name, const std::vector<std::string>& params);
        std::string getCommand() const;
        std::vector<std::string> getParams() const;
};

class Game
{
private:

    Board* board;
    Player* player;

    /*
     * Added this to store the current phase of the game
     * Mainly used to decide whether to print the number of moves made when quitting
     */
    GamePhase gamePhase;

    /**
     * Load the board. 
     * Return true if the board is loaded and false for quiting the game.
     */
    bool loadBoard();
    
    /**
     * Initialize the player. 
     * Return true if the player is initilized and false for quiting the game.
     */
    bool initializePlayer();

    /**
     * Play the game and move the play[er interactively.
     */
    void play();

    static StatusResponseValue<CommandInfo> readCommand(const std::string& prompt);

    /*
     * Methods to handle each of the possible commands
     * This includes validation of the list of parameters as strings
     * and executing the effects of each command
     */
    StatusResponse handleLoadCommand(const std::vector<std::string>& params);
    StatusResponse handleInitCommand(const std::vector<std::string>& params);
    StatusResponse handleForwardCommand(const std::vector<std::string>& params);
    StatusResponse handleTurnLeftCommand(const std::vector<std::string>& params);
    StatusResponse handleTurnRightCommand(const std::vector<std::string>& params);
    StatusResponse handleQuitCommand(const std::vector<std::string>& params);

    /*
     * Check if a direction is DIRECTION_NORTH, DIRECTION_EAST, DIRECTION_WEST or DIRECTION_SOUTH
     * The string must be in lowercase and must not contrain leading or trailing spaces
     */
    static bool isValidDirectionString(const std::string& direction);

    /*
     * Convert a string to the Direction enum type
     * The string must be in lowercase and must not contain leading or trailing spaces
     * If a Direction cannot be found, then NORTH is returned
     */
    static Direction stringToDirection(const std::string& directionStr);

    /*
     * Prints out the info of individual commands
     */
    static void showCmdInfoLoad();
    static void showCmdInfoInit();
    static void showCmdInfoMovement();
    static void showCmdInfoQuit();

    /*
     * Calls a combination of the print methods above
     * Prints out the list of commands for each phase of the game
     */
    static void showCmdInfoAll();
    static void showCmdInfoLoadBoard();
    static void showCmdInfoPlayerInit();
    static void showCmdInfoPlay();

public:

    // initialize and destroy the member variables
    Game();
    ~Game();

    /**
     * Main menu option 1 - play the game as per the specification.
     *
     * This method should call other methods in this class in the following order:
     * - loadBoard()
     * - initilizePlayer()
     * - play() 
     * 
     * These methods should in turn make all the calls to methods in board & player 
     * objects and handle interaction with the user (reading inputs from the console,
     * error checking, etc...).
     */
    void start();
};


#endif