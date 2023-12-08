go to M123/readme.txt for summary of M123

Things worth noting in my approach:
-To generate the board the following series of steps were applied (in board.cpp):
    1. clear the board vector and append boardSize number of vectors with boardSize length
    2. set all elements in board vector to EMPTY
    3. Calculate the number of cells with formula: boardSize * boardSize, and number of cells to fill with formula: (number of cells) * probability
    4. if (number of cells to fill) = (number of cells) decrement number of cells to fill so there is at least one position the player can be placed
    5. create an array of integers from 0 to (number of cells - 1), Each integer represents a position on the board
    6. swap a random element of int array (between nth to the end) with the nth element from the front, 
    doing so (number of cells to fill) times while incrementing n after each iteration
    7. loop through the first (number of cells to fill) elements of the int array
    8. For each iteration, set board cell at x = element mod (board size), y = element // (board size) to BLOCKED
-To have the board display the entire column number as the column name the following series of steps were applied:
    1. calculate the number of digits of boardSize - 1 with the formula floor(log10(10) + 1)
    2. Print out the (number of digits) amount of rows used for the column names. With each row having the correct digit place for the column numbers.
    3. Before each row print out the correct row number with padding of zeros if its length doesn't match the number of digits
-tryParseFloat is successful if the string has only numeric characters and doesnt cause an exception on std::stof (in helper.cpp)
-handleGenerate method was added to handle the inputs for the generate command (in game.cpp)

Issues that were encountered were:
-Randomly generating a block for each inividual cell will result in a less or more than expected percentage of cells being blocked
-When using rand() and setting the seed with time(), the same outputs were observed 
-This could have been because the calls to get a random number were less than a second apart and time() gives us an output in seconds so the same seed was used?
-When using rand() and setting the seed with milliseconds using chrono, the outputs were in increasing order and I'm not too sure why
-Deciding whether to round down or round up to get the number of cells to fill with blocks (100 cells * 0.406 probability = 40 blocked cells or 41?) (I ended up rounding down)
-I was told that I'm not allowed to fill up the entire board if the player chooses a probability of 1, so I had to account for that even though it wasn't in the spec sbfjksgsghskhfjks
-I was also told that I cannot use the last digits of the column number as the column name, and I had to fit the entire number... even thought it wasn't in the spec shfushgkjshgjks

Improvements that could be made:
-If the player chooses to fill up more than half of the board, then generating the board will take longer because more swaps will be applied to the position array (N swaps for N number of cells to fill)
-Instead what I could do if more than half the board is to be filled, I could initialize the board all as blocked, do swaps to get empty positions to the front of the positions array
    then place down the front empty positions of the positions array on the board.
-Each time I'm generating a random number, I'm recreating random_device, mt19937 and distribution objects. I should probably fix this.