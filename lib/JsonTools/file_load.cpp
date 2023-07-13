#include <fstream>
#include <cstdlib>
#include <ctime>
#include <file_load.hpp>

void Parser::parseFile(const std::string& filename) {
    std::ifstream file(filename);
    std::string word;
    char c;
    while (file.get(c)) {
        if (std::isalpha(c)) {
            word += std::toupper(c);
        }
        else if (!word.empty()) {
            words_.push_back(word);
            word.clear();
        }
    }

    if (!word.empty()) {
        words_.push_back(word);
    }

    file.close();
}

void Parser::fillWords() {
    std::ifstream file("default.json");
    json j;
    file >> j;
    for (auto& [key, value] : j.items()) {
        words_.push_back(key);
    }
    file.close();
}

std::string Parser::generateRandomString(const std::string& letters, int length) {
    std::vector<std::string> matchingWords;
    if (letters.length() == 0) {
        matchingWords = words_;
    }
    else {
        for (const std::string& word : words_) {
            bool matches = false;
            for (char c : letters) {
                if (word.find(c) != std::string::npos) {
                    matches = true;
                    break;
                }
            }
            if (matches) {
                matchingWords.push_back(word);
            }
        }
    }

    std::srand(std::time(nullptr));

    std::string result;
    int currentLength = 0;
    while (currentLength < length) {
        if (result.length() > 0) {
            result += " ";
            currentLength++;
        }
        std::string word = matchingWords[std::rand() % matchingWords.size()];
        int wordLength = std::min(length - currentLength, static_cast<int>(word.length()));
        result += word.substr(0, wordLength);
        currentLength += wordLength;
    }

    return result;
}

void file_control::set_letter_group(std::string group) { 
    letter_group = std::move(group); 
}

void file_control::set_path(std::string path) { 
    path_to_file = std::move(path); 
}

std::string file_control::getRandomString() {
    if (path_to_file.length() == 0) {
        parser.fillWords();
        return parser.generateRandomString(letter_group, max_char_count);
    }
    else {
        parser.parseFile(path_to_file);
        return parser.generateRandomString(letter_group, max_char_count);
    }
};