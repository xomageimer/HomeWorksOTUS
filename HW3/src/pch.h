//
// Created by xomag on 24.08.2020.
//
                                                                    /*!
                                                                     * \file
                                                                     * \brief Набор макросов для генерации класса
                                                                     *
                                                                     *  Приведенные макросы генерируют класс специализированный
                                                                     *  под расшифровку 8-байтового ip адреса из целочиселнного типа.
                                                                     *
                                                                     */

#ifndef OTUSHOMEWORKS_PCH_H
#define OTUSHOMEWORKS_PCH_H
//
#include <iostream>
#include <memory>
#include <vector>
#include <map>
#include <sstream>
#include <utility>
#include <list>
#include <type_traits>

//-------------------------------------------------------------------------------------

// make struct
namespace user {
    struct empty {                                                  ///< Фиктивный класс для того чтобы правильно работал макрос насоедования
    };
}


#define MK_STRUCT(NAME) template<auto N, size_t s>\
struct NAME : INHERITANCE_2                                         ///< Макрос создания шаблонной сигнатуры структуры
                                                                    /*! \defgroup PRIVATE_INHERITANCE PRIVATE INHERITANCE
                                                                        @{ \details внутренний макрос для правильной работы макроса наследования
                                                                    */
#define INHERITANCE_2 INHERITANCE_WITHOUT_BRACE(public user::empty)
#define INHERITANCE_WITHOUT_BRACE(PARENTS) PARENTS
                                                                    /*! @} */
#define INHERITANCE(PARENTS) PARENTS {                              ///< Макрос наследования от другого класса
#define TYPE_DEC(T) T                                               ///< Макрос, позволяющий сделать макрос-подстановку типа
#define field(n) m_ ## n                                            ///< Макрос для объявления поля с префиксом m_
#define TYPE_DEC_MEMBER(T) T field(                                 ///< Макрос для объявления типа приватного m_ поля
#define PROPERTY(n) public : TYPE_DEC n;                            ///< Макрос для создания публичного свойства с вызовом макроса декларации типа
#define FIELD(n) private : TYPE_DEC_MEMBER n);                      ///< Макрос для создания приватного поля класса
#define END_STRUCT };                                               ///< Макрос конца структуры



#endif //OTUSHOMEWORKS_PCH_H
