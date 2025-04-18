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

/**************************************************************************
 * @brief Checks if a file is open and reads the header line if it is.
 * 
 * This function checks if the provided ifstream object is open. If the file is not open,
 * it prints an error message to the standard error stream. If the file is open, it reads
 * the first line (header line) from the file.
 * 
 * @param file Reference to an ifstream object representing the file to be checked.
 **************************************************************************/
void checkFileOpen(std::ifstream &file)
{
    if (!file.is_open())
    {
        std::cerr << "[!] Error: Unable to read Zips File." << std::endl;
    }
    else
    {
        std::string headerLine;
        getline(file, headerLine);
    }
}

/**************************************************************************
 * @brief Searches the zips file for the specified zip code.
 * 
 * This function reads each line from the provided file stream and searches for
 * the specified zip code. If the zip code is found, it populates the provided
 * Location object with the corresponding data and returns true. If the zip code
 * is not found, it returns false.
 * 
 * @param file Reference to an open ifstream object representing the zips file.
 * @param zipCode The zip code to search for.
 * @param location Reference to a Location object to populate with the found data.
 * @return True if the zip code was found, false otherwise.
 **************************************************************************/
bool fileSearch(std::ifstream &file, std::string &zipCode, Location &location)
{
    std::string line;

    auto readAndRemoveQuotes = [](std::istringstream &ss) {
        std::string value;
        std::getline(ss, value, ',');
        return removeQuotes(value);
    };

    while (std::getline(file, line))
    {
        std::istringstream ss(line);
        std::string value = readAndRemoveQuotes(ss);

        if (value == zipCode)
        {
            location.zipCode = value;
            location.state_abbreviation = readAndRemoveQuotes(ss);
            location.latitude = readAndRemoveQuotes(ss);
            location.longitude = readAndRemoveQuotes(ss);
            location.city = readAndRemoveQuotes(ss);
            location.state = readAndRemoveQuotes(ss);

            file.close();
            return true;
        }
    }
    file.close();
    return false;
}

/**************************************************************************
 * @brief Callback function for handling data received from a cURL request.
 * 
 * @param contents Pointer to the data received.
 * @param size Size of each data element.
 * @param nmemb Number of data elements.
 * @param response Pointer to the string where the data will be appended.
 * @return The total size of the data processed.
 **************************************************************************/
size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *response)
{
    size_t totalSize = size * nmemb;
    response->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

/**************************************************************************
 * @brief Makes a generic API call to the specified URL and stores the response in the response_body.
 * 
 * @param url The URL to make the API call to.
 * @param response_body The string where the response will be stored.
 *************************************************************************/
void genericAPICaller(const std::string &url, std::string &response_body)
{
    CURL* curl = curl_easy_init();
    if (curl)
    {
        auto cleanup = [&]() { curl_easy_cleanup(curl); };
        try
        {
            CURLcode res;
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_USERAGENT, "NWS-Forecast");
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_body);
            res = curl_easy_perform(curl);

            if (res != CURLE_OK)
            {
                std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
            }
        }
        catch (...)
        {
            cleanup();
            throw;
        }
        cleanup();
    }
}

/**************************************************************************
 * @brief Retrieves the forecast URL from a given API endpoint.
 *
 * This function calls a generic API using the provided URL and stores the response
 * in the response_body. It then parses the JSON response to extract the forecast URL.
 *
 * @param url The API endpoint URL to call.
 * @param response_body A reference to a string where the API response body will be stored.
 * @return A string containing the forecast URL if parsing is successful, otherwise an empty string.
 *
 * @throws nlohmann::json::parse_error if there is an error parsing the JSON response.
 **************************************************************************/
std::string getForecastURL(const std::string &url, std::string& response_body)
{
    genericAPICaller(url, response_body);
    try
    {
        nlohmann::json jsonResponse = nlohmann::json::parse(response_body);
        std::string forecastURL = jsonResponse["properties"]["forecast"];
        return forecastURL;
    }
    catch (const nlohmann::json::parse_error &e)
    {
        std::cerr << "JSON Parsing Error: " << e.what() << std::endl;
    }
    return "";
}

/**************************************************************************
 * @brief Populates the forecast data from a given URL.
 *
 * This function makes an API call to the specified forecast URL, parses the JSON response,
 * and populates the provided Forecast object with the forecast periods data. The number of
 * periods to populate can be limited by the maxPeriods parameter.
 *
 * @param forecastURL The URL to fetch the forecast data from.
 * @param forecast The Forecast object to populate with the forecast data.
 * @param maxPeriods The maximum number of forecast periods to populate.
 **************************************************************************/
void populateForecasts(const std::string& forecastURL, Forecast& forecast, int maxPeriods = 6)
{
    std::string response_body;
    genericAPICaller(forecastURL, response_body);

    try
    {
        nlohmann::json jsonResponse = nlohmann::json::parse(response_body);

        int count = 0;
        for (const auto& period : jsonResponse["properties"]["periods"])
        {
            if (count >= maxPeriods) break;
            ForecastPeriod forecastPeriod;

            forecastPeriod.name = period["name"];
            forecastPeriod.temperature = period["temperature"];
            forecastPeriod.temperatureUnit = period["temperatureUnit"];
            forecastPeriod.windSpeed = period["windSpeed"];
            forecastPeriod.windDirection = period["windDirection"];
            forecastPeriod.shortForecast = period["shortForecast"];
            forecastPeriod.detailedForecast = period["detailedForecast"];

            forecast.periods.push_back(forecastPeriod);
            ++count;
        }
    }
    catch (const nlohmann::json::parse_error &e)
    {
        std::cerr << "JSON Parsing Error: " << e.what() << std::endl;
    }
}
