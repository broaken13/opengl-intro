
#include "stdio.h"
#include "include/glad/glad.h"
#include <GLFW/glfw3.h>


const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";


const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n";

void framebuffer_size_callback(GLFWwindow *window, int width, int height) 
{
    glViewport(0, 0, width, height);
}

void process_input(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) 
    {
        glfwSetWindowShouldClose(window, true);
    }
}

int check_shader_compilation(unsigned int shaderObject)
{

    int success;
    const int logLength = 512;
    char infoLog[logLength];
    glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(shaderObject, logLength, NULL, infoLog);
        printf("Error compiling vertex shader: %s", infoLog);
    }

    return success;
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "Learning some shit", NULL, NULL);
    if (window == NULL)
    {
        printf("Couldn't make a goddamn window\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
       printf("GLAD has failed us\n");
       return -1;
    }


    // Compile shaders
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    if (!check_shader_compilation(vertexShader) || !check_shader_compilation(fragmentShader))
    {
        printf("Problems compiling shader(s)");
        return -1;
    }

    // Link shaders into a program
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    int success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        char log[512];
        glGetProgramInfoLog(shaderProgram, 512, NULL, log);
        printf("Error linking shaders: %s", log);
        return -1;
    }
    // Cleanup shader objects
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    const int triangles = 2;

    float triangleDefinitions[triangles][9] = {
        {
            -.5f, -.5f, .0f,
            .5f, -.5f, .0f,
            .0f, 0.5f, .0f
        },{
            0.6f, -0.3f, .0f,
            0.1f, 0.8f, .0f,
            0.9f, 0.9f, .0f
        }
    };

    // Vertex Array Objects allow us to store buffers and their configurations together
    // so that we can easily rebind to it to begin drawing an object again
    unsigned int VAOs[triangles];
    glGenVertexArrays(triangles, VAOs);

    unsigned int VBOs[triangles];
    glGenBuffers(triangles, VBOs);

    for (int i = 0; i < triangles; i++) {
        glBindVertexArray(VAOs[i]);
        glBindBuffer(GL_ARRAY_BUFFER, VBOs[i]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(triangleDefinitions[i]), triangleDefinitions[i], GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }

    // This will give us wireframe polygons
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


    while(!glfwWindowShouldClose(window))
    {
        process_input(window);
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAOs[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(VAOs[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(triangles, VAOs);
    glDeleteBuffers(triangles, VBOs);
    glDeleteProgram(shaderProgram);
    
    glfwTerminate();
    return 0;
}
