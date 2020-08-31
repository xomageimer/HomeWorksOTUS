#include "pch.h"
#include <typeinfo>

                                                                     /*!
                                                                     * \file
                                                                     * \brief Программа для печати ip адреса основнная на принципах метапрограммирования
                                                                     *
                                                                     *  Программа позволяет выполнить вывод ip адресов следующих типов:
                                                                     *  integral, string, vector, list, tuple.
                                                                     *
                                                                     */

using namespace std;
                                                                    /*! \defgroup SPECIAL_STRUCT BYTE READER
                                                                       @{ \details Класс сконструированный из макросов для обработки целочисленных значений и расшифровки ip адресов
                                                                   */
MK_STRUCT(byte_reader) INHERITANCE()
    FIELD((static const int) BYTE = 255)
    FIELD((static const int) OCTATE = 8)
    PROPERTY((static constexpr auto) value = N >> s * m_OCTATE - m_OCTATE & m_BYTE)  ///< \public Создание статических констант, хранящих 8 байтный элемент ip адреса
END_STRUCT                                                         /*! @} */

// META_LIST start


                                /*!
                                 * сигнатура мета листа
                                 * @tparam Args набор типов
                                 */

template <typename ... Args>
struct Meta_List;
                                                                    /*! \defgroup META_LIST STRUCT FOR ITERATE ALL BYTES
                                                                        @{ \details список работающий во время компиляции, храняший и выводящий байты.
                                                                    */
                               /*!
                                * Нода для хранения значений
                                * @tparam val - значение конкретного элемента
                                */
template <auto val>
struct Meta_Node{
    static constexpr auto value = val;
};

                                /*!
                                 * Частичная специализация мета лист для 2 элементов
                                 * @tparam F хранит Meta_Node
                                 * @tparam O хранит Meta_List с последующей иерархией
                                 * \public value значение конкретного нода
                                 */
template <typename F, typename O>
struct Meta_List<F, O>{
    using Front = F;
    using Other = O;
    static constexpr auto value = F::value;
};

                                /*!
                                 * заглушка на конец листа
                                 */
template <>
struct Meta_List<>{};

                                /*!
                                 * функция мета листа для создания дерева с определнным количеством элементов
                                 * @tparam T интегральный тип
                                 * @tparam BR структура \ref byte_reader<T>
                                 * @tparam number интегральное значение
                                 * @tparam size размер типа в байтах
                                 * \typedef extract отрезок хранящий кусок Meta_List'a
                                 */

template <typename T, template<T, size_t bytes>class BR, T number, size_t size>
struct Push_Back{
    using extract = Meta_List<Meta_Node<BR<number, size>::value>, typename Push_Back<T, BR, number, size-1>::extract>;
};

                                /*!
                                 * частичная специализация функции \ref push_back<T, BR, number, size> когда размер == 0
                                 * @tparam T интегральный тип
                                 * @tparam BR структура \ref byte_reader<T>
                                 * @tparam number интегральное значение
                                 */

template <typename T, template<T, size_t bytes>class BR, T number>
struct Push_Back <T, BR, number, 0> {
    using extract = Meta_List<>;
};

                                /*!
                                 * метафункция для инициализации списка
                                 * @tparam T интегральный тип
                                 * @tparam N переменная типа
                                 */

template <typename T, T N>
struct Initialization_Meta_List {
    using extract = typename Push_Back<T, byte_reader, N, sizeof(N)>::extract;
};

                                /*!
                                 * мета_IF для проверки, является ли элемент последним в мета листе
                                 * @tparam R отрезок мета листа
                                 */

template <class R>
struct is_End : false_type {};

                                /*!
                                 * мета_IF для проверки, является ли элемент последним в мета листе
                                 * @tparam F первый элемент отрезка
                                 * @tparam O отрезок мета листа (extract)
                                 * @tparam R мета лист
                                 */

template <typename F, typename O, template<typename F1 = F, typename O1 = O> class R>
struct is_End<R<F, O>> : true_type {};

                                /*!
                                * \overload template <typename L> static constexpr void print_ip_impl()
                                * Вывод в "no impl" случае false значения у метафункции \ref is_End <R>
                                */
template <typename L>
static constexpr enable_if_t<!is_End<L>::value, void> print_ip_impl(ostream & os){
    os << "no impl" << endl;
}

                                    /*!
                                     * \overload template <typename L> static constexpr void print_ip_impl()
                                     * вывод всех значений мета листа
                                     * @tparam L мета лист
                                     * @return void
                                    */


template <typename L>
static constexpr enable_if_t<is_End<L>::value, void> print_ip_impl(ostream & os){
    if (is_End<typename L::Other>::value) {
        os << L::value << '.';
        print_ip_impl<typename L::Other>(os);
    }
    else
        os << L::value << '\n';
}


// META_LIST end

                                        /*! @} */

                                                       /*! \defgroup STRUCT_HELPERS STRUCT FOR HELP TO METAPROGRAMMING
                                                           @{ \details набор шаблонных функций использующих SFINAE для реализации некоторых метафункций выполняющихся в print_ip
                                                       */

                                                       /*!
                                                        * \details Функция для проверки валидности итераторов контейнеров
                                                        * @tparam T тип контейнера
                                                        * @return void
                                                        */
template <typename T>
static constexpr enable_if_t<is_same_v<typename std::iterator_traits<typename T::iterator>::iterator_category(), random_access_iterator_tag()> || is_same_v<typename std::iterator_traits<typename T::iterator>::iterator_category(), bidirectional_iterator_tag()>, void>
is_vector_or_list (){}


                                                                /*! \defgroup ALL_SAME UNIQUE ALGORITHM IN METAPROGRAMMING
                                                                    @{ \details метацикл для проверки уникальности типов
                                                                */
template<typename T, typename...Args> struct all_same ;
template<typename T> struct all_same<T, T> : true_type {};
template<typename T, typename...Args> struct all_same<T, T, Args...> : all_same<T, Args...> {};
template <typename T, typename U, typename...Args> struct all_same<T, U, Args...> : false_type {};
template <typename T, typename U> struct all_same<T, U> : false_type {};
                                                                /*! @} */


                                                                /*! \defgroup TUPLE_ITERATE METAPROGRAMMING TUPLE ITERATOR FUNCTION
                                                                    @{ \details итерирование по элиментам tuple и вывод в обратном порядке (от 0)
                                                                */
template <size_t i, typename T>
struct tuple_iterate{
    static constexpr void iterate(T & tup, ostream & os = cout) {
        tuple_iterate<i - 1, T>().iterate(tup, os);
        os << '.' << get<i>(tup);
    }
};

template <typename T>
struct tuple_iterate<0, T>{
    static constexpr void iterate(T & tup, ostream & os = cout) {
        os << get<0>(tup);
    }
};
                                                             /*! @} */

/*! @} */


                                                            /*! \defgroup PRINT_IP SFINAE PRINT IP
                                                               @{ \details набор шаблонных функций использующих SFINAE для вывода ip адресов
                                                           */
                                                            /*!
                                                            Выполняется если не удастся подобрать print_ip функцию с помощью SFINAE
                                                            \tparam Args набор шаблонных параметров
                                                            \return void
                                                            */
template <typename ... Args>
static constexpr void print_ip(ostream & os = cout){
    os << "no impl" << endl;
}

                                                            /*!
                                                            Выполняется если в шаблон передается интегральный тип и его переменная
                                                            \tparam T Тип, обязательно должен быть целочисленным
                                                            \tparam value Значение - представляемое целочисленным типом
                                                            \return void
                                                            */
template <typename T, T value>
static constexpr enable_if_t<is_integral_v<remove_cv_t<T>>, void> print_ip(ostream & os = cout){
    print_ip_impl<typename Initialization_Meta_List<T, value>::extract>(os);
}


                                                            /*!
                                                            \overload template <typename T> static constexpr void print_ip(T)
                                                           Выполняется если в шаблон передан вектор или лист
                                                           \tparam T Тип, обязательно должен иметь метод emplace_back, а также инстанциировать собой функцию is_vector_or_list<T>()
                                                           \param[in] container вектор или лист содержащие ip адрес
                                                           \return void
                                                           */
template <typename T>
static constexpr enable_if_t< is_same_v<decltype(is_vector_or_list<T>()), void> && ( is_same_v<decltype(declval<T>().emplace_back(typename T::value_type())), void> || is_same_v<decltype(declval<T>().emplace_back(typename T::value_type())), typename T::reference>), void> print_ip(T container, ostream & os = cout) {
    size_t j = container.size();
    for (auto & i : container)
        if (--j)
            os << i << '.';
    os << *prev(container.end()) << endl;
}

                                                           /*!
                                                            \overload template <typename T> static constexpr void print_ip(T)
                                                           В другом случае выполняется перегрузка если в шаблон передана строка std::string
                                                           \tparam T Тип string
                                                           \param[in] str строка содержащая ip адрес
                                                           \return void
                                                           */


template <typename T>
static constexpr enable_if_t<is_same_v<string, T>, void> print_ip(T str, ostream & os = cout)
{
    os << str << endl;
}

                                                           /*!
                                                          В случае вызова print_ip от tuple будет вызвана эта функция
                                                          \tparam Args набор типов tuple
                                                          \param[in] tup tuple содержащий Args типы
                                                          \details Выполнится только в случае полного совпадения типов tuple, для чего вызывается \ref all_same< T, Args >
                                                          \return void
                                                          */

template <typename ... Args>
static constexpr enable_if_t<all_same<Args...>::value, void> print_ip (tuple<Args...> tup, ostream & os = cout) {
    const size_t tup_size = tuple_size<tuple<Args...>>::value - 1;
    tuple_iterate<tup_size, tuple<Args...>>().iterate(tup, os);
    os << endl;
}

                                                            /*! @} */