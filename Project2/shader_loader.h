#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

class Shader_loader {
public:
    enum MatrixType { M2, M3, M4 }; // Добавлен enum для типов матриц

    GLuint oneLinkProgram(const std::string& fragment_shader_path = "./fragment_shader.glsl",
        const std::string& vertex_shader_path = "./vertex_shader.glsl") {
        std::string vert_string, frag_string;

        // Чтение фрагментного шейдера
        std::ifstream frag_file(fragment_shader_path);
        if (frag_file.is_open()) {
            std::stringstream buffer;
            buffer << frag_file.rdbuf();
            frag_string = buffer.str();
            frag_file.close();
        }
        else {
            std::cerr << "Error: Unable to open fragment shader file!\n";
            return 0;
        }

        // Чтение вершинного шейдера
        std::ifstream vert_file(vertex_shader_path);
        if (vert_file.is_open()) {
            std::stringstream buffer;
            buffer << vert_file.rdbuf();
            vert_string = buffer.str();
            vert_file.close();
        }
        else {
            std::cerr << "Error: Unable to open vertex shader file!\n";
            return 0;
        }

        GLuint vs = glCreateShader(GL_VERTEX_SHADER);
        const char* vShaderCode = vert_string.c_str();
        glShaderSource(vs, 1, &vShaderCode, nullptr);
        glCompileShader(vs);
        checkCompileErrors(vs, "VERTEX");

        GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
        const char* fShaderCode = frag_string.c_str();
        glShaderSource(fs, 1, &fShaderCode, nullptr);
        glCompileShader(fs);
        checkCompileErrors(fs, "FRAGMENT");

        GLuint shader_program = glCreateProgram();
        glAttachShader(shader_program, vs);
        glAttachShader(shader_program, fs);
        glLinkProgram(shader_program);
        glDeleteShader(vs);
        glDeleteShader(fs);
        return shader_program;
    }

    void uniform_set_vec(GLuint shader_program, const std::string& attribute_name, float x) {
        glUniform1f(glGetUniformLocation(shader_program, attribute_name.c_str()), x);
    }

    void uniform_set_vec(GLuint shader_program, const std::string& attribute_name, float x, float y) {
        glUniform2f(glGetUniformLocation(shader_program, attribute_name.c_str()), x, y);
    }

    void uniform_set_vec(GLuint shader_program, const std::string& attribute_name, float x, float y, float z) {
        glUniform3f(glGetUniformLocation(shader_program, attribute_name.c_str()), x, y, z);
    }

    void uniform_set_vec(GLuint shader_program, const std::string& attribute_name, float x, float y, float z, float a) {
        glUniform4f(glGetUniformLocation(shader_program, attribute_name.c_str()), x, y, z, a);
    }

    void uniform_set_vec(GLuint shader_program, const std::string& attribute_name, GLsizei count, const GLfloat* value, GLboolean transpose, MatrixType m_type) {
        auto location = glGetUniformLocation(shader_program, attribute_name.c_str());
        switch (m_type) {
        case M2: glUniformMatrix2fv(location, count, transpose, value); break;
        case M3: glUniformMatrix3fv(location, count, transpose, value); break;
        case M4: glUniformMatrix4fv(location, count, transpose, value); break;
        }
    }

    void use(GLuint shader_program) {
        glUseProgram(shader_program);
    }

private:
    void checkCompileErrors(GLuint shader, const std::string& type) {
        GLint success;
        GLchar infoLog[1024];
        if (type != "PROGRAM") {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
                std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
        else {
#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

            class Shader_loader {
            public:
                enum MatrixType { M2, M3, M4 }; 

                GLuint oneLinkProgram(const std::string& fragment_shader_path = "./fragment_shader.glsl",
                    const std::string& vertex_shader_path = "./vertex_shader.glsl") {
                    std::string vert_string, frag_string;

                
                    std::ifstream frag_file(fragment_shader_path);
                    if (frag_file.is_open()) {
                        std::stringstream buffer;
                        buffer << frag_file.rdbuf();
                        frag_string = buffer.str();
                        frag_file.close();
                    }
                    else {
                        std::cerr << "Error: Unable to open fragment shader file!\n";
                        return 0;
                    }

                   
                    std::ifstream vert_file(vertex_shader_path);
                    if (vert_file.is_open()) {
                        std::stringstream buffer;
                        buffer << vert_file.rdbuf();
                        vert_string = buffer.str();
                        vert_file.close();
                    }
                    else {
                        std::cerr << "Error: Unable to open vertex shader file!\n";
                        return 0;
                    }

                    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
                    const char* vShaderCode = vert_string.c_str();
                    glShaderSource(vs, 1, &vShaderCode, nullptr);
                    glCompileShader(vs);
                    checkCompileErrors(vs, "VERTEX");

                    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
                    const char* fShaderCode = frag_string.c_str();
                    glShaderSource(fs, 1, &fShaderCode, nullptr);
                    glCompileShader(fs);
                    checkCompileErrors(fs, "FRAGMENT");

                    GLuint shader_program = glCreateProgram();
                    glAttachShader(shader_program, vs);
                    glAttachShader(shader_program, fs);
                    glLinkProgram(shader_program);
                    glDeleteShader(vs);
                    glDeleteShader(fs);
                    return shader_program;
                }

                void uniform_set_vec(GLuint shader_program, const std::string& attribute_name, float x) {
                    glUniform1f(glGetUniformLocation(shader_program, attribute_name.c_str()), x);
                }

                void uniform_set_vec(GLuint shader_program, const std::string& attribute_name, float x, float y) {
                    glUniform2f(glGetUniformLocation(shader_program, attribute_name.c_str()), x, y);
                }

                void uniform_set_vec(GLuint shader_program, const std::string& attribute_name, float x, float y, float z) {
                    glUniform3f(glGetUniformLocation(shader_program, attribute_name.c_str()), x, y, z);
                }

                void uniform_set_vec(GLuint shader_program, const std::string& attribute_name, float x, float y, float z, float a) {
                    glUniform4f(glGetUniformLocation(shader_program, attribute_name.c_str()), x, y, z, a);
                }

                void uniform_set_vec(GLuint shader_program, const std::string& attribute_name, GLsizei count, const GLfloat* value, GLboolean transpose, MatrixType m_type) {
                    auto location = glGetUniformLocation(shader_program, attribute_name.c_str());
                    switch (m_type) {
                    case M2: glUniformMatrix2fv(location, count, transpose, value); break;
                    case M3: glUniformMatrix3fv(location, count, transpose, value); break;
                    case M4: glUniformMatrix4fv(location, count, transpose, value); break;
                    }
                }

                void use(GLuint shader_program) {
                    glUseProgram(shader_program);
                }

            private:
                void checkCompileErrors(GLuint shader, const std::string& type) {
                    GLint success;
                    GLchar infoLog[1024];
                    if (type != "PROGRAM") {
                        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
                        if (!success) {
                            glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
                            std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
                        }
                    }
                    else {
                        glGetProgramiv(shader, GL_LINK_STATUS, &success);
                        if (!success) {
                            glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
                            std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
                        }
                    }
                }
            };

            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
                std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }
};
