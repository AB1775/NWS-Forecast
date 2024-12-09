#pragma once

#include <iostream>

/**************************************************************************
 * @brief Class representing a grid point with latitude and longitude coordinates.
 **************************************************************************/

class GridPoints
{
public:
    double latitude;
    double longitude;
};

/**************************************************************************
 * @brief Class representing a location entity
 **************************************************************************/
class Location
{
public:
    std::string zipCode;
    std::string state_abbreviation;
    std::string latitude;
    std::string longitude;
    std::string city;
    std::string state;
};

/**************************************************************************
 * @brief Class representing a single forecast period with temperature, wind, and forecast details
 **************************************************************************/
class ForecastPeriod
{
    public:
        std::string name;
        int temperature;
        std::string temperatureUnit;
        std::string windSpeed;
        std::string windDirection;
        std::string shortForecast;
        std::string detailedForecast;
};

/**************************************************************************
 * @brief Class representing a forecast with multiple periods.
 **************************************************************************/
class Forecast
{
    public:
        std::vector<ForecastPeriod> periods;
};
