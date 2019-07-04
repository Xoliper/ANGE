#pragma once

#include <string>
#include <map>
#include <glad/glad.h>
//#include <glload/gl_3_3.hpp>
//#include <glload/gl_load.hpp>

namespace Ange {

	//-----------------------------------------------------------------------
	//Classes
	//-----------------------------------------------------------------------

	/*!
	The shader management class. In addition to loading standard shaders, it also allows you to load
	external programs.
	*/
	class ShaderManager
	{
	public:

		/*!
		Default constructor - loads programs(shaders).
		*/
		ShaderManager();

		/*!
		Copy constructor is currently unavailable.
		*/
		ShaderManager(const ShaderManager & copy) = delete;
		
		/*!
		Destructor - removes loaded programs(shaders).
		*/
		~ShaderManager();
	
		/*!
		Loads the shader and stores it under the given name.
		*/
		void AddShader(const char* shaderName, const std::string vertexShader, const std::string fragmentShader);
		
		/*!
		Loads uniforms for the given shader.
		*/
		void LoadUniform(const char* shaderName, int uniformsAmount, const char* uniformName ...);
		
		/*!
		Allows access to shader data.
		*/
		std::map<const char*, GLuint>* GetShaderData(const char* shaderName);

	private:

		/*!
		Compiles the OpenGL shader.
		*/
		GLuint CompileShader(const char* name, const char* vertexShader, const char* fragmentShader);

		/*Containing data of loaded shaders - their names, identifiers and uniform identifiers.*/
		std::map<const char*, std::map<const char*, GLuint> > m_Programs;

		/*It secures(prevent) the re-initialization of shaders.*/
		bool m_ShadersInitialized;
	};
}
