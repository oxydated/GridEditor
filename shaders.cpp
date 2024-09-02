#include <GL/glew.h>
#include <sstream>
#include <string>

std::string fragmentShaderString;
std::string vertexShaderString;

GLuint shaderProgram = -1;

void setVertexShader() {
    std::stringstream vs;
    vs << "#version 130" << "\n";

    vs << "in  vec3 in_Position;" << "\n";
    vs << "in  vec3 in_Color;" << "\n";
    vs << "out vec3 ex_Color;" << "\n";

    vs << "void main(void)" << "\n";
    vs << "{" << "\n";
    vs << "    ex_Color = in_Color;" << "\n";
    vs << "    gl_Position = vec4(in_Position, 1.0);" << "\n";
    vs << "}" << "\n";

    vertexShaderString = vs.str();
}


void setFragmentShader() {
    std::stringstream fs;
    fs << "#version 130" << "\n";

    fs << "precision highp float; // needed only for version 1.30" << "\n";

    fs << "in  vec3 ex_Color;" << "\n";
    fs << "out vec4 out_Color;" << "\n";

    fs << "void main(void)" << "\n";
    fs << "{" << "\n";
    fs << "    out_Color = vec4(ex_Color, 1.0);" << "\n";

    // Try replacing the above with the following:
    //vec3 tmp_Color;
    //tmp_Color = ex_Color.rrr;
    //out_Color = vec4(tmp_Color,1.0);
    fs << "}" << "\n";

    fragmentShaderString = fs.str();
}

bool setShaders() {

    setVertexShader();
    setFragmentShader();

    GLuint vertexShaderObj = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShaderObj = glCreateShader(GL_FRAGMENT_SHADER);

    const char* vertexShaderBuffer = vertexShaderString.c_str();

    glShaderSource(vertexShaderObj, 1, &vertexShaderBuffer, nullptr);
    glCompileShader(vertexShaderObj);

    GLint isCompiled = 0;
    glGetShaderiv(vertexShaderObj, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE) {
        GLint maxLength = 0;
        glGetShaderiv(vertexShaderObj, GL_INFO_LOG_LENGTH, &maxLength);
        char* infoLog = new char[maxLength];
        glGetShaderInfoLog(vertexShaderObj, maxLength, &maxLength, infoLog);
        glDeleteShader(vertexShaderObj);
        delete[] infoLog;
        return false;
    }

    const char* fragmentShaderBuffer = fragmentShaderString.c_str();

    glShaderSource(fragmentShaderObj, 1, &fragmentShaderBuffer, nullptr);
    glCompileShader(fragmentShaderObj);

    glGetShaderiv(fragmentShaderObj, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE) {
        GLint maxLength = 0;
        glGetShaderiv(fragmentShaderObj, GL_INFO_LOG_LENGTH, &maxLength);
        char* infoLog = new char[maxLength];
        glGetShaderInfoLog(fragmentShaderObj, maxLength, &maxLength, infoLog);
        glDeleteShader(fragmentShaderObj);
        delete[] infoLog;
        return false;
    }

    shaderProgram = glCreateProgram();

    glBindAttribLocation(shaderProgram, 0, "in_Position");
    glBindAttribLocation(shaderProgram, 1, "in_Color");

    glAttachShader(shaderProgram, vertexShaderObj);
    glAttachShader(shaderProgram, fragmentShaderObj);

    //glBindAttribLocation(shaderProgram, 0, "vertexPosition_modelspace");

    glLinkProgram(shaderProgram);

    GLint isLinked = -1;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &isLinked);
    if (isLinked == GL_FALSE) {
        GLint maxLength = 0;
        glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &maxLength);
        char* infoLog = new char[maxLength];
        glGetProgramInfoLog(shaderProgram, maxLength, &maxLength, infoLog);
        delete[] infoLog;

        glDeleteShader(vertexShaderObj);
        glDeleteShader(fragmentShaderObj);
        glDeleteProgram(shaderProgram);
        return false;
    }

    glUseProgram(shaderProgram);

    return true;
}

void useShader() {
    glUseProgram(shaderProgram);
}

void destroyShaders() {
    //glDeleteShader(vertexShaderObj);
    //glDeleteShader(fragmentShaderObj);
    glDeleteProgram(shaderProgram);
}
