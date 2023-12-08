#ifndef HELPER_H
#define HELPER_H

#include <iostream>
#include <string>
#include <cassert>
#include <sstream>
#include <vector>
#include <iostream>
#include <string.h>
#include <random>

#include <algorithm>

// whitespace characters used for trim functions and stringRemoveInstancesOf
#define WHITESPACE " \n\r\t\f\v"

/*
 * I want a class to store succes, error message and possibily a value
 * This allows me to return all those values from a function
 */
class StatusResponse
{
    private:
        bool success;
        std::string errorMessage;

    public:
        bool isSuccess() const;
        std::string getErrorMessage() const;
        StatusResponse();
        StatusResponse(bool success, const std::string& errorMessage);
};

template <typename T>
class StatusResponseValue: public StatusResponse
{
    private:
        T value;

    public:
        T getValue() const;
        StatusResponseValue(bool succcess, const std::string& errorMessage, T value);

};

class Helper
{
private:
    Helper();

public:

    /**
     * @brief Check whether the given string is an integer or a float 
     * 
     * @param s The given string
     * @return true if the string is an integer or a float 
     * @return false if the string is neither an integer nor a float 
     */
    static bool isNumber(std::string s);

    /**
     * @brief Split the given string given a delimeter. For instance, given the string "1,2,3" and the delimeter ",", 
     *        This string will be splited into "1","2","3", and they will be put into the vector.  
     * 
     * @param s The given string
     * @param tokens The vector containing the results
     * @param delimeter the string based on which splitting is performed
     */
    static void splitString(std::string s, std::vector<std::string>& tokens, std::string delimeter);

    /**
     * @brief Read the user input (a line) from keyboard
     * 
     * @return the user input.
     */
    static std::string readInput();

    static void printInvalidInput();

    /*
     * Check if the string:
     * - Does not contain a decimal point
     * - Contains only numeric characters
     * - Does not throw an error from std::stoi
     */
    static StatusResponseValue<int> tryParseInt(const std::string& s);

    /*
     * Check if the string:
     * - Contains only numeric characters (including decimal points)
     * - Does not throw an error from std::stof 
     */
    static StatusResponseValue<float> tryParseFloat(const std::string& s);

    /*
     * Return a string with all its characters in lower case form
     * This is so commands such as "Load" or "LOAD" or even "lOaD" are valid
     */
    static std::string stringToLower(std::string str);

    /*
     * Trims whitespaces from the ends of the string
     * This is so commands such as "                                 LOAD 1                      " is valid
     */
    static std::string stringTrimLeft(const std::string& str);
    static std::string stringTrimRight(const std::string& str);
    static std::string stringTrim(const std::string& str);

    /*
     * Get a digit from a number
     * Used to display row and column names
     */
    static int getDigit(int num, int digit);

    /*
     * Get the digit count of a numnber
     */
    static int getDigitCount(int num);

    /*
     * Check if a float is between 0 and 1
     */
    static bool isValidProbability(float f);

    /*
     * Get a random integer between lower and upper from a uniform distribution
     */
    static int randInt(int lower, int upper);
    
};
#endif