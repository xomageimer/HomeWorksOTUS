#ifndef OTUSHOMEWORKS_PRIMITIVES_H
#define OTUSHOMEWORKS_PRIMITIVES_H

#include <vector>
#include <memory>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


/*!
\file
\brief Заголовочный файл с описанием классов примитивов

Данный файл содержит в себе определения основных
классов, используемых в качестве примитивов на форме
*/


/*!
 * \brief пространство имен объектов примитивов
 */
namespace Primitives{

    struct IShape;

    /*!
     * Функция для генерации вертоксноых буферов и буфером этих атрибутов в контексте OpenGL для
     * последующих вычислений на видеокарте
     * @tparam SHAPE тип примитива (прямоугольник или треугольник)
     * @param obj объект класса SHAPE в который нужно сгенерировать все необходимые атрибуты
     */
    template <typename SHAPE>
    void ConstructShape(SHAPE & obj);

    /*!
     * Интерфейс для базовых фигур
     */
    struct IShape {
        virtual ~IShape() = default;

        /*!
        * Метод для вызова отрисовки примитва средставми OpenGl
        */
        virtual void Draw() const = 0;
        /*!
        * Метод для создания прототипа конкретного объекта
        * \return Точно такой же примитив как и сам объект из которого вызван метод
        */
        [[nodiscard]] virtual std::unique_ptr<IShape> Clone() const = 0;
        /*!
        * Метод для получения вершин текущего объекта
        * \return возвращает вектор координат и цветов вершин объекта
        */
        [[nodiscard]] virtual const std::vector<float> & GetVertices() const = 0;
        /*!
        * Метод для установки новых вершин
        * \param vec вектор содержащий новые координаты и цвета вершин
        */
        virtual void SetVertices(const std::vector<float> & vec) = 0;
        /*!
        * Метод для проверки пересечения с координатами мыши
         * \param x_mouse координата x курсора
         * \param y_mouse координата y курсора
         * \return пересекла ли мышки границы текущего объекта
        */
        /*!
         * \code
            void mouse_callback(GLFWwindow* window, double xpos, double ypos){
                glm::vec3 pos_2D {xpos, ypos, 0.f};
                glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.f));
                glm::mat4 projection = glm::frustum(-1.0f, 1.0f, -1.f, 1.f, 0.1f, 100.f);
                glm::vec3 proje_vect = glm::unProject(pos_2D, model, projection, glm::vec4(0.f, 0.f, 800.f, 600.f));

                x_pose = proje_vect.x;
                y_pose = (-1) * proje_vect.y;
            }
         * \endcode
         */
        virtual bool Crossing(float x_mouse, float y_mouse) = 0;

        virtual std::pair<float*, float*> GetPosition() = 0;

        template <typename SHAPE>
        friend void ConstructShape(SHAPE & obj);

    };

    /*!
     * Класс фигуры прямоугольника
     */
    struct Rectangle : public IShape {
        ~Rectangle() override;

        Rectangle() = delete;

        explicit Rectangle(const std::vector<float> &
        vertices);

        void Draw() const
        override;

        [[nodiscard]] std::unique_ptr<IShape> Clone() const
        override;

        [[nodiscard]] const std::vector<float> & GetVertices() const
        override;

        void SetVertices(const std::vector<float> &) override;

        inline std::pair<float*, float*> GetPosition() override {
            return std::make_pair(&x_pos, &y_pos);
        }

        virtual bool Crossing(float x_mouse, float y_mouse) override;

        template <typename SHAPE>
        friend void ConstructShape(SHAPE & obj);

    private:
        unsigned int m_local_buffer;    ///< вершинный буферный массив контекста OpenGL
        unsigned int m_vertex_buffers;  ///< вершинный буферный объект для хранения координат в памяти видеокарте

        std::vector<float> m_vertices;  ///< вершины объекта
        float x_pos = 0.f, y_pos = 0.f; ///< координаты центра объекта
    };

    /*!
     * Класс фигуры треугольник
     */
    struct Triangle : public IShape {
        ~Triangle() override;

        Triangle() = delete;

        explicit Triangle(const std::vector<float> &
        vertices);

        void Draw() const
        override;

        [[nodiscard]] std::unique_ptr<IShape> Clone() const
        override;

        [[nodiscard]] const std::vector<float> & GetVertices() const
        override;

        void SetVertices(const std::vector<float> &) override;

        inline std::pair<float*, float*> GetPosition() override {
            return std::make_pair(&x_pos, &y_pos);
        }

        virtual bool Crossing(float x_mouse, float y_mouse) override;

        template <typename SHAPE>
        friend void ConstructShape(SHAPE & obj);

    private:
        unsigned int m_local_buffer;
        unsigned int m_vertex_buffers;

        std::vector<float> m_vertices;
        float x_pos = 0.f, y_pos = 0.f;
    };
}

/*!
 * \brief пространство имен цветов
 */
namespace Colors {

    /*!
     * Интерфейс для реализации цвета
     */
    struct IColor{
        virtual ~IColor() = default;

        /*! \bug прозрачность - не работает :c */
        virtual void Transparency(float) = 0;
    };

    /*!
     * Структура цветов
     */
    struct Color : IColor {
        Color (const std::vector<float> & col = {1.0f, 1.0f, 1.0f, 1.0f}) : RGBA {col} {
        }

        void Transparency(float) override;

        /*!
         * Метод для получения цветов
         * @return возвращает координаты цвета
         */
        [[nodiscard]] const std::vector<float> & GetColor () const;

    private:
        std::vector<float> RGBA; ///< вектор хранящий цвет
    };

    /*!
     * Структура цвета Red
     */
    struct Red : public Color{
        Red ();
    };

    /*!
    * Структура цвета Green
    */
    struct Green : public Color{
        Green ();
    };

    /*!
   * Структура цвета Blue
   */
    struct Blue : public Color{
        Blue ();
    };

    /*!
   * Структура цвета White
   */
    struct White : public Color{
        White ();
    };

}



#endif //OTUSHOMEWORKS_PRIMITIVES_H
