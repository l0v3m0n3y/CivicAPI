# CivicAPI
api for civicapi.org site about vote 
# Example
```cpp
#include "CivicAPI.h"
#include <iostream>

int main() {
    CivicAPI api;
    
    auto search_task = api.search_races(
        "2024-01-01", 
        "2024-12-31", 
        "presidential", 
        "US", 
        "California"
    ).then([](json::value result) {
        std::cout << "Search results: " << result.serialize() << std::endl;
    });
    
    auto race_task = api.get_race(
        "12345", 
        false, 
        true, 
        "json", 
        true
    ).then([](json::value result) {
        std::cout << "Race info: " << result.serialize() << std::endl;
    });

    auto dates_task = api.get_election_dates("US", "California", "2024")
        .then([](json::value result) {
            std::cout << "Election dates: " << result.serialize() << std::endl;
        });
    search_task.wait();
    race_task.wait();
    dates_task.wait();
    
    return 0;
}
```

# Launch (your script)
```
g++ -std=c++11 -o civicapi_test main.cpp -lcpprest -lssl -lcrypto -lpthread -lboost_system -lboost_chrono -lboost_thread
./civicapi_test
```
