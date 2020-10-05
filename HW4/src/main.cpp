#include <iostream>
#include <sstream>
#include <cmath>
#include <iomanip>
#include <filesystem>
#include <string>
#include <fstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Primitives.h"
#include "Widgets.h"

void framebuffer_size_callback(GLFWwindow*, int, int);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
struct Mouse;

float x_pose, y_pose;
const int Width = 800, Height = 600;

int main () {
    using namespace std;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(Width, Height, "Editor", NULL, NULL);
    if (!window){
        std::cout << "Failed to create window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to init GLAD" << std::endl;
        return EXIT_FAILURE;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    //std::cerr << std::filesystem::current_path().parent_path().parent_path() << std::endl;

    Shader shaderWrapper ( (std::filesystem::current_path().parent_path()).parent_path() /"HW4/shaders/VertexShader.glsl", (std::filesystem::current_path().parent_path()).parent_path() /"HW4/shaders/FragmentShader.glsl" );

    std::vector<std::unique_ptr<Primitives::IShape>> sceneObjects;
    Widgets::BuildWidgets::Instance().SetButtonRectangle(sceneObjects, -0.3f, -0.6f);
    Widgets::BuildWidgets::Instance().SetButtonTriangle(sceneObjects, 0.3f, -0.6f);
    Widgets::BuildWidgets::Instance().SetButtonColor(0.8f, -0.1f, Widgets::COLORS::BLUE);
    Widgets::BuildWidgets::Instance().SetButtonColor(0.8f, 0.2f, Widgets::COLORS::GREEN);
    Widgets::BuildWidgets::Instance().SetButtonColor(0.8f, 0.5f, Widgets::COLORS::RED);
    Widgets::BuildWidgets::Instance().SetNewButton(sceneObjects, -0.8f, 0.5f);
    Widgets::BuildWidgets::Instance().SetImportButton(sceneObjects, -0.8f, 0.2f);
    Widgets::BuildWidgets::Instance().SetExportButton(sceneObjects, -0.8f, -0.1f);

    std::vector<std::unique_ptr<Widgets::IWidget>> sceneWidgets = Widgets::BuildWidgets::Instance().Build();

    std::pair<float*, float*> pos;

    bool l_click = false;
    auto l_cur_iter = sceneObjects.rbegin();

    bool r_click = false;
    auto r_cur_iter = sceneObjects.rbegin();

    float last_cli = 0.0f;
    float last_del = 0.0f;

    while(!glfwWindowShouldClose(window))
    {
        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        glClearColor(0.9f, 0.9f, 1.f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        shaderWrapper.Use();

        for (auto it = sceneObjects.rbegin(); it != sceneObjects.rend(); it++){
            pos = (*it)->GetPosition();
            auto x = (float)glfwGetTime();
            if ((*it)->Crossing(x_pose, y_pose) && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS && !r_click && !l_click && (x - last_del >= 0.3f)){
                r_click = true;
                last_del = x;
                sceneObjects.erase(it.base() - 1);
                break;
            } else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE){
                r_click = false;
            }

            if ((*it)->Crossing(x_pose, y_pose) && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && (!l_click || it == l_cur_iter) && !r_click){
                *pos.first = x_pose;
                *pos.second = y_pose;
                l_cur_iter = it;
                l_click = true;
            } else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE && it == l_cur_iter){
                l_click = false;
            }

            glm::mat4 trans = glm::mat4(1.0f);
            trans = glm::translate(trans, glm::vec3(*pos.first, *pos.second, 0.0));
            shaderWrapper.glUniform("transform", trans);
            (*it)->Draw();
        }


        for (auto & i : sceneWidgets){
            auto x = (float)glfwGetTime();
            glm::mat4 trans = glm::mat4(1.0f);
            pos = i->GetPosition();
            if (i->Button(x_pose, y_pose) && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && (x - last_cli >= 0.5f)) {
                i->Action();
                last_cli = x;
            }
            trans = glm::translate(trans, glm::vec3(*pos.first, *pos.second, 0.0));
            shaderWrapper.glUniform("transform", trans);
            i->Draw();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return EXIT_SUCCESS;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos){
    using std::cout;
    using std::endl;

    //  cout << "x before: " << xpos << " ,y before: " << ypos << endl;

    glm::vec3 pos_2D {xpos, ypos, 0.f};

    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.f));

    glm::mat4 projection = glm::frustum(-1.0f, 1.0f, -1.f, 1.f, 0.1f, 100.f);

    glm::vec3 proje_vect = glm::unProject(pos_2D, model, projection, glm::vec4(0.f, 0.f, 800.f, 600.f));

//    if (proje_vect.x > -0.9f && proje_vect.x < 0.9f && (-1) * proje_vect.y > -0.9f && (-1) * proje_vect.y < 0.9f) {
//        x_pos = proje_vect.x;
//        y_pos = (-1) * proje_vect.y;
//    }

    x_pose = proje_vect.x;
    y_pose = (-1) * proje_vect.y;

 //   cout << "x: " << proje_vect.x  << " y: " << (-1) * proje_vect.y << endl << endl;
}