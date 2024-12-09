# NWS-Forecast
<img src="Images/Logo.png" width="800">
NWS-Forecast is a command-line utility that provides weather forecasts by leveraging the National Weather Service (NWS) API, libcurl, and Nlohmann JSON. 

## Features
- **Real-time Weather Data**: Fetches up-to-date weather information from the National Weather Service
- **Detailed Forecasts**: Provides detailed weather forecasts, including temperature, wind speed, wind direction, and descriptive forecasts
- **Multi-day Forecasts**: Displays a 6-day forecast for the requested location, allowing users to plan ahead
- **User-friendly Interface**: Simple command-line interface that prompts users for input and displays weather data in a readable format
## Getting Started

### Prerequisites
- **C++ Compiler**: GCC or any other C++ compiler that supports C++17
- **CMake**: Version 3.15 or higher
- **Conan**: Package manager for C++ dependencies
- **libcurl**: Transfer Library for C++
- **nlohmann/json**: JSON library for C++

### Installation
#### 1. Clone the Repository and Navigate to the Root Directory of the Project:
```bash
git clone https://github.com/AB1775/NWS-Forecast.git
cd NWS-Forecast/
```
#### 2. Install Dependencies using Conan:
```bash
conan install . --output-folder=build --build=missing
cd build/
cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
```
#### 3. Build the Project using CMake
```bash
cmake --build .
```
#### 4. Run the Program from the Build Directory
```bash
./nws
```

### Contributions
Contributions are welcome. If you have any suggestions or feature requests, please open an issue or submit a pull request.

### Acknowledgements
- [libcurl](https://curl.se/libcurl/)
- [nlohmann/json](https://github.com/nlohmann/json)
- [National Weather Service API](https://weather-gov.github.io/api/general-faqs)
