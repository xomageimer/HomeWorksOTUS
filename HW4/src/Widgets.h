#ifndef OTUSHOMEWORKS_WIDGETS_H
#define OTUSHOMEWORKS_WIDGETS_H

#include <vector>
#include <set>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>
#include <optional>
#include "Primitives.h"


/*!
\file
\brief Заголовочный файл с описанием классов объектов интерфейса

Данный файл содержит в себе определения основных
классов, используемых в качестве движка для интерфейса программы
*/

/*!
 * \brief пространство имен объектов интерфейса
 */


namespace Widgets {

    extern std::unique_ptr<Colors::Color> current_color; ///< глобальная переменная для переключения цвета

    /*!
     Сериализатор данных для импорта в фаил сохранения
     \param[in] data Данные для сохранения
     \return текст с сериализованными данными
    */
    std::string Serialize(std::vector<std::unique_ptr<Primitives::IShape>>& data);

    /*!
     Сериализатор данных для экспорта фаила из сохраненного ранее
     \param[in] data Данные для загрузки
     \return  загруженные данные для отображения примитивов
    */
    std::vector<std::unique_ptr<Primitives::IShape>> DeSerialize(std::fstream & stream);

    // 0 - rect, 1 - trian
    extern int button_type; ///< глобальная переменная для переключения формы кнопки

    /*!
     * Интерфейс для дефолтной кнопки
     */
    struct IWidget {
        /*!
         * Виртуальный деструктор для вызовов диструкторв у наследующихся классов
         */
        virtual ~IWidget() = default;

        /*!
         * Метод для вызова отрисовки примитва (отрисовка кнопки)
         */
        virtual void Draw() = 0;

        /*!
         * Метод для обработки нажатия на кнопку
         * \param[in] x_mouse позиция мыши по горизонтали
         * \param[in] y_mouse позиция мыши по вертикали
         */
        virtual bool Button(float x_mouse, float y_mouse) = 0;

        /*!
         * Метод для вызова процедур при условии что кнопка нажата
         */
        virtual void Action () = 0;

        /*!
         * Метод обертка над методом \ref Primitives::IShape::GetPoisition для позиции кнопки
         */
        virtual std::pair<float*, float*> GetPosition() = 0;
    };

    /*!
     * \brief Класс кнопки создания прямоугольника
     * Необходим для корректной работы с прямоугольниками
     */
    struct CreateModelButton_Rectangle : public IWidget{
        /*!
         * Конструктор кнопки для создания прямоугльников
         * \param[in] ver вектор background редактора (в нем хранятся все примитивы на форме)
         * \param[in] x смещение от центра по горизонтали
         * \param[in] y смещение по вертикали от центра
         */
        CreateModelButton_Rectangle(std::vector<std::unique_ptr<Primitives::IShape>> & ver, float x = 0.f, float y = 0.f);

        /*!
        * Метод для вызова отрисовки примитва (отрисовка кнопки)
        */
        void Draw() override;

        /*!
         * Метод для обработки нажатия на кнопку
         * \param[in] x_mouse позиция мыши по горизонтали
         * \param[in] y_mouse позиция мыши по вертикали
         */
        bool Button(float x_mouse, float y_mouse) override;

        /*!
         * Метод для вызова процедур при условии что кнопка нажата
         */
        void Action() override;

        inline std::pair<float*, float*> GetPosition() override {
            return sprite.value()->GetPosition();
        }

    private:
        std::optional<std::unique_ptr<Primitives::IShape>> sprite; ///< внешний вид кнопки
        std::vector<std::unique_ptr<Primitives::IShape>> & ground; ///< ссылка на background (все примитивы на форме)
    };


    /*!
     * \brief Класс кнопки создания треугольника
     * Необходим для корректной работы с треугольниками
     */
    struct CreateModelButton_Triangle : public IWidget{
        /*!
         * Конструктор кнопки для создания треугольников
         * \param[in] ver вектор background редактора (в нем хранятся все примитивы на форме)
         * \param[in] x смещение от центра по горизонтали
         * \param[in] y смещение по вертикали от центра
         */
        CreateModelButton_Triangle(std::vector<std::unique_ptr<Primitives::IShape>> & ver, float x = 0.f, float y = 0.f);

        /*!
        * Метод для вызова отрисовки примитва (отрисовка кнопки)
        */
        void Draw() override;

        /*!
         * Метод для обработки нажатия на кнопку
         * \param[in] x_mouse позиция мыши по горизонтали
         * \param[in] y_mouse позиция мыши по вертикали
         */
        virtual bool Button(float x_mouse, float y_mouse);

        /*!
        * Метод для вызова процедур при условии что кнопка нажата
        */
        void Action() override;

        inline std::pair<float*, float*> GetPosition() override {
            return sprite.value()->GetPosition();
        }

    private:
        std::optional<std::unique_ptr<Primitives::IShape>> sprite;
        std::vector<std::unique_ptr<Primitives::IShape>> & ground;
    };

    //! Набор возможных цветов
    enum class COLORS : int {
        RED,    ///< Красный цвет
        GREEN,  ///< Зеленый цвет
        BLUE    ///< Голубой цвет
    };

    //! база цветов по их номеру в енумераторе
    extern const std::set<Widgets::COLORS> data_base_color;

    /*!
     * Структура для создания кнопки цвета
     */
    struct CreateColorButton : public IWidget{
        /*!
         * Конструктор кнопки
         * \param[in] x смещение от центра по горизонтали
         * \param[in] y смещение по вертикали от центра
         * \param[in] color цвет на который переключает кнопка
         */
        explicit CreateColorButton(float x = 0.f, float y = 0.f, COLORS color = COLORS::RED);

        /*!
        * Метод для вызова отрисовки примитва (отрисовка кнопки)
        */
        void Draw() override;

        /*!
         * Метод для обработки нажатия на кнопку
         * \param[in] x_mouse позиция мыши по горизонтали
         * \param[in] y_mouse позиция мыши по вертикали
         */
        bool Button(float x_mouse, float y_mouse) override;

        /*!
       * Метод для вызова процедур при условии что кнопка нажата
       */
        void Action() override;

        inline std::pair<float*, float*> GetPosition() override {
            switch(::Widgets::button_type) {
                case 0 :
                    return sprite_rect.value()->GetPosition();
                    break;

                case 1 :
                    return sprite_trian.value()->GetPosition();
                    break;

                default:
                    return sprite_rect.value()->GetPosition();
                    break;
            }
        }

    private:
        std::optional< std::unique_ptr<Primitives::IShape> > sprite_rect; ///< внешний вид кнопки, если последним был создан прямоугольник
        std::optional< std::unique_ptr<Primitives::IShape> > sprite_trian; ///< внешний вид кнопки, если последним был создан треугольник

        COLORS color;  ///< цвет, который хранит кнопка
    };

    /*!
     * Структура кнопки очистки экрана / создания нового пространства
     */
    struct NewGroundButton : public IWidget{
        /*!
         * Конструктор кнопки
         * \param[in] ver вектор background редактора (в нем хранятся все примитивы на форме)
         * \param[in] x смещение от центра по горизонтали
         * \param[in] y смещение по вертикали от центра
         */
        explicit NewGroundButton (std::vector<std::unique_ptr<Primitives::IShape>> & ver, float x = 0.f, float y = 0.f);

        /*!
        * Метод для вызова отрисовки примитва (отрисовка кнопки)
        */
        void Draw() override;

        /*!
         * Метод для обработки нажатия на кнопку
         * \param[in] x_mouse позиция мыши по горизонтали
         * \param[in] y_mouse позиция мыши по вертикали
         */
        bool Button(float x_mouse, float y_mouse) override;

        /*!
       * Метод для вызова процедур при условии что кнопка нажата
       */
        void Action() override;

        inline std::pair<float*, float*> GetPosition() override {
            return sprite.value()->GetPosition();
        }

        friend std::string Serialize(std::vector<std::unique_ptr<Primitives::IShape>>& data);

        friend std::vector<std::unique_ptr<Primitives::IShape>> DeSerialize(std::fstream & stream);

    private:
        std::optional<std::unique_ptr<Primitives::IShape>> sprite;
        std::vector<std::unique_ptr<Primitives::IShape>> & ground;
    };

    /*!
     * Структура кнопки загрузки файла (импорта)
     */
    struct ImportGroundButton : public IWidget{
        /*!
         * Конструктор кнопки
         * \param[in] ver вектор background редактора (в нем хранятся все примитивы на форме)
         * \param[in] x смещение от центра по горизонтали
         * \param[in] y смещение по вертикали от центра
         */
        explicit ImportGroundButton (std::vector<std::unique_ptr<Primitives::IShape>> & ver, float x = 0.f, float y = 0.f);

        /*!
        * Метод для вызова отрисовки примитва (отрисовка кнопки)
        */
        void Draw() override;

        /*!
         * Метод для обработки нажатия на кнопку
         * \param[in] x_mouse позиция мыши по горизонтали
         * \param[in] y_mouse позиция мыши по вертикали
         */
        bool Button(float x_mouse, float y_mouse) override;

        /*!
      * Метод для вызова процедур при условии что кнопка нажата
      */
        void Action();

        inline std::pair<float*, float*> GetPosition() override {
            return sprite.value()->GetPosition();
        }

        friend std::string Serialize(std::vector<std::unique_ptr<Primitives::IShape>>& data);

        friend std::vector<std::unique_ptr<Primitives::IShape>> DeSerialize(std::fstream & stream);

    private:
        std::optional<std::unique_ptr<Primitives::IShape>> sprite;
        std::vector<std::unique_ptr<Primitives::IShape>> & ground;
    };

    /*!
     * Структура кнопки сохранения в файл (экспорта)
     */
    struct ExportGroundButton : public IWidget{
        /*!
         * Конструктор кнопки
         * \param[in] ver вектор background редактора (в нем хранятся все примитивы на форме)
         * \param[in] x смещение от центра по горизонтали
         * \param[in] y смещение по вертикали от центра
         */
        explicit ExportGroundButton (std::vector<std::unique_ptr<Primitives::IShape>> & ver, float x = 0.f, float y = 0.f);

        /*!
        * Метод для вызова отрисовки примитва (отрисовка кнопки)
        */
        void Draw() override;

        /*!
         * Метод для обработки нажатия на кнопку
         * \param[in] x_mouse позиция мыши по горизонтали
         * \param[in] y_mouse позиция мыши по вертикали
         */
        bool Button(float x_mouse, float y_mouse) override;

        /*!
          * Метод для вызова процедур при условии что кнопка нажата
          */
        void Action() override;

        inline std::pair<float*, float*> GetPosition() override {
            return sprite.value()->GetPosition();
        }

        friend std::string Serialize(std::vector<std::unique_ptr<Primitives::IShape>>& data);

        friend std::vector<std::unique_ptr<Primitives::IShape>> DeSerialize(std::fstream & stream);

    private:
        std::optional<std::unique_ptr<Primitives::IShape>> sprite;
        std::vector<std::unique_ptr<Primitives::IShape>> & ground;
    };


    /*!
     * Структура для построения интерфейса (позволяет редакторировать интерфейс внутри кода)
     */
    struct BuildWidgets{
        //! статический метод для того, чтобы редактор интерфейса был в программе один (сингтон Майерса)
        static BuildWidgets & Instance();

        /*! \defgroup SetMethodsOfInterface Сеттеры для построения интерфейса приложения
          @{
         */
        void SetButtonRectangle (std::vector<std::unique_ptr<Primitives::IShape>> &, float x, float y);
        void SetButtonTriangle (std::vector<std::unique_ptr<Primitives::IShape>> &, float x, float y);
        void SetButtonColor (float x, float y, COLORS color);
        void SetImportButton (std::vector<std::unique_ptr<Primitives::IShape>> &, float x, float y);
        void SetExportButton (std::vector<std::unique_ptr<Primitives::IShape>> &, float x, float y);
        void SetNewButton (std::vector<std::unique_ptr<Primitives::IShape>> &, float x, float y);
        /*! @} */

        //! Метод для генерации конечного интерфейса
        inline std::vector<std::unique_ptr<Widgets::IWidget>> Build() {
            return std::move(Widgets);
        }

    private:
        BuildWidgets() = default; ///< конструктор (запрятан в private)
        std::vector<std::unique_ptr<Widgets::IWidget>> Widgets; ///< вектор, хранящий в себе элементы интерфейса на форме
    };
}



#endif //OTUSHOMEWORKS_WIDGETS_H
