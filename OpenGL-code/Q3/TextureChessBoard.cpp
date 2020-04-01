
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <corecrt_math_defines.h>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 1000;

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec2 aTexCoord;\n"
"out vec2 TexCoord;\n"

"void main()\n"
"{\n"
"   gl_Position = vec4(aPos, 1.0);\n"
"   TexCoord = aTexCoord;\n"
"}\0";
const char* fragmentShaderSourceWhite = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec2 TexCoord;\n"
"uniform sampler2D ourTexture;\n"

"void main()\n"
"{\n"
"   FragColor = texture(ourTexture, TexCoord);\n"
"}\n\0";
const char* fragmentShaderSourceBlack = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec2 TexCoord;\n"
"uniform sampler2D ourTexture;\n"

"void main()\n"
"{\n"
"   FragColor = texture(ourTexture, TexCoord);\n"
"}\n\0";
int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Chess Board", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shaders
    int fragmentShaderBlack = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderBlack, 1, &fragmentShaderSourceBlack, NULL);
    glCompileShader(fragmentShaderBlack);
    int fragmentShaderWhite = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderWhite, 1, &fragmentShaderSourceWhite, NULL);
    glCompileShader(fragmentShaderWhite);
    // check for shader compile errors
    glGetShaderiv(fragmentShaderBlack, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShaderBlack, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    glGetShaderiv(fragmentShaderWhite, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShaderWhite, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    int shaderProgramBlack = glCreateProgram();
    glAttachShader(shaderProgramBlack, vertexShader);
    glAttachShader(shaderProgramBlack, fragmentShaderBlack);
    glLinkProgram(shaderProgramBlack);
    int shaderProgramWhite = glCreateProgram();
    glAttachShader(shaderProgramWhite, vertexShader);
    glAttachShader(shaderProgramWhite, fragmentShaderWhite);
    glLinkProgram(shaderProgramWhite);
    // check for linking errors
    glGetProgramiv(shaderProgramBlack, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgramBlack, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glGetProgramiv(shaderProgramWhite, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgramWhite, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShaderBlack);
    glDeleteShader(fragmentShaderWhite);


    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[384 * 5];
    for (int i = 0;i < 384;i++) vertices[(5 * i) + 2] = 0.0f;
    int c = 0;
    for (float x = -1.0f;x < 1.0f;x += 0.25f)
    {
        for (float y = -1.0f;y < 1.0f;y += 0.25f)
        {
            vertices[c] = x;
            vertices[c + 1] = y;
            vertices[c + 3] = x;
            vertices[c + 4] = y;
            vertices[c + 5] = x;
            vertices[c + 6] = y + 0.25f;
            vertices[c + 8] = x;
            vertices[c + 9] = y + 0.25f;
            vertices[c + 10] = x + 0.25f;
            vertices[c + 11] = y;
            vertices[c + 13] = x + 0.25f;
            vertices[c + 14] = y;
            c = c + 15;
        }
    }
    for (float x = -1.0f;x < 1.0f;x += 0.25f)
    {
        for (float y = -1.0f;y < 1.0f;y += 0.25f)
        {
            vertices[c] = x + 0.25f;
            vertices[c + 1] = y + 0.25f;
            vertices[c + 3] = x + 0.25f;
            vertices[c + 4] = y +0.25f;
            vertices[c + 5] = x;
            vertices[c + 6] = y + 0.25f;
            vertices[c + 8] = x;
            vertices[c + 9] = y + 0.25f;
            vertices[c + 10] = x + 0.25f;
            vertices[c + 11] = y;
            vertices[c + 13] = x + 0.25f;
            vertices[c + 14] = y;

            c = c + 15;
        }
    }
    c = -15;
    float WhiteSquareVertices[192 * 5];
    for (int i = 0;i < (192 * 5);i = i + 15)
    {
        if (i == 120 || i == 120 * 2 || i == 120 * 3 || i == 120 * 4 || i == 120 * 5 || i == 120 * 6 || i == 120 * 7 || i == 120 * 8) c = c + 45;
        else if (i == 60 || i == 120 * 2 - 60 || i == 120 * 3 - 60 || i == 120 * 4 - 60 || i == 120 * 5 - 60 || i == 120 * 6 - 60 || i == 120 * 7 - 60 || i == 120 * 8 - 60) c = c + 15;
        else c = c + 30;
        for(int j=0;j<15;j++)
        WhiteSquareVertices[i+j] = vertices[c+j];
    }
    c = -30;
    float BlackSquareVertices[192 * 5];
    for (int i = 0;i < (192 * 5);i = i + 15)
    {
        if (i == 120 || i == 120 * 2 || i == 120 * 3 || i == 120 * 4 || i == 120 * 5 || i == 120 * 6 || i == 120 * 7 || i == 120 * 8) c = c + 45;
        else if (i == 60 || i == 120 * 2 - 60 || i == 120 * 3 - 60 || i == 120 * 4 - 60 || i == 120 * 5 - 60 || i == 120 * 6 - 60 || i == 120 * 7 - 60 || i == 120 * 8 - 60) c = c + 15;
        else c = c + 30;
        for (int j = 0;j < 15;j++)
        BlackSquareVertices[i + j] = vertices[c + j];
    }

    unsigned int VBOB, VBOW, VAOB, VAOW;
    glGenVertexArrays(1, &VAOB);
    glGenBuffers(1, &VBOB);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAOB);

    glBindBuffer(GL_ARRAY_BUFFER, VBOB);
    glBufferData(GL_ARRAY_BUFFER, sizeof(BlackSquareVertices), BlackSquareVertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenVertexArrays(1, &VAOW);
    glGenBuffers(1, &VBOW);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAOW);

    glBindBuffer(GL_ARRAY_BUFFER, VBOW);
    glBufferData(GL_ARRAY_BUFFER, sizeof(WhiteSquareVertices), WhiteSquareVertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // load and create a texture 
 // -------------------------
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    unsigned char* data = stbi_load("Sea.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // bind Texture
        glBindTexture(GL_TEXTURE_2D, texture);

        // render
        // ------
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // render the triangle
        glUseProgram(shaderProgramWhite);
        glBindVertexArray(VAOW);
        glDrawArrays(GL_TRIANGLES, 0, 192);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAOB);
    glDeleteBuffers(1, &VBOB);
    glDeleteVertexArrays(1, &VAOW);
    glDeleteBuffers(1, &VBOW);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}