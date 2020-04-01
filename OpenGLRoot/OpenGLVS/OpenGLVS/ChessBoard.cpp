#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char* fragmentShaderSourceBlack = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);\n"
"}\n\0";
const char* fragmentShaderSourceWhite = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
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
    float vertices[384 * 3];
    for (int i = 0;i < 384;i++) vertices[(3 * i) + 2] = 0.0f;
    int c = 0;
    for (float x = -1.0f;x < 1.0f;x+=0.25f)
    {
        for (float y = -1.0f;y < 1.0f;y+=0.25f)
        {
            vertices[c] = x;
            vertices[c + 1] = y;
            vertices[c + 3] = x;
            vertices[c + 4] = y + 0.25f;
            vertices[c + 6] = x + 0.25f;
            vertices[c + 7] = y;

            c = c + 9;
        }
    }
    for (float x = -1.0f;x < 1.0f;x += 0.25f)
    {
        for (float y = -1.0f;y < 1.0f;y += 0.25f)
        {
            vertices[c] = x + 0.25f;
            vertices[c + 1] = y + 0.25f;
            vertices[c + 3] = x;
            vertices[c + 4] = y + 0.25f;
            vertices[c + 6] = x + 0.25f;
            vertices[c + 7] = y;

            c = c + 9;
        }
    }
    c = -9;
    float WhiteSquareVertices[192 * 3];
    for (int i = 0;i < (192 * 3);i=i+9)
    {
        if (i == 72 || i == 72 * 2 || i == 72 * 3 || i == 72 * 4 || i == 72 * 5 || i == 72 * 6 || i == 72 * 7 || i == 72 * 8) c = c + 27;
        else if (i == 36 || i == 72 * 2 - 36 || i == 72 * 3 - 36 || i == 72 * 4 - 36 || i == 72 * 5 - 36 || i == 72 * 6 - 36 || i == 72 * 7 - 36 || i == 72 * 8 - 36) c = c + 9;
        else c = c + 18;
        WhiteSquareVertices[i] = vertices[c];
        WhiteSquareVertices[i + 1] = vertices[c + 1];
        WhiteSquareVertices[i + 2] = vertices[c + 2];
        WhiteSquareVertices[i + 3] = vertices[c + 3];
        WhiteSquareVertices[i + 4] = vertices[c + 4];
        WhiteSquareVertices[i + 5] = vertices[c + 5];
        WhiteSquareVertices[i + 6] = vertices[c + 6];
        WhiteSquareVertices[i + 7] = vertices[c + 7];
        WhiteSquareVertices[i + 8] = vertices[c + 8];
    }
    c = -18;
    float BlackSquareVertices[192 * 3];
    for (int i = 0;i < (192 * 3);i = i + 9)
    {
        if (i == 72 || i == 72 * 2 || i == 72 * 3 || i == 72 * 4 || i == 72 * 5 || i == 72 * 6 || i == 72 * 7 || i == 72 * 8) c = c + 27;
        else if (i == 36 || i == 72 * 2 - 36 || i == 72 * 3 - 36 || i == 72 * 4 - 36 || i == 72 * 5 - 36 || i == 72 * 6 - 36 || i == 72 * 7 - 36 || i == 72 * 8 - 36) c = c + 9;
        else c = c + 18;
        BlackSquareVertices[i] = vertices[c];
        BlackSquareVertices[i + 1] = vertices[c + 1];
        BlackSquareVertices[i + 2] = vertices[c + 2];
        BlackSquareVertices[i + 3] = vertices[c + 3];
        BlackSquareVertices[i + 4] = vertices[c + 4];
        BlackSquareVertices[i + 5] = vertices[c + 5];
        BlackSquareVertices[i + 6] = vertices[c + 6];
        BlackSquareVertices[i + 7] = vertices[c + 7];
        WhiteSquareVertices[i + 8] = vertices[c + 8];
    }

    unsigned int VBOB,VBOW, VAOB, VAOW;
    glGenVertexArrays(1, &VAOB);
    glGenBuffers(1, &VBOB);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAOB);

    glBindBuffer(GL_ARRAY_BUFFER, VBOB);
    glBufferData(GL_ARRAY_BUFFER, sizeof(BlackSquareVertices), BlackSquareVertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
     glBindVertexArray(0);
     glBindBuffer(GL_ARRAY_BUFFER, 0);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // render the triangle
        glUseProgram(shaderProgramBlack);
        glBindVertexArray(VAOB);
        glDrawArrays(GL_TRIANGLES, 0, 192);

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