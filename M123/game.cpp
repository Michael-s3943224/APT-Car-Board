#include "game.h"

 //explicit template initialisation doesn't work without empty constructor?
CommandInfo::CommandInfo(): name(""), params(std::vector<std::string>()) {}

CommandInfo::CommandInfo(const std::string& name, const std::vector<std::string>& params): 
    name(name), params(params) {}

std::string CommandInfo::getCommand() const
{
    return name;
}

std::vector<std::string> CommandInfo::getParams() const
{
    return params;
}

Game::Game()
{
    board = new Board();
    player = new Player();
    gamePhase = PHASE_NOT_STARTED;
}

Game::~Game()
{
    delete board;
    delete player;
}

void Game::start()
{
    //display all the commands and the board
    showCmdInfoAll();
    board->display(player);

    //enter load phase
    gamePhase = PHASE_LOAD;
    bool loadedBoard = loadBoard();

    if (loadedBoard)
    {
        //enter initialisation phase
        gamePhase = PHASE_INIT;
        bool initializedPlayer = initializePlayer();

        if (initializedPlayer)
        {
            //enter play phase
            gamePhase = PHASE_PLAY;
            play();
        }
    }

    //if we've reached this point, then the game has ended
    gamePhase = PHASE_FINISHED;
}

bool Game::loadBoard()
{
    showCmdInfoLoadBoard();

    bool quit = false;
    bool isValidCommand = false;

    /*
     * The condition for exiting this phase is to either:
     * - Enter a valid command
     * - Quit (in that case false is returned)
     */
    while (!isValidCommand && !quit)
    {
        StatusResponseValue<CommandInfo> commandReadResponse = readCommand(COMMAND_PROMPT);
        bool commandReadSuccess = commandReadResponse.isSuccess();

        CommandInfo comandInfo = commandReadResponse.getValue();
        std::string command = comandInfo.getCommand();
        std::vector<std::string> params = comandInfo.getParams();

        if (std::cin.eof())
        {
            //may i kindly tell you to leave EOF char
            quit = true;
        }
        else
        {
            StatusResponse handleCommandResponse; 

            if (commandReadSuccess)
            {
                if (command == COMMAND_LOAD)
                {
                    handleCommandResponse = handleLoadCommand(params);
                }
                else if (command == COMMAND_QUIT)
                {
                    handleCommandResponse = handleQuitCommand(params);
                    quit = handleCommandResponse.isSuccess();
                }
            }

            isValidCommand = handleCommandResponse.isSuccess();
            std::string errorMessage = handleCommandResponse.getErrorMessage();
        
            if (!isValidCommand)
            {
                //if the error message is empty, output default error message else ouput custom one
                if (errorMessage.empty())
                {
                    Helper::printInvalidInput();
                }
                else
                {
                    std::cout << errorMessage << std::endl << std::endl;
                }

                showCmdInfoLoadBoard();
            }
            else if (!quit)
            {
                //only display the board if the quit command was not entered
                board->display(player);
            }
        }
    }

    return !quit;
}

bool Game::initializePlayer()
{
    showCmdInfoPlayerInit();

    bool initializedPlayer = false;
    bool quit = false;
    bool isValidCommand = false;

    /*
     * The condition for exiting this phase is to either:
     * - Initialise the player
     * - Quit (in that case false is returned)
     */
    while (!initializedPlayer && !quit)
    {
        StatusResponseValue<CommandInfo> commandReadResponse = readCommand(COMMAND_PROMPT);
        bool commandReadSuccess = commandReadResponse.isSuccess();

        CommandInfo comandInfo = commandReadResponse.getValue();
        std::string command = comandInfo.getCommand();
        std::vector<std::string> params = comandInfo.getParams();

        if (std::cin.eof())
        {
            //piss off EOF char!
            quit = true;
        }
        else
        {
            StatusResponse handleCommandResponse;

            if (commandReadSuccess)
            {
                if (command == COMMAND_LOAD)
                {
                    handleCommandResponse = handleLoadCommand(params);
                }
                else if (command == COMMAND_INIT)
                {
                    handleCommandResponse = handleInitCommand(params);
                    //if the init command was successful then we have initilised the player
                    initializedPlayer = handleCommandResponse.isSuccess();
                }
                else if (command == COMMAND_QUIT)
                {
                    handleCommandResponse = handleQuitCommand(params);
                    quit = handleCommandResponse.isSuccess();
                }
            }

            isValidCommand = handleCommandResponse.isSuccess();
            std::string errorMessage = handleCommandResponse.getErrorMessage();

            if (!isValidCommand)
            {
                //if the error message is empty, output default error message else ouput custom one
                if (errorMessage.empty())
                {
                    Helper::printInvalidInput();
                }
                else
                {
                    std::cout << errorMessage << std::endl << std::endl;
                }
                showCmdInfoPlayerInit();
            }
            else if (!quit)
            {
                //only display the board if the quit command was not entered
                board->display(player);
            }
        }
    }

    return initializedPlayer;
}

void Game::play()
{
    showCmdInfoPlay();

    bool quit = false;
    bool isValidCommand = false;

    //the only condition for exiting this phase is to quit
    while (!quit)
    {
        StatusResponseValue<CommandInfo> commandReadResponse = readCommand(COMMAND_PROMPT);
        bool commandReadSuccess = commandReadResponse.isSuccess();

        CommandInfo comandInfo = commandReadResponse.getValue();
        std::string command = comandInfo.getCommand();
        std::vector<std::string> params = comandInfo.getParams();

        if (std::cin.eof())
        {
            //go away EOF char!
            quit = true;
        }
        else
        {
            StatusResponse handleCommandResponse;

            if (commandReadSuccess)
            {
                if (command == COMMAND_FORWARD || command == COMMAND_FORWARD_SHORTCUT)
                {
                    handleCommandResponse = handleForwardCommand(params);
                }
                else if (command == COMMAND_TURN_LEFT || command == COMMAND_TURN_LEFT_SHORTCUT)
                {
                    handleCommandResponse = handleTurnLeftCommand(params);
                }
                else if (command == COMMAND_TURN_RIGHT || command == COMMAND_TURN_RIGHT_SHORTCUT)
                {
                    handleCommandResponse = handleTurnRightCommand(params);
                }
                else if (command == COMMAND_QUIT)
                {
                    handleCommandResponse = handleQuitCommand(params);
                    quit = handleCommandResponse.isSuccess();
                }
            }

            isValidCommand = handleCommandResponse.isSuccess();
            std::string errorMessage = handleCommandResponse.getErrorMessage();

            if (!isValidCommand)
            {
                //if the error message is empty, output default error message else ouput custom one
                if (errorMessage.empty())
                {
                    Helper::printInvalidInput();
                }
                else
                {
                    std::cout << errorMessage << std::endl << std::endl;
                }
                showCmdInfoPlay();
            }
            else if (!quit)
            {
                //only display the board if the quit command was not entered
                board->display(player);
            }
        }
    }
}

StatusResponseValue<CommandInfo> Game::readCommand(const std::string& prompt)
{
    std::string errorMessage = "";

    std::string commandNamePart = "";
    std::string paramsPart = "";
    std::vector<std::string> tokens = std::vector<std::string>();

    std::cout << prompt;
    std::string command = Helper::readInput();

    //if the input is way too long, chances are that the player is trying to do something sus
    //(possibly a marker trying to blow up my program?)
    bool validSize = command.size() <= COMMAND_SIZE_LIMIT;

    if (validSize)
    {
        //trim and lower input string
        command = Helper::stringTrim(command);
        command = Helper::stringToLower(command);

        //split the string into parts at the first whitespace into "commandNamePart" and "paramsPart
        //if there is no whitespace, then the entire string is the command name and there are no parameters
        std::size_t firstSpaceIndex = command.find_first_of(WHITESPACE);
        if (firstSpaceIndex == std::string::npos)
        {
            commandNamePart = command;
        }
        else
        {
            commandNamePart = command.substr(0, firstSpaceIndex);
            paramsPart = command.substr(firstSpaceIndex + 1);
        }
        
        Helper::splitString(paramsPart, tokens, ",");
        for (unsigned i = 0; i < tokens.size(); ++i)
        {
            tokens[i] = Helper::stringTrim(tokens[i]);
        }
    }

    return StatusResponseValue<CommandInfo>(validSize, errorMessage, CommandInfo(commandNamePart, tokens));
}


//no longer need this anymore!
StatusResponse Game::handleLoadCommand(const std::vector<std::string>& params)
{
    unsigned int loadNumParam = 1;
    bool validCommand = false;
    std::string errorMessage = "";

    if (params.size() == loadNumParam)
    {
        //0: board num
        std::string boardNumStr = params[0];

        //parse the board id string
        StatusResponseValue<int> parseBoardNumResponse = Helper::tryParseInt(boardNumStr);
        bool parseBoardNumSuccess = parseBoardNumResponse.isSuccess();
        int boardId = parseBoardNumResponse.getValue();

        /*
         * command is successful if:
         * - board id string was parsed successfully
         * - board id represents a valid board
         */
        validCommand = parseBoardNumSuccess && Board::isValidBoardId(boardId);

        if (validCommand)
        {
            board->load(boardId);
        }
    }

    return StatusResponse(validCommand, errorMessage);
}

StatusResponse Game::handleInitCommand(const std::vector<std::string>& params)
{
    unsigned int initNumParam = 3; //hopefully this prevents "magic numbers?"
    bool validCommand = false;
    std::string errorMessage = "";

    if (params.size() == initNumParam)
    {
        //0: x, 1: y, 2: direction
        std::string xStr = params[0];
        std::string yStr = params[1];
        std::string direction = params[2];

        //parse the x string
        StatusResponseValue<int> xParseResponse = Helper::tryParseInt(xStr);
        bool xParseSuccess = xParseResponse.isSuccess();
        int x = xParseResponse.getValue();

        //parse the y string
        StatusResponseValue<int> yParseResponse = Helper::tryParseInt(yStr);
        bool yParseSuccess = yParseResponse.isSuccess();
        int y = yParseResponse.getValue();

        Position pos(x, y);

        /*
         * command is successful if:
         * - 3 params were passed in
         * - x string is parsed successfully
         * - y string is parsed successfully
         * - direction string is a valid direction string
         * - x and y reprensents a position that exists on the board
         * - the player was able to be placed
         */
        if (xParseSuccess && yParseSuccess && isValidDirectionString(direction) && Board::isValidPosition(pos))
        {
            bool placedPlayer = board->placePlayer(pos);
            if (!placedPlayer)
            {
                errorMessage = "Unable to place player at that position";
            }
            validCommand = placedPlayer;
        }

        if (validCommand)
        {
            player->initialisePlayer(&pos, stringToDirection(direction));
        }
    }

    return StatusResponse(validCommand, errorMessage);
}

StatusResponse Game::handleForwardCommand(const std::vector<std::string>& params)
{
    bool validCommand = false;
    std::string errorMessage = "";
    PlayerMove moveState = board->movePlayerForward(player);

    if (params.empty())
    {
        //set custom error messages
        if (moveState == CELL_BLOCKED)
        {
            errorMessage = "Error: cannot move forward because the road is blocked";
        }
        else if (moveState == OUTSIDE_BOUNDS)
        {
            errorMessage = "The car is at the edge of the board and cannot move further in that direction";
        }

        /*
         * command is successful if:
         * - no params were passed in
         * - movePlayerForward return PLAYER_MOVED
         */
        validCommand = moveState == PLAYER_MOVED;
        
        if (validCommand)
        {
            //increment player moves
            ++player->moves;
        }
    }

    return StatusResponse(validCommand, errorMessage);
}

StatusResponse Game::handleTurnLeftCommand(const std::vector<std::string>& params)
{
    //comand is successful if no params were passed in
    bool validCommand = params.empty();
    std::string errorMessage = "";
    
    if (validCommand)
    {
        player->turnDirection(TURN_LEFT);
    }

    return StatusResponse(validCommand, errorMessage);
}

StatusResponse Game::handleTurnRightCommand(const std::vector<std::string>& params)
{
    //comand is successful if no params were passed in
    bool validCommand = params.empty();
    std::string errorMessage = "";

    if (validCommand)
    {
        player->turnDirection(TURN_RIGHT);
    }
    
    return StatusResponse(validCommand, errorMessage);
}

StatusResponse Game::handleQuitCommand(const std::vector<std::string>& params)
{
    //command is successful if no params were passed in
    bool validCommand = params.empty();
    std::string errorMessage = "";

    if (validCommand)
    {
        //if the phase was play when the player quit, display the numnber of total moves
        if (gamePhase == PHASE_PLAY)
        {
            std::cout << "Total player moves: " << player->moves << std::endl << std::endl;
        }
    }

    return StatusResponse(validCommand, errorMessage);
}

bool Game::isValidDirectionString(const std::string& direction)
{
    return direction == DIRECTION_NORTH || direction == DIRECTION_WEST ||
           direction == DIRECTION_SOUTH || direction == DIRECTION_EAST;
}

Direction Game::stringToDirection(const std::string& directionStr)
{
    Direction direction = NORTH;

    if (directionStr == DIRECTION_EAST)
    {
        direction = EAST;
    }
    else if (directionStr == DIRECTION_SOUTH)
    {
        direction = SOUTH;
    }
    else if (directionStr == DIRECTION_WEST)
    {
        direction = WEST;
    }
    else
    {
        //the direction is defaulted to north if a valid direction cannot be found
        direction = NORTH;
    }

    return direction;
}

void Game::showCmdInfoLoad()
{
    std::cout << "load <g>" << std::endl;
    std::cout << "   g: number of the game board to load" << std::endl;
}

void Game::showCmdInfoInit()
{
    std::cout << "init <x>,<y>,<direction>" << std::endl;
    std::cout << "   x: horizontal position of the car on the board (between 0 & 9)" << std::endl;
    std::cout << "   y: vertical position of the car on the board (between 0 & 9)" << std::endl;
    std::cout << "   direction: direction of the car's movement (north, east, south, west)" << std::endl;
}

void Game::showCmdInfoMovement()
{
    std::cout << "forward (or f)" << std::endl;
    std::cout << "turn_left (or l)" << std::endl;
    std::cout << "turn_right (or r)" << std::endl;
}

void Game::showCmdInfoQuit()
{
    std::cout << "quit" << std::endl;
}

void Game::showCmdInfoAll()
{
    std::cout << "You can use the following commands to play the game:" << std::endl;
    showCmdInfoLoad();
    showCmdInfoInit();
    showCmdInfoMovement();
    showCmdInfoQuit();
    std::cout << std::endl;
}

void Game::showCmdInfoLoadBoard()
{
    std::cout << "At this stage of the program, only two commands are acceptable:" << std::endl;
    showCmdInfoLoad();
    showCmdInfoQuit();
    std::cout << std::endl;
}

void Game::showCmdInfoPlayerInit()
{
    std::cout << "At this stage of the program, only three commands are acceptable:" << std::endl;
    showCmdInfoLoad();
    showCmdInfoInit();
    showCmdInfoQuit();
    std::cout << std::endl;
}

void Game::showCmdInfoPlay()
{
    std::cout << "At this stage of the program, only four commands are acceptable:" << std::endl;
    showCmdInfoMovement();
    showCmdInfoQuit();
    std::cout << std::endl;
}
