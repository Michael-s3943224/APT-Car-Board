#include "board.h"
#include "game.h"

using std::vector;

const vector<vector<Cell>> Board::BOARD_EMPTY =
    {
        {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
        {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
        {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
        {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
        {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
        {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
        {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
        {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
        {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
        {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}};

const vector<vector<Cell>> Board::BOARD_1 =
    {
        {BLOCKED, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
        {EMPTY, BLOCKED, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, BLOCKED, EMPTY, EMPTY},
        {EMPTY, EMPTY, BLOCKED, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
        {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, BLOCKED, EMPTY, EMPTY},
        {EMPTY, EMPTY, EMPTY, EMPTY, BLOCKED, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
        {EMPTY, EMPTY, BLOCKED, EMPTY, EMPTY, BLOCKED, EMPTY, BLOCKED, EMPTY, EMPTY},
        {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, BLOCKED, EMPTY, EMPTY, EMPTY},
        {EMPTY, EMPTY, BLOCKED, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
        {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
        {EMPTY, EMPTY, BLOCKED, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, BLOCKED}};

const vector<vector<Cell>> Board::BOARD_2 =
    {
        {BLOCKED, BLOCKED, BLOCKED, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
        {EMPTY, BLOCKED, BLOCKED, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
        {EMPTY, BLOCKED, BLOCKED, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
        {EMPTY, EMPTY, EMPTY, EMPTY, BLOCKED, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
        {EMPTY, EMPTY, EMPTY, EMPTY, BLOCKED, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
        {EMPTY, BLOCKED, BLOCKED, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
        {EMPTY, EMPTY, EMPTY, EMPTY, BLOCKED, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
        {EMPTY, BLOCKED, BLOCKED, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
        {EMPTY, BLOCKED, BLOCKED, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
        {EMPTY, BLOCKED, BLOCKED, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}};

Board::Board()
{
    board = new std::vector<std::vector<Cell>>(BOARD_EMPTY);
}

Board::~Board()
{
    delete board;
}

// this is no longer used anymore and I don't know if I'm allowed to remove it
void Board::load(int boardId)
{
    if (boardId == 1)
    {
        *board = BOARD_1;
    }
    else if (boardId == 2)
    {
        *board = BOARD_2;
    }
}

void Board::generate(int boardSize, float proability)
{
    // resize the board
    board->clear();
    for (int i = 0; i < boardSize; ++i)
    {
        std::vector<Cell> row(boardSize);
        board->push_back(row);
    }

    // initialize the board as empty
    for (int y = 0; y < boardSize; ++y)
    {
        for (int x = 0; x < boardSize; ++x)
        {
            (*board)[y][x] = EMPTY;
        }
    }

    // initialize a list of all possible positions
    int numberOfCells = boardSize * boardSize;
    int positionIndices[numberOfCells];
    for (int i = 0; i < numberOfCells; ++i)
    {
        positionIndices[i] = i;
    }

    int cellsToFill = numberOfCells * proability;
    // make sure at least one cell is empty even if probability is 1
    if (cellsToFill == numberOfCells)
    {
        cellsToFill = cellsToFill - 1;
    }

    // select random positions and swap with the ith elements from the front of position array
    for (int i = 0; i < cellsToFill; ++i)
    {
        int randIndex = Helper::randInt(i, numberOfCells - 1);
        int temp = positionIndices[i];
        positionIndices[i] = positionIndices[randIndex];
        positionIndices[randIndex] = temp;
    }

    // access the front elements of the array which are random and set that position as blocked
    for (int i = 0; i < cellsToFill; ++i)
    {
        int y = positionIndices[i] / boardSize;
        int x = positionIndices[i] % boardSize;

        (*board)[y][x] = BLOCKED;
    }
}

bool Board::placePlayer(Position position)
{
    //a valid position is if the position exists on the board and is not a blocked cell
    bool validPosition = isValidPosition(position) && ((*board)[position.y][position.x] != BLOCKED);

    if (validPosition)
    {
        (*board)[position.y][position.x] = PLAYER;
    }

    return validPosition;
}

PlayerMove Board::movePlayerForward(Player *player)
{
    PlayerMove moveState = PLAYER_MOVED;
    Position forwardPos = player->getNextForwardPosition();
    Position currPos = player->position;

    if (!Board::isValidPosition(forwardPos))
    {
        moveState = OUTSIDE_BOUNDS;
    }
    else if ((*board)[forwardPos.y][forwardPos.x] == BLOCKED)
    {
        moveState = CELL_BLOCKED;
    }
    else
    {
        //set previous position to empty to cell to make sure theres only one player cell
        (*board)[currPos.y][currPos.x] = EMPTY; 
        (*board)[forwardPos.y][forwardPos.x] = PLAYER;
        player->updatePosition(forwardPos);
        moveState = PLAYER_MOVED;
    }

    return moveState;
}

void Board::display(Player *player)
{
    int boardSize = board->size();

    //this will help us properly allign the header rows and columns
    int numDigits = Helper::getDigitCount(boardSize - 1);

    //print out the top numbered columns of the board
    for (int d = 0; d < numDigits; ++d)
    {
        std::cout << LINE_OUTPUT;
        for (int j = 0; j < numDigits; ++j)
        {
            std::cout << EMPTY_OUTPUT;
        }
        std::cout << LINE_OUTPUT;

        for (int i = 0; i < boardSize; ++i)
        {
            int currDigit = Helper::getDigit(i, numDigits - d);
            std::cout << currDigit << LINE_OUTPUT;
        }
        std::cout << std::endl;
    }

    //print out the rest of board (including the numbered rows)
    for (int y = 0; y < boardSize; ++y)
    {
        //print name of row
        std::cout << LINE_OUTPUT;
        for (int d = 0; d < numDigits; ++d)
        {
            int currDigit = Helper::getDigit(y, numDigits - d);
            std::cout << currDigit;
        }
        std::cout << LINE_OUTPUT;

        //print the cells
        for (int x = 0; x < boardSize; ++x)
        {
            Cell cellId = (*board)[y][x];
            std::cout << cellIdToString(cellId, *player) << LINE_OUTPUT;
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

std::string Board::cellIdToString(Cell cellId, const Player& player)
{
    std::string output = ERROR_OUTPUT;

    if (cellId == EMPTY)
    {
        output = EMPTY_OUTPUT;
    }
    else if (cellId == BLOCKED)
    {
        output = BLOCKED_OUTPUT;
    }
    else if (cellId == PLAYER)
    {
        output = player.directionToArrowString();
    }

    return output;
}

bool Board::isValidPosition(const Position &pos)
{
    int boardSize = board->size();
    return pos.x >= 0 && pos.x < boardSize && pos.y >= 0 && pos.y < boardSize;
}

bool Board::isValidBoardSize(int size)
{
    return MIN_BOARD_SIZE <= size && size <= MAX_BOARD_SIZE;
}
