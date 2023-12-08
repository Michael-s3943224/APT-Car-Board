#include "board.h"
#include "game.h"

using std::vector;

const vector<vector<Cell>> Board::BOARD_EMPTY = 
{
    { EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY },
    { EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY },
    { EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY },
    { EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY },
    { EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY },
    { EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY },
    { EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY },
    { EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY },
    { EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY },
    { EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY }
};

const vector<vector<Cell>> Board::BOARD_1 =
{
    { BLOCKED, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY },
    { EMPTY, BLOCKED, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, BLOCKED, EMPTY, EMPTY },
    { EMPTY, EMPTY, BLOCKED, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY },
    { EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, BLOCKED, EMPTY, EMPTY },
    { EMPTY, EMPTY, EMPTY, EMPTY, BLOCKED, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY },
    { EMPTY, EMPTY, BLOCKED, EMPTY, EMPTY, BLOCKED, EMPTY, BLOCKED, EMPTY, EMPTY },
    { EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, BLOCKED, EMPTY, EMPTY, EMPTY },
    { EMPTY, EMPTY, BLOCKED, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY },
    { EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY },
    { EMPTY, EMPTY, BLOCKED, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, BLOCKED }
};

const vector<vector<Cell>> Board::BOARD_2 =
{
    { BLOCKED, BLOCKED, BLOCKED, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY },
    { EMPTY, BLOCKED, BLOCKED, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY },
    { EMPTY, BLOCKED, BLOCKED, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY },
    { EMPTY, EMPTY, EMPTY, EMPTY, BLOCKED, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY },
    { EMPTY, EMPTY, EMPTY, EMPTY, BLOCKED, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY },
    { EMPTY, BLOCKED, BLOCKED, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY },
    { EMPTY, EMPTY, EMPTY, EMPTY, BLOCKED, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY },
    { EMPTY, BLOCKED, BLOCKED, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY },
    { EMPTY, BLOCKED, BLOCKED, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY },
    { EMPTY, BLOCKED, BLOCKED, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY }
};

Board::Board()
{
    board = new std::vector<std::vector<Cell>>(BOARD_EMPTY);
}

Board::~Board()
{
    delete board;
}

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

PlayerMove Board::movePlayerForward(Player* player)
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

void Board::display(Player* player)
{
    //print out the top numbered columns of the board
    std::cout << LINE_OUTPUT << EMPTY_OUTPUT << LINE_OUTPUT;
    for (int i = 0; i < DEFAULT_BOARD_DIMENSION; ++i)
    {
        std::cout << i << LINE_OUTPUT;
    }
    std::cout << std::endl;

    //print out the rest of board (including the numbered rows)
    for (int y = 0; y < DEFAULT_BOARD_DIMENSION; ++y)
    {
        //print name of row
        std::cout << LINE_OUTPUT << y << LINE_OUTPUT;

        //print the cells
        for (int x = 0; x < DEFAULT_BOARD_DIMENSION; ++x)
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

bool Board::isValidBoardId(int id)
{
    return MIN_BOARD_ID <= id && id <= MAX_BOARD_ID;
}

bool Board::isValidPosition(const Position& pos)
{
    return pos.x >= 0 && pos.x < DEFAULT_BOARD_DIMENSION && 
        pos.y >= 0 && pos.y < DEFAULT_BOARD_DIMENSION;
}

