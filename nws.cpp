#include "include/Functions.h"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>

void mainMenu()
{
    std::cout << "Enter Zipcode (or 4 to Exit): ";
}

int main()
{    
    std::string zipsFile = "zips.csv";
    std::ifstream file(zipsFile);

    std::string zipCode;

    checkFileOpen(file);

    while (true)
    {
        mainMenu();
        std::cin >> zipCode;

        if (zipCode == "4")
        {
            std::cout << "[+] Exiting Program..." << std::endl;
        }
        else
        {
            Location myLocation;
            Forecast forecast;

            if (fileSearch(file, zipCode, myLocation))
            {
                std::string apiURL = "https://api.weather.gov/points/" + myLocation.latitude + "," + myLocation.longitude;
                std::string response_body;
                
                std::string forecastURL = getForecastURL(apiURL, response_body);
                genericAPICaller(forecastURL, response_body);
                std::cout << forecastURL << std::endl;

                populateForecasts(forecastURL, forecast);

                std::cout << "\n[Forecast for " << myLocation.city << ", " << myLocation.state_abbreviation << "]" << std::endl;

                for (const auto& period : forecast.periods) 
                {
                    std::cout << "==========================================" << std::endl;
                    std::cout << "              " << std::setw(15) << std::left << period.name << std::endl;
                    std::cout << "==========================================" << std::endl;
                    std::cout << "[Temp] " << period.temperature << " " << period.temperatureUnit << std::endl;
                    std::cout << "[Wind] " << period.windSpeed << " " << period.windDirection << std::endl;

                    std::cout << "------------------------------------------" << std::endl;
                    
                    // Word-wrap the detailed forecast
                    size_t lineLength = 42;
                    std::vector<std::string> wrappedDescription = wordWrap(period.detailedForecast, lineLength);
                    for (const auto& line : wrappedDescription) {
                        std::cout << line << std::endl;
                    }
                    std::cout << "==========================================" << std::endl;

                    std::cout << "\n";
                }
            }
            std::cout << "[+] Exiting Program..." << std::endl;
            return false;
        }
    }
}
