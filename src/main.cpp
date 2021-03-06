#define GL_GLEXT_PROTOTYPES

#include "maze.hpp"

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
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

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

    GLuint tiles_buffer;
    glGenBuffers(1, &tiles_buffer);

    std::array<glm::vec2, 4> square = {{
        {-1.0, -1.0},
        {+1.0, -1.0},
        {+1.0, +1.0},
        {-1.0, +1.0},
    }};
    glBindBuffer(GL_ARRAY_BUFFER, square_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(square), square.data(), GL_STATIC_DRAW);

    amaze::maze::Maze maze("###########\n"
                           "#!!!!#....#\n"
                           "#.........#\n"
                           "#....#!!!!#\n"
                           "###########\n");

    glBindBuffer(GL_ARRAY_BUFFER, tiles_buffer);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(amaze::maze::Tile) * maze.size(),
                 maze.tiles(),
                 GL_STATIC_DRAW);

    auto vertex_shader_source = R"GLSL(
        #version 330

        #extension GL_ARB_explicit_uniform_location : require

        layout(location = 0) uniform mat4 v_vp;
        layout(location = 1) uniform int  v_maze_width;

        layout(location = 0) in vec2 v_vertex;
        layout(location = 1) in int  v_tile;

        flat out int f_tile;

        void main()
        {
            vec2 position = vec2(gl_InstanceID % v_maze_width,
                                 gl_InstanceID / v_maze_width);
            gl_Position = v_vp * vec4(position + v_vertex, 0.0, 1.0);

            f_tile = v_tile;
        }
    )GLSL";

    auto fragment_shader_source = R"GLSL(
        #version 330

        flat in int f_tile;

        void main()
        {
            if (f_tile == 0x2E) // .
            {
                gl_FragColor = vec4(0.1, 0.1, 0.1, 1.0);
            }
            else if (f_tile == 0x23) // #
            {
                gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
            }
            else if (f_tile == 0x21) // !
            {
                gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
            }
            else
            {
                gl_FragColor = vec4(1.0, 0.0, 1.0, 1.0);
            }
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
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_TRUE, 0, nullptr);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, tiles_buffer);
    glVertexAttribIPointer(1, 1, GL_BYTE, 0, nullptr);
    glVertexAttribDivisor(1, 1);

    glfwSwapInterval(1);
    while (!glfwWindowShouldClose(window))
    {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);

        float aspect_ratio = 1.0f * width / height;

        glm::mat4 p = glm::ortho(-aspect_ratio, aspect_ratio,
                                 -1.0f, 1.0f,
                                 -1.0f, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 v = glm::translate(glm::scale(glm::mat4(), glm::vec3(0.05)),
                                     glm::vec3(-4.0f, -4.0f, 0.0f));
        glm::mat4 vp = v * p;

        glUseProgram(program);
        glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(vp));
        glUniform1i(1, maze.width());
        glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, square.size(), maze.size());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}
