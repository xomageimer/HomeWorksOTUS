#include "Primitives.h"

Primitives::Rectangle::Rectangle(const std::vector<float> & vertices) {
    m_vertices = vertices;

    ConstructShape<Rectangle>(*this);
}

Primitives::Rectangle::~Rectangle() {
    glDeleteBuffers(1, &m_vertex_buffers);
    glDeleteVertexArrays(1, &m_local_buffer);
}

void Primitives::Rectangle::Draw() const {
    glBindVertexArray(this->m_local_buffer);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

const std::vector<float> & Primitives::Rectangle::GetVertices() const {
    return this->m_vertices;
}

void Primitives::Rectangle::SetVertices(const std::vector<float> & vertices) {
    this->m_vertices = vertices;
}

std::unique_ptr<Primitives::IShape> Primitives::Rectangle::Clone() const {
    return std::make_unique<Primitives::Rectangle>(this->m_vertices);
}


Primitives::Triangle::~Triangle() {
    glDeleteBuffers(1, &m_vertex_buffers);
    glDeleteVertexArrays(1, &m_local_buffer);
}

Primitives::Triangle::Triangle(const std::vector<float> &vertices) {
    m_vertices = vertices;

    ConstructShape<Triangle>(*this);
}

void Primitives::Triangle::Draw() const {
    glBindVertexArray(this->m_local_buffer);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

template <typename SHAPE>
void Primitives::ConstructShape(SHAPE & obj) {

    glGenVertexArrays(1, &obj.m_local_buffer);
    glGenBuffers(1, &obj.m_vertex_buffers);

    glBindVertexArray(obj.m_local_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, obj.m_local_buffer);

    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * obj.m_vertices.size(), &obj.m_vertices[0], GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), nullptr);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));

}

bool Primitives::Rectangle::Crossing(float x_mouse, float y_mouse) {
    return x_mouse < (x_pos + std::abs(m_vertices[0])) && y_mouse > (y_pos - std::abs(m_vertices[0])) && x_mouse > (x_pos - std::abs(m_vertices[0])) && y_mouse < (y_pos + std::abs(m_vertices[0]));
}

std::unique_ptr<Primitives::IShape> Primitives::Triangle::Clone() const {
    return std::make_unique<Primitives::Triangle>(this->m_vertices);
}

const std::vector<float> &Primitives::Triangle::GetVertices() const {
    return this->m_vertices;
}

void Primitives::Triangle::SetVertices(const std::vector<float> & vertices) {
    this->m_vertices = vertices;
}

bool Primitives::Triangle::Crossing(float x_mouse, float y_mouse) {
    return x_mouse < (x_pos + std::abs(m_vertices[0])) && y_mouse > (y_pos - std::abs(m_vertices[0])) && x_mouse > (x_pos - std::abs(m_vertices[0])) && y_mouse < (y_pos + std::abs(m_vertices[0]));
}

Colors::Red::Red() : Color(std::vector{1.f, 0.f, 0.f, 1.f}) {}

Colors::Green::Green() : Color(std::vector{0.f, 1.f, 0.f, 1.f})  {}

Colors::Blue::Blue() : Color(std::vector{0.f, 0.f, 1.f, 1.f}) {}

Colors::White::White() : Color(std::vector{0.7f, 0.7f, 0.7f, 0.6f}) {}

void Colors::Color::Transparency(float trans) {
    *prev(RGBA.end()) = trans;
}

const std::vector<float> &Colors::Color::GetColor() const {
    return this->RGBA;
}