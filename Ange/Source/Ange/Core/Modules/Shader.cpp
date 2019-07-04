#include "Apch.h"
#include "Shader.h"
#include "Ange/Core/Modules/Logger.h"

//Shaders data
#include "Ange/Shaders/Quad.h"
#include "Ange/Shaders/Image.h"
#include "Ange/Shaders/Text.h"

namespace Ange {

	ShaderManager::ShaderManager()
	{
		m_ShadersInitialized = false;
		if (m_ShadersInitialized == false) {
			//Quad shader
			AddShader(Shader::Quad::Name, Shader::Quad::Vertex, Shader::Quad::Fragment);
			LoadUniform(
				Shader::Quad::Name,
				Shader::Quad::UniformsAmount,
				Shader::Quad::Uniforms::MVP,
				Shader::Quad::Uniforms::Tint,
				Shader::Quad::Uniforms::BorderColor,
				Shader::Quad::Uniforms::Anchor,
				Shader::Quad::Uniforms::ChildPos,
				Shader::Quad::Uniforms::BorderData,
				//Shader::Quad::Uniforms::ClipArea,
				nullptr
			);

			AddShader(Shader::Image::Name, Shader::Image::Vertex, Shader::Image::Fragment);
			LoadUniform(
				Shader::Image::Name,
				Shader::Image::UniformsAmount,
				Shader::Image::Uniforms::MVP,
				Shader::Image::Uniforms::Tint,
				Shader::Image::Uniforms::BorderColor,
				Shader::Image::Uniforms::Anchor,
				Shader::Image::Uniforms::ChildPos,
				Shader::Image::Uniforms::BorderData,
				Shader::Image::Uniforms::DiffuseTex,
				nullptr
			);

			AddShader(Shader::Text::Name, Shader::Text::Vertex, Shader::Text::Fragment);
			LoadUniform(
				Shader::Text::Name,
				Shader::Text::UniformsAmount,
				Shader::Text::Uniforms::MVP,
				Shader::Text::Uniforms::Tint,
				Shader::Text::Uniforms::Anchor,
				Shader::Text::Uniforms::ChildPos,
				Shader::Text::Uniforms::DiffuseTex,
				Shader::Text::Uniforms::ClipArea,
				Shader::Text::Uniforms::DispOffset,
				nullptr
			);

			//Prevent re-initialization from other places than constructor
			m_ShadersInitialized = true;
		}
	}
	
	ShaderManager::~ShaderManager()
	{
		for (auto sh : m_Programs) {
			auto it = sh.second.find("ShaderId");
			if (it != sh.second.end()) {
				glDeleteProgram(it->second);
			}
			sh.second.clear();
		}
		m_Programs.clear();
	}

	void ShaderManager::AddShader(const char* shaderName, const std::string vertexShader, const std::string fragmentShader)
	{
		if (shaderName == nullptr) {
			std::string message = "[ShaderManager AddShader] A 'nullptr' was passed instead of a valid shader name.";
			ANGE_FATAL(message.c_str());
			throw std::runtime_error(message);
		}
		std::map<const char*, GLuint> tempMap;
		tempMap.insert(std::pair<const char*, GLuint>("ShaderId", CompileShader(shaderName, vertexShader.c_str(), fragmentShader.c_str())));
		m_Programs.insert(std::pair<const char*, std::map<const char*, GLuint>>(shaderName, tempMap));
	}

	void ShaderManager::LoadUniform(const char* shaderName, int uniformsAmount, const char* uniformName ...)
	{
		if (shaderName == nullptr) {
			std::string message = "[ShaderManager LoadUniform] A 'nullptr' was passed instead of a valid shader name.";
			ANGE_FATAL(message.c_str());
			throw std::runtime_error(message);
		}

		auto shaderData = GetShaderData(shaderName);
		if (shaderData != nullptr) {
			GLuint programID = shaderData->at("ShaderId");
			int counter = 0;
			va_list arguments;
			for(va_start(arguments, uniformName); uniformName != nullptr; uniformName = va_arg(arguments, const char *)) {
				GLuint uniformID = glGetUniformLocation(programID, uniformName);
				if (uniformID != (GLuint)-1) {
					++counter;
				} else {
					ANGE_WARNING("Cannot load uniform: [%s]", uniformName);
				}
				shaderData->insert(std::pair<const char*, GLuint>(uniformName, uniformID));
			}
			va_end(arguments);

			if (counter == uniformsAmount) {
				ANGE_SUCCESS("\t-> Loaded [%u] out of [%u] uniform names.", counter, uniformsAmount);
			} else {
				ANGE_ERROR("\t-> Loaded [%u] out of [%u] uniform names.", counter, uniformsAmount);
			}
		}
	}


	std::map<const char*, GLuint>* ShaderManager::GetShaderData(const char* shaderName)
	{
		if (shaderName == nullptr) {
			std::string message = "[ShaderManager GetShaderData] A 'nullptr' was passed instead of a valid shader name.";
			ANGE_FATAL(message.c_str());
			throw std::runtime_error(message);
		}
		auto it = m_Programs.find(shaderName);
		if (it != m_Programs.end()) {
			return &it->second;
		}
		ANGE_ERROR("ShaderLoader -> GetShaderData -> The shader can not be found. [%s]", shaderName);
		return nullptr;
	}


	GLuint ShaderManager::CompileShader(const char* name, const char* vertexShader, const char* fragmentShader)
	{
		if (name == nullptr || vertexShader==nullptr || fragmentShader == nullptr) {
			std::string message = "[ShaderManager AddShader] A 'nullptr' was passed as 'name' or 'vertexShader' or 'fragmentShader'.";
			ANGE_FATAL(message.c_str());
			throw std::runtime_error(message);
		}

		// Create the shaders
		GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

		GLint Result = false;
		int InfoLogLength;

		// Compile Vertex Shader

		char const * VertexSourcePointer = vertexShader;
		glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
		glCompileShader(VertexShaderID);

		// Check Vertex Shader
		glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
		glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if (InfoLogLength > 1) {
			std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
			glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
			ANGE_ERROR("Shader [%s] compiling error [Vertex]: %s", name, &VertexShaderErrorMessage[0]);
		}

		// Compile Fragment Shader
		char const * FragmentSourcePointer = fragmentShader;
		glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
		glCompileShader(FragmentShaderID);

		// Check Fragment Shader
		glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
		glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if (InfoLogLength > 1) {
			std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
			glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
			ANGE_ERROR("Shader [%s] compiling error [Fragment]: %s", name, &FragmentShaderErrorMessage[0]);
		}

		// Link the program
		GLuint ProgramID = glCreateProgram();
		glAttachShader(ProgramID, VertexShaderID);
		glAttachShader(ProgramID, FragmentShaderID);
		glLinkProgram(ProgramID);

		// Check the program
		glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
		glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if (InfoLogLength > 1) {
			std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
			glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
			ANGE_ERROR("Shader [%s] compiling error: %s", name, &ProgramErrorMessage[0]);
		} else {
			ANGE_SUCCESS("Shader [%s] compiled.", name);
		}

		glDetachShader(ProgramID, VertexShaderID);
		glDetachShader(ProgramID, FragmentShaderID);

		glDeleteShader(VertexShaderID);
		glDeleteShader(FragmentShaderID);

		return ProgramID;
	}
	
}
