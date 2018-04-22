#define GL_GLEXT_PROTOTYPES

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <array>
#include <cstdlib>
#include <iostream>

namespace
{
    template<typename T>
    T check(T value)
    {
        if (!value)
        {
            std::exit(1);
        }
        return value;
    }
}

int main()
{
    check(glfwInit());

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    auto window = check(glfwCreateWindow(640, 480, "Amaze", nullptr, nullptr));

    glfwMakeContextCurrent(window);

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback([] (GLenum, GLenum, GLuint, GLenum, GLsizei,
                               GLchar const* message, void const*) {
        std::cout << message << '\n';
        std::exit(1);
    }, nullptr);

    GLuint square_buffer;
    glGenBuffers(1, &square_buffer);

    GLuint positions_buffer;
    glGenBuffers(1, &positions_buffer);

    std::array<glm::vec2, 4> square = {{
        {-1.0, -1.0},
        {+1.0, -1.0},
        {+1.0, +1.0},
        {-1.0, +1.0},
    }};
    glBindBuffer(GL_ARRAY_BUFFER, square_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(square), square.data(), GL_STATIC_DRAW);

    std::array<glm::vec2, 4> positions = {{
        {-2.0, -2.0},
        {+2.0, -2.0},
        {+2.0, +2.0},
        {-2.0, +2.0},
    }};
    glBindBuffer(GL_ARRAY_BUFFER, positions_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions.data(), GL_STATIC_DRAW);

    auto vertex_shader_source = R"GLSL(
        #extension GL_ARB_explicit_attrib_location : require
        #extension GL_ARB_explicit_uniform_location : require

        layout(location = 0) uniform mat4 vp;

        layout(location = 0) in vec2 vertex;
        layout(location = 1) in vec2 position;

        void main()
        {
            gl_Position = vp * vec4(position + vertex, 0.0, 1.0);
        }
    )GLSL";

    auto fragment_shader_source = R"GLSL(
        void main()
        {
            gl_FragColor = vec4(1.0, 0.0, 1.0, 1.0);
        }
    )GLSL";

    auto vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, nullptr);
    glCompileShader(vertex_shader);

    auto fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, nullptr);
    glCompileShader(fragment_shader);

    auto program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    GLuint vertex_array;
    glGenVertexArrays(1, &vertex_array);


    glBindVertexArray(vertex_array);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, square_buffer);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, positions_buffer);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    glVertexAttribDivisor(1, 1);


    glfwSwapInterval(1);
    while (!glfwWindowShouldClose(window))
    {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 vp = glm::scale(glm::mat4(), glm::vec3(0.25f));

        glUseProgram(program);
        glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(vp));
        glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, square.size(), positions.size());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}
