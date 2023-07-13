#include<fstream>
#include<user.hpp>

void user::addUser(const std::string& name, const std::string& password) {
    std::ifstream file("users.json");
    json data;
    if (file.is_open()) {
        file >> data;
        file.close();
    }
    data["users"].push_back({
        {"name", name},
        {"password", password}
        });
    std::ofstream output("users.json");
    output << std::setw(4) << data << std::endl;
    output.close();
}

std::map<std::string, std::string> user::getUsersFromJson() {
    std::ifstream file("users.json");
    json data;
    if (file.is_open()) {
        file >> data;
        file.close();
    }
    std::map<std::string, std::string> users;
    for (auto& user : data["users"]) {
        std::string name = user["name"];
        std::string password = user["password"];
        users[name] = password;
    }
    return users;
}

void user::deleteUser(const std::string& nameToDelete) {
    std::ifstream file("users.json");
    json data;
    if (file.is_open()) {
        file >> data;
        file.close();
    }

    for (auto it = data["users"].begin(); it != data["users"].end(); ++it) {
        if (it.value()["name"] == nameToDelete) {
            data["users"].erase(it);
            break;
        }
    }

    std::ofstream output("users.json");
    output << std::setw(4) << data << std::endl;
    output.close();
}

void user::SetMode(int32_t md) { 
    cursor_mode = std::move(md); 
}

void user::setUser(std::string user) { 
    current_user = std::move(user); 
    cursor_mode = 0; 
}

std::string user::getUser() { 
    return current_user; 
}

int32_t user::getMode() { 
    return cursor_mode; 
}