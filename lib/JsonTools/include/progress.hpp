#include <vector>
#include <tuple>
#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

/*!
* @brief Класс для записи данных о прогрессе пользователя
*
* Позволяет записывать и считывать данные о прогрессе пользователя
*/
class progress {
public:
    /*!
    * @brief Конструктор по умолчанию
    */
    progress() = default;
    /*!
    * @brief Деструктор по умолчанию
    */
    ~progress() = default;
    /*!
    * @brief Задает имя пользователя, который сейчас использует программу
    * @param user Имя пользователя
    */
    void setUser(std::string user);
    /*!
    * @brief Записывает скорость и аккуратность пользователя в файл
    * @param speed Скорость печати пользователя
    * @param acc Аккуратность пользователя
    */
    void addRecord(int speed, int acc);
    /*!
    * @brief Возвращает все записи о прогрессе пользователя
    * @return Список содержащий в себе кортежи из трех элементов: точно время записи, скорость и аккуратность
    */
    std::vector<std::tuple<std::string, int, int>> getRecords();
    /*!
    * @brief Удаляет записи о прогрессе пользователя, который сейчас использует программу
    */
    void deleteRecords();
private:
    std::string current_user{ "default" };
    json data;
    void loadJson();
    void saveJson();
};