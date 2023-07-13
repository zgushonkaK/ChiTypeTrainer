#include <fstream>
#include <chrono>
#include <ctime>
#include <progress.hpp>

void progress::setUser(std::string user) { 
    current_user = std::move(user); 
}

void progress::addRecord(int speed, int acc) {
    loadJson();
    if (!data.contains(current_user)) {
        data[current_user] = json::array();
    }

    std::time_t currentTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    std::string dateTime = std::ctime(&currentTime);

    dateTime.erase(dateTime.length() - 1);

    json record = {
        {"date", dateTime},
        {"speed", speed},
        {"acc", acc}
    };
    data[current_user].push_back(record);

    saveJson();
}

std::vector<std::tuple<std::string, int, int>> progress::getRecords() {
    loadJson();

    std::vector<std::tuple<std::string, int, int>> userRecords;

    if (data.contains(current_user)) {
        for (const auto& record : data[current_user]) {
            std::string date = record["date"];
            int speed = record["speed"];
            int acc = record["acc"];
            userRecords.emplace_back(date, speed, acc);
        }
    }

    return userRecords;
}

void progress::deleteRecords() {
    loadJson();

    if (data.contains(current_user)) {
        data[current_user] = json::array();
    }

    saveJson();
}

void progress::loadJson() {
    if (data.empty()) {
        std::ifstream file("progress.json");
        if (!(file.peek() == std::ifstream::traits_type::eof())) {
            file >> data;
        }
    }
}

void progress::saveJson() {
    std::ofstream file("progress.json");
    file << data;
}