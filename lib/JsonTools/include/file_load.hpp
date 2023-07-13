#include <string>
#include <vector>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

/*!
* @brief Класс для парсинга файлов txt
*
* Позволяет создавать и редактивровать записи о пользователях
*/
class Parser {
public:
    /*!
    * @brief Конструктор по умолчанию
    */
    Parser() = default;
    /*!
    * @brief Деструктор по умолчанию
    */
    ~Parser() = default;
    /*!
    * @brief Разбивает файл на слова
    * @param filename Имя файла
    */
    void parseFile(const std::string& filename);
    /*!
    * @brief Заполняет словами из дефолтного набора
    */
    void fillWords();
    /*!
    * @brief Генерирует строку из случайных слов
    * @param leters Группа букв которые должны быть в словах в строке
    * @param length Длина строки
    */
    std::string generateRandomString(const std::string& letters, int length);

private:
    std::vector<std::string> words_;
};

/*!
* @brief Класс для более удобного взаимодействия класса Parser с основной программой
*
* Позволяет задавать пользователя в данный момент времени, возвращать строку фиксированного размера, задавать буквенную группу
*/
class file_control {
public:
    /*!
    * @brief Задает буквенную группу
    * @param group Строка с группой
    */
    void set_letter_group(std::string group);
    /*!
    * @brief Задает путь до файла
    * @param path Путь до файла
    */
    void set_path(std::string path);
    /*!
    * @brief Возвращает строку определенной длины
    * @return Строка фиксированной длины
    */
    std::string getRandomString();
private:
    std::string path_to_file{ "" };
    std::string letter_group{ "" };
    int max_char_count{ 83 };
    Parser parser;
};
