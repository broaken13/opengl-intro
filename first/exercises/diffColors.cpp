#include <cstdio>
#include <string.h>
#include "include/glad/glad.h"
#include <GLFW/glfw3.h>

const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";


const char *orangeShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n";

const char *greyShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"FragColor = vec4(0.5f, 0.5f, 0.5f, 1.0f);\n"
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

    unsigned int orangeShader;
    orangeShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(orangeShader, 1, &orangeShaderSource, NULL);
    glCompileShader(orangeShader);

    unsigned int greyShader;
    greyShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(greyShader, 1, &greyShaderSource, NULL);
    glCompileShader(greyShader);

    if (!check_shader_compilation(vertexShader) || !check_shader_compilation(orangeShader) || !check_shader_compilation(greyShader))
    {
        printf("Problems compiling shader(s)");
        return -1;
    }

    // Link shaders into a program
    unsigned int orangeShaderProgram;
    orangeShaderProgram = glCreateProgram();
    glAttachShader(orangeShaderProgram, vertexShader);
    glAttachShader(orangeShaderProgram, orangeShader);
    glLinkProgram(orangeShaderProgram);

    int success;
    glGetProgramiv(orangeShaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        char log[512];
        glGetProgramInfoLog(orangeShaderProgram, 512, NULL, log);
        printf("Error linking shaders: %s", log);
        return -1;
    }
    // Cleanup shader objects
    glDeleteShader(vertexShader);
    glDeleteShader(orangeShader);


    float vertices[] = {
        -.5f, -.5f, .0f,
        .5f, -.5f, .0f,
        .0f, 0.5f, .0f,
        0.6f, -0.3f, .0f,
        0.1f, 0.8f, .0f,
        0.9f, 0.9f, .0f
    };

    // Vertex Array Objects allow us to store buffers and their configurations together
    // so that we can easily rebind to it to begin drawing an object again
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Tell OpenGL how to process the buffer
    // 0 corresponds with the layout (location = 0) in the vertex shader
    // 3 corresponds with the vec3, the size of the vertex attribute
    // GL_FLOAT cuz they're floats
    // GL_FALSE if we need it to normalize the coordinates for us (if we were using ints). We don't
    // Next is the "stride", or the distance between these attributes. 0 would have also worked since we're tightly packed
    // Finally, the offset. This is where the data begins. 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // 0 corresponds with the location in the shader
    glEnableVertexAttribArray(0);

    // We can do this as glVertexAttribPointer has now linked the buffer to the attribute for the shader
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // We could also unbind the VAO, but that's not usually necessary
    //  glBindVertexArray(0);
    
    // This will give us wireframe polygons
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);



    while(!glfwWindowShouldClose(window))
    {
        process_input(window);
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(orangeShaderProgram);
        glBindVertexArray(VAO); // Technically not necessary, as we're not switching VAO's, but if there are mutliple, this would be necessary
        // It's time to draw. We're doing a triangle
        // We're starting at index 0
        // and we're drawing 3 vertices
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwTerminate();
    return 0;
}

const char *shaderTemplate = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"FragColor = vec4(%.3ff, %.3ff, %.3ff, %.3ff);\n"
"}\n";

char* shader_maker(float a, float b, float c, float d)
{
    if (a > 1.0f) a -= (int)a;
    if (b > 1.0f) b -= (int)b;
    if (c > 1.0f) c -= (int)c;
    if (d > 1.0f) d -= (int)d;
    

    int length = snprintf(NULL, 0, shaderTemplate, a, b, c, d) + 1;

    printf("shader length is %d", length);

    char shaderSource[length];
    
    snprintf(shaderSource, length, shaderTemplate, a, b, c, d);
    printf("Shader:\n%s\n", shaderSource);

    unsigned int shader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(shader, 1, &shaderSource, NULL);
    glCompileShader(shader);
}
