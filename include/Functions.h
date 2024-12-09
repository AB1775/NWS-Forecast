#pragma once

#include <sstream>
#include <fstream>
#include <iomanip>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include "Classes.h"

/**************************************************************************
 * @brief Wraps a given text into lines of specified maximum length.
 *
 * This function takes a string of text and splits it into multiple lines,
 * ensuring that each line does not exceed the specified maximum length.
 * Words are not split; instead, they are moved to the next line if they
 * do not fit within the current line.
 *
 * @param text The input string to be wrapped.
 * @param lineLength The maximum length of each line.
 * @return A vector of strings, where each string is a line of wrapped text.
 ***************************************************************************/
std::vector<std::string> wordWrap(const std::string& text, size_t lineLength) {
    std::istringstream words(text);
    std::vector<std::string> wrappedLines;
    std::string word;
    std::string line;

    while (words >> word) {
        if (line.length() + word.length() + 1 > lineLength) {
            wrappedLines.push_back(line);
            line = word;
        } else {
            if (!line.empty()) {
                line += " ";
            }
            line += word;
        }
    }
    if (!line.empty()) {
        wrappedLines.push_back(line);
    }

    return wrappedLines;
}

/**************************************************************************
 * @brief Removes the leading and trailing double quotes from a given string.
 *
 * This function checks if the input string starts and ends with double quotes.
 * If it does, it returns a new string with the leading and trailing quotes removed.
 * If the input string does not start and end with double quotes, it returns the original string.
 *
 * @param quotedString The input string that may contain leading and trailing double quotes.
 * @return A new string with the leading and trailing double quotes removed, or the original string if no quotes are found.
 ***************************************************************************/
std::string removeQuotes(const std::string& quotedString)
{
    if (!quotedString.empty() && quotedString.front() == '"' && quotedString.back() == '"')
    {
        return quotedString.substr(1, quotedString.size() - 2);
    }
    return quotedString;
}
