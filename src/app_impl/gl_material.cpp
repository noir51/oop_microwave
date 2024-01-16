
/*
 * Copyright (c) 2024, noir51
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Path: app_impl/gl_material.cpp
 */

#include <microwave/debug/logger.h>
#include <microwave/app_impl/gl_material.h>

GLMaterial::GLMaterial()
{
    program__ = glCreateProgram();
}

GLMaterial::GLMaterial(const std::string &vspath) : GLMaterial()
{
    set_vertex_shader_source(vspath);
}

GLMaterial::GLMaterial(const std::string &vs_path, const std::string &fs_path) : GLMaterial()
{
    set_vertex_shader_source(vs_path);
    set_fragment_shader_source(fs_path);
}

GLMaterial::GLMaterial(const std::string &gs_path, const std::string &vs_path, const std::string &fs_path) : GLMaterial()
{
    set_geometry_shader_source(gs_path);
    set_vertex_shader_source(vs_path);
    set_fragment_shader_source(fs_path);
}

GLMaterial::~GLMaterial()
{
    glDeleteProgram(program__);
}

void
GLMaterial::init()
{
    gshader__ = glCreateShader(GL_GEOMETRY_SHADER);
    vshader__ = glCreateShader(GL_VERTEX_SHADER);
    fshader__ = glCreateShader(GL_FRAGMENT_SHADER);
}

void
GLMaterial::compile()
{
    compile_geometry_shader();
    compile_vertex_shader();
    compile_fragment_shader();

    is_linked__ = link_program();

    glDeleteShader(gshader__);
    glDeleteShader(vshader__);
    glDeleteShader(fshader__);
}

void
GLMaterial::use()
{
    if (is_compiled_fshader__ || is_compiled_vshader__) {
        glUseProgram(program__);
    }
}

void
GLMaterial::set_geometry_shader_source(const std::string &path)
{
    gshader_src__ = get_shader_source(path);
    gs_path__ = path;
}

void
GLMaterial::set_vertex_shader_source(const std::string &path)
{
    vshader_src__ = get_shader_source(path);
    vs_path__ = path;
}

void
GLMaterial::set_fragment_shader_source(const std::string &path)
{
    fshader_src__ = get_shader_source(path);
    fs_path__ = path;
}

bool
GLMaterial::compile_geometry_shader()
{
    if (compile_shader(gshader__, gshader_src__, gs_path__, is_compiled_gshader__)) {
        glAttachShader(program__, gshader__);
        return true;
    }
    return false;
}

bool
GLMaterial::compile_vertex_shader()
{
    if (compile_shader(vshader__, vshader_src__, vs_path__, is_compiled_vshader__)) {
        glAttachShader(program__, vshader__);
        return true;
    }
    return false;
}

bool
GLMaterial::compile_fragment_shader()
{
    if (compile_shader(fshader__, fshader_src__, fs_path__, is_compiled_fshader__)) {
        glAttachShader(program__, fshader__);
        return true;
    }
    return false;
}

GLuint
GLMaterial::get_program() const
{
    return program__;
}

GLuint
GLMaterial::compile_shader(GLuint &shader, std::string source, std::string path, bool &compiled)
{
    if (source.empty() || path.empty()) {
        return 0;
    }

    const char *buffer = source.c_str();
    glShaderSource(shader, 1, &buffer, NULL);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        GLchar info_log[512];
        glGetShaderInfoLog(shader, 512, NULL, info_log);

        Logger::log("GLMaterial", "Shader failed to compile (%s):", path.c_str());
        std::istringstream iss(info_log);
        std::string line;
        while (std::getline(iss, line)) {
            Logger::log("GLMaterial", "%s", line.c_str());
        }
        compiled = false;
    }
    else {
        Logger::log("GLMaterial", "Shader compiled successfully (%s)", path.c_str());
        compiled = true;
    }

    return success;
}

GLint
GLMaterial::link_program()
{
    glLinkProgram(program__);

    GLint success;
    glGetProgramiv(program__, GL_LINK_STATUS, &success);

    if (!success) {
        GLchar info_log[512];
        glGetProgramInfoLog(program__, 512, NULL, info_log);
        Logger::log("GLMaterial", "Failed to link shader program:");
        std::istringstream iss(info_log);
        std::string line;
        while (std::getline(iss, line)) {
            Logger::log("GLMaterial", "%s", line.c_str());
        }
    }
    else {
        Logger::log("GLMaterial", "Shader program linked successfully");
    }

    return success;
}

std::string
GLMaterial::get_shader_source(const std::string &path)
{
    std::string shader_source;
    std::ifstream source_file;
    source_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        source_file.open(path);
        std::stringstream source_stream;
        source_stream << source_file.rdbuf();
        source_file.close();
        shader_source = source_stream.str();
    }
    catch (std::ifstream::failure &e) {
        Logger::log("GLMaterial", "Failed to read shader source file (%s)", path.c_str());
    }

    return shader_source;
}

/* EOF */
