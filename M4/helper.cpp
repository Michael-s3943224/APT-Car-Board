#include "helper.h"
//this is so i can access ComandInfo to make an explicit template instantiation of CommandInfo
#include "game.h" 

using std::string;
using std::vector;

StatusResponse::StatusResponse(): success(false), errorMessage("") {}

StatusResponse::StatusResponse(bool success, const std::string& errorMessage): 
    success(success), errorMessage(errorMessage){}

bool StatusResponse::isSuccess() const
{
    return success;
}

std::string StatusResponse::getErrorMessage() const
{
    return errorMessage;
}

template <typename T>
StatusResponseValue<T>::StatusResponseValue(bool success, const std::string& errorMessage, T value): 
    StatusResponse(success, errorMessage), value(value){}

template <typename T>
inline T StatusResponseValue<T>::getValue() const
{
    return value;
}

//explicit template instantiations
template class StatusResponseValue<int>;
template class StatusResponseValue<float>;
template class StatusResponseValue<CommandInfo>;


Helper::Helper(){}

void Helper::printInvalidInput()
{
    std::cout << "Invalid input.\n" << std::endl;
}

bool Helper::isNumber(string s)
{
    string::const_iterator it = s.begin();
    char dot = '.';
    int nb_dots = 0;
    while (it != s.end()) 
    {
        if (*it == dot)
        {
            nb_dots++;
            if (nb_dots>1)
            {
                break;
            }
        }   
        else if (!isdigit(*it))
        {
            break;
        } 

        ++it;
    }
    return !s.empty() && s[0] != dot && it == s.end();
}

void Helper::splitString(string s, vector<string>& tokens, string delimeter)
{
    tokens.clear();
    char* _s = new char[s.length()+1];
    strcpy(_s, s.c_str());

    char * pch;
    pch = strtok (_s, delimeter.c_str());
    while (pch != NULL)
    {
        tokens.push_back(pch);
        pch = strtok (NULL, delimeter.c_str());
    }
    delete[] _s;
}

string Helper::readInput()
{
    string input;
    std::getline(std::cin, input);
    std::cout << std::endl;

    return input;
}

StatusResponseValue<int> Helper::tryParseInt(const std::string& str)
{
    int result = 0;
    bool success = true;
    char dot = '.';
    std::string errorMessage = "";

    try
    {
        //check if there is no decimal point and the string represents a number
        if (str.find(dot) == std::string::npos && isNumber(str))
        {
            result = std::stoi(str);
        }
        else
        {
            success = false;
        }
    }
    catch(const std::exception& e)
    {
        //overflow exception or some other random thing I don't expect
        success = false;
    }
    
    return StatusResponseValue<int>(success, errorMessage, result);
}

StatusResponseValue<float> Helper::tryParseFloat(const std::string& str)
{
    float result = 0;
    bool success = true;
    std::string errorMessage = "";

    try
    {
        //only check if the string represents a number
        if (isNumber(str))
        {
            result = std::stof(str);
        }
        else
        {
            success = false;
        }
    }
    catch(const std::exception& e)
    {
        //overflow exception or some other random thing I don't expect
        success = false;
    }
    
    return StatusResponseValue<float>(success, errorMessage, result);
}

std::string Helper::stringToLower(std::string str)
{
    //https://www.geeksforgeeks.org/conversion-whole-string-uppercase-lowercase-using-stl-c/
    //param1: start of iterator copying from
    //param2: end of iterator copying from
    //param3: start of iterator to place copy
    //param4: function for transformation
    transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

//Trim functions from: https://www.techiedelight.com/trim-string-cpp-remove-leading-trailing-spaces/
std::string Helper::stringTrimLeft(const std::string& str)
{
    //find the first index that is not a whitespace from left to right
    //we return the substring starting from that index to the end
    std::size_t startIndex = str.find_first_not_of(WHITESPACE);
    return (startIndex == std::string::npos) ? "" : str.substr(startIndex);

}

std::string Helper::stringTrimRight(const std::string& str)
{
    //find the first index that is not a whitespace from right to left
    //we return the substring from the start to that index
    std::size_t endIndex = str.find_last_not_of(WHITESPACE);
    return (endIndex == std::string::npos) ? "" : str.substr(0, endIndex + 1);
}

std::string Helper::stringTrim(const std::string& str)
{
    //apply both of the trims
    return stringTrimLeft(stringTrimRight(str));
}

int Helper::getDigit(int num, int digit)
{
    //keep dividing by 10 until we reach our desired digit
    int currDigit = 0;
    for (int i = 0; i < digit; ++i)
    {
        currDigit = num % 10;
        num /= 10;
    }

    return currDigit;
} 

int Helper::getDigitCount(int num)
{
    return static_cast<int>(log10(num) + 1);
}

bool Helper::isValidProbability(float f)
{
    //probabilities should be between 0 and 1
    return f >= 0 && f <= 1.0;
}

int Helper::randInt(int lower, int upper)
{
    //code from: https://en.cppreference.com/w/cpp/numeric/random/uniform_int_distribution
    std::random_device randomDevice;
    std::mt19937 generator(randomDevice());
    std::uniform_int_distribution<int> distribution(lower, upper);
    int output = distribution(generator);
    return output;
}