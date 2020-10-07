#ifndef OTUSHOMEWORKS_SHADER_H
#define OTUSHOMEWORKS_SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>
#include <functional>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/*! \defgroup AutoFuncGenerator макросы для генерации сишных функций (в C нет полиморфиза и поэтому некоторые функции перегружены по именам, а не по аргументам, как например: glUniform(разные постфиксы). Здесь прсдетавлено решение данной проблемы!)
    @{
*/

/*!
 * Макрос для тегирования постфикса функции glUniform
 */
#define UNIFORMTAG(ph_1, ph_2) glUniform ## ph_1 ## ph_2

/*!
 * Макрос для генерации токна вызова функции callable (определяющая какая функция должна быть вызвана)
 */
#define CallFuncs(type, separate) callable(UNIFORMTAG(1, type), glGetUniformLocation(ID, val_name.c_str()), std::forward<Params>(params)...) separate \
    callable(UNIFORMTAG(2, type), glGetUniformLocation(ID, val_name.c_str()), std::forward<Params>(params)...) separate \
    callable(UNIFORMTAG(3, type), glGetUniformLocation(ID, val_name.c_str()), std::forward<Params>(params)...) separate  \
    callable(UNIFORMTAG(4, type), glGetUniformLocation(ID, val_name.c_str()), std::forward<Params>(params)...)

/*!
 * Макрос для генерации токна вызова функции callable для матриц (определяющая какая функция должна быть вызвана)
 */
#define CallFuncsForMatrix(type, separate) callable(UNIFORMTAG(type, 2fv), glGetUniformLocation(ID, val_name.c_str()), 1, GL_FALSE, glm::value_ptr(std::forward<Params>(params)...)) separate \
    callable(UNIFORMTAG(type, 3fv), glGetUniformLocation(ID, val_name.c_str()), 1, GL_FALSE, glm::value_ptr(std::forward<Params>(params)...)) separate  \
    callable(UNIFORMTAG(type, 4fv), glGetUniformLocation(ID, val_name.c_str()), 1, GL_FALSE, glm::value_ptr(std::forward<Params>(params)...))

/*!
 * функция вызова подходящей по аргументам функции
 * @tparam F тип функции, которую пробуем вызвать
 * @tparam T типы ее аргументов
 * @param f функция для вызова
 * @param t аргументы функции
 * @return строка, оповещающая удалось ли вызвать эти аргументы с этой функцией
 */
template <typename F, typename... T>
constexpr std::string_view callable(F && f, T &&... t) {
    if constexpr (std::is_invocable_v<F, decltype(std::forward<T>(t))...>) {
        std::invoke(std::forward<F>(f), std::forward<T>(t)...);
        return "Callable";
    } else {
        return "Non-callable";;
    }
}

/*! @} */

//! класс Шейдера для более удобного чтения их из файлов glsl
struct Shader {
public:
    /*!
     * конструктор класса
     * @param vertexShaderCode путь к файлу вертексного шейдера
     * @param fragmentShaderCode путь к файлу фрагментного шейдера
     */
    Shader (const std::filesystem::path & vertexShaderCode, const std::filesystem::path & fragmentShaderCode);

    /*!
     * получение ID шейдерной программы
     * @return ID шейдераной программы
     */
    inline uint32_t & GetId() {
        return ID;
    }

    /*!
     * Функция, перегружающая вызовы сишных функций glUniform(постфикс) с помощью аргументов, а не имет этих функций!
     * @tparam Params тип аргументов
     * @param val_name имя uniform переменной шейдерной программы
     * @param params аргументы для вызова glUniform
     */
    template<typename... Params>
    void glUniform(const std::string &val_name, Params... params) {
        if constexpr (std::is_same_v<Params..., float>) {
            CallFuncs(f, ;);
        }
        else if constexpr (std::is_same_v<Params..., int>) {
            CallFuncs(i, ;);
        }
        else if constexpr (std::is_same_v<Params..., glm::mat4>){
            CallFuncsForMatrix(Matrix, ;);
        }
    }

    /*!
     * вызов использования этой шейдерной программы контекстом OpenGL
     */
    inline void Use(){
        glUseProgram(ID);
    }

    /*!
     * проверка на валидность работы шейдерв
     * @return истинна если шейдер работает
     */
    bool Check(unsigned int &);

private:
    unsigned int ID; ///< ID шейдерной программы
};


#endif //OTUSHOMEWORKS_SHADER_H
