#pragma once

#include <string>

namespace Ange {
	namespace Shader {
		namespace Image {

			//--------------------------------------------------------
			//Shader definition
			//--------------------------------------------------------

			static const char* Name = "Image";

			static const int UniformsAmount = 7;

			namespace Uniforms {
				static const char* MVP			= "MVP";
				static const char* Tint			= "SpriteColor";
				static const char* DiffuseTex	= "DiffuseTexture";
				static const char* BorderColor	= "BorderColor";
				static const char* Anchor		= "AnchorVec";
				static const char* ChildPos		= "ChildPosVec";
				static const char* BorderData	= "BorderData";
			}

			static const std::string Vertex = R"(

				#version 330 core

				layout(location = 0) in vec3 vertexPosition;
				layout(location = 1) in vec2 vertexUV;
				
				out vec3 vertexPos;
				out vec2 UV;

				uniform mat4 MVP;
				uniform vec3 AnchorVec;
				uniform vec3 ChildPosVec;

				void main(void)
				{
					gl_Position = MVP * vec4(vertexPosition+AnchorVec+ChildPosVec, 1.0);
					vertexPos = vertexPosition;
					UV = vertexUV;
				}

			)";

			static const std::string Fragment = R"(

				#version 330 core
	
				precision highp float;
				precision highp int;

				in vec3 vertexPos;
				in vec2 UV;
				out vec4 color;

				uniform vec4 SpriteColor;
				uniform vec4 BorderColor;
				uniform vec4 BorderData;
				uniform sampler2D DiffuseTexture;

				float EPSILON = 0.001; 

				void main(void)
				{

					if(
					  abs(vertexPos.x - BorderData.x) >= (BorderData.z+EPSILON)  && abs(vertexPos.x + BorderData.x) > (BorderData.z+EPSILON) &&
					  abs(vertexPos.y - BorderData.y) >= (BorderData.w+EPSILON) && abs(vertexPos.y + BorderData.y) > (BorderData.w+EPSILON)
					){	
						color = texture(DiffuseTexture, UV) * SpriteColor;
					} else {
						color = BorderColor;
					}

				}

			)";
		}
	}
}
