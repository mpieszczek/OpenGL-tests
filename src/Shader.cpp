#include "Shader.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include "Renderer.h"


Shader::Shader(const std::string& filepath):m_FilePath(filepath)
{
	ShaderProgramSource source = ParseShader(filepath);
	m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}


Shader::~Shader()
{
	GLCall(glDeleteProgram(m_RendererID));
}


void Shader::Bind() const
{
	GLCall(glUseProgram(m_RendererID));
}


void Shader::Unbind() const
{
	GLCall(glUseProgram(0));
}

void Shader::SetUniform1i(const std::string& name, int value){
	GLCall(glUniform1i(GetUniformLocation(name), value));
}
void Shader::SetUniform4f(const std::string& name, float v0,float v1, float v2, float v3)
{
	GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}
void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& m){
	GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &m[0][0]));
}

int Shader::GetUniformLocation(const std::string& name)
{
	//sprawdzenie czy nazwa istnieje w mapie
	if ( m_UniformlocationCache.find(name) != m_UniformlocationCache.end())
		return m_UniformlocationCache[name];
	
	GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
	if(location == -1)
		std::cout<<"Warning: uniform location "<<name<<" doesn't exist."<<std::endl;
	else 
		m_UniformlocationCache[name] = location;
	return location;
}


ShaderProgramSource Shader::ParseShader(const std::string& filepath)
{
	enum class ShaderType {
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::ifstream stream(filepath);
	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;

	while (getline(stream,line)) {
		if (line.find("#shader") != std::string::npos) {
			if (line.find("vertex") != std::string::npos) {
				type = ShaderType::VERTEX;
			} else if (line.find("fragment")!=std::string::npos) {
				type = ShaderType::FRAGMENT;
			}
		} else {
			ss[(int)type] << line << '\n';
		}
	}
	return { ss[0].str(), ss[1].str()};
}


unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();//&source[0] - alt (pierwszy znak tekstu)
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length* sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile " <<
		          (type == GL_VERTEX_SHADER ? "vertex": "fragment")<< " shader!"<< std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}
	return id;
}


unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	/* Pobiera 2 kody źródłowe w formie stringa i generuje wyjściowy shader */
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	// linkowanie
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}
