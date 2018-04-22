#include <GLFW/glfw3.h>

#include <cstdlib>

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

    glfwSwapInterval(1);
    while (!glfwWindowShouldClose(window))
    {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}
