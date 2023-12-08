#include "player.h"

Position::Position()
{
    x = 0;
    y = 0;
}

Position::Position(int x, int y)
{
    this->x = x;
    this->y = y;
}

Player::Player()
{
    position = Position();
    direction = NORTH;
    moves = 0;
}

void Player::initialisePlayer(Position* position, Direction direction)
{
    this->position = *position;
    this->direction = direction;
}

void Player::turnDirection(TurnDirection turnDirection)
{
    //i don't feel like doing nested if conditionals
    //currDir - 1 will get the left direction relative to currDir
    //curDir + 1 will get the right direction relative to currDir
    //if we go below 0 or to number of directions, we wrap around
    
    int turnOffset = 0;
    if (turnDirection == TURN_LEFT)
    {
        turnOffset = -1;
    }
    else if (turnDirection == TURN_RIGHT)
    {
        turnOffset = 1;
    }

    int newDirection = direction + turnOffset;
    
    //could replace this with newDirection %= DIRECTION_NUM
    //but probably too confusing
    if (newDirection < 0)
    {
        newDirection = DIRECTION_NUM - 1;
    }
    else if (newDirection >= DIRECTION_NUM)
    {
        newDirection = 0;
    }

    direction = static_cast<Direction>(newDirection);
}

Position Player::getNextForwardPosition()
{
    int x = position.x;
    int y = position.y;

    Position pos;

    if (direction == NORTH)
    {
        pos = Position(x, y - 1);
    }
    else if (direction == EAST)
    {
        pos = Position(x + 1, y);
    }
    else if (direction == SOUTH)
    {
        pos = Position(x, y + 1);
    }
    else if (direction == WEST)
    {
        pos = Position(x - 1, y);
    }
    else
    {
        pos = this->position;
    }

    return pos;
}

void Player::updatePosition(Position position)
{
    this->position = position;
}

void Player::displayDirection()
{
    std::cout << directionToArrowString();
}

std::string Player::directionToArrowString() const
{
    //if we cant find the direction, return DIRECTION_ARROW_OUTPUT_ERROR
    std::string output = DIRECTION_ARROW_OUTPUT_ERROR;
    
    if (direction == NORTH)
    {
        output = DIRECTION_ARROW_OUTPUT_NORTH;
    }
    else if (direction == EAST)
    {
        output = DIRECTION_ARROW_OUTPUT_EAST;
    }
    else if (direction == SOUTH)
    {
        output =  DIRECTION_ARROW_OUTPUT_SOUTH;
    }
    else if (direction == WEST)
    {
        output = DIRECTION_ARROW_OUTPUT_WEST;
    }

    return output;
}
