#include<string>
#include<vector>
#include<map>
#include<nlohmann/json.hpp>

using json = nlohmann::json;

/*!
* @brief Класс управления данными о пользователе
*
* Позволяет создавать и редактивровать записи о пользователях
*/
class user {
public:
    /*!
    * @brief Конструктор по умолчанию
    */
    user() = default;
    /*!
    * @brief Деструктор по умолчанию
    */
    ~user() = default;
      /*!
    * @brief Создает нового пользователя
    * @param name Имя пользователя
    * @param name Пароль пользователя
    */
    void addUser(const std::string& name, const std::string& password);
    /*!
    * @brief Возвращает имена и пароли пользователей
    * @return Словарь, где ключ имя пользователя, значение пароль
    */
    std::map<std::string, std::string> getUsersFromJson();
    /*!
    * @brief Задает режим курсора для пользователя
    */
    void SetMode(int32_t md);
    /*!
    * @brief Задает имя пользователя, который сейчас использует программу
    */
    void setUser(std::string user);
    /*!
    * @brief Возвращает имя пользователя, который сейчас использует программу
    * @return Имя пользователя
    */
    std::string getUser();
    /*!
    * @brief Удаляет пользователя
    */
    void deleteUser(const std::string& nameToDelete);
    /*!
    * @brief Возвращает режим курсора
    * @return Режим курсора
    */
    int32_t getMode();
private:
    json users;
    std::string current_user{"default"};
    int32_t cursor_mode{ 0 };
};