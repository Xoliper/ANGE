#pragma once

#include <string>

namespace Ange {
	namespace Shader {
		namespace Quad {

			//--------------------------------------------------------
			//Shader definition
			//--------------------------------------------------------

			static const char* Name = "Quad";

			static const int UniformsAmount = 6;

			namespace Uniforms {
				static const char* MVP			= "MVP";
				static const char* Tint			= "RectColor";
				static const char* BorderColor	= "BorderColor";
				static const char* Anchor		= "AnchorVec";
				static const char* ChildPos		= "ChildPosVec";
				static const char* BorderData	= "BorderData";
			}

			static const std::string Vertex = R"(

				#version 330 core

				layout(location = 0) in vec3 vertexPosition;
	
				out vec3 vertexPos;

				uniform mat4 MVP;
				uniform vec3 AnchorVec;
				uniform vec3 ChildPosVec;

				void main(void)
				{
					gl_Position = MVP * vec4(vertexPosition+AnchorVec+ChildPosVec, 1.0);
					vertexPos = vertexPosition;
				}

			)";


			//Kolor 1, Kolor 2(border), BorderSize, T

			static const std::string Fragment = R"(

				#version 330 core
	
				precision highp float;
				precision highp int;

				in vec3 vertexPos;
				out vec4 color;

				uniform vec4 RectColor;
				uniform vec4 BorderColor;
				uniform vec4 BorderData;
				uniform vec3 AnchorVec;
				uniform vec3 ChildPosVec;

				float EPSILON = 0.001; 
				float clamp = 1.0;

				void main(void)
				{


					if(
					  abs(vertexPos.x - BorderData.x) >= (BorderData.z+EPSILON)  && abs(vertexPos.x + BorderData.x) > (BorderData.z+EPSILON) &&
					  abs(vertexPos.y - BorderData.y) >= (BorderData.w+EPSILON) && abs(vertexPos.y + BorderData.y) > (BorderData.w+EPSILON)
					){	
						color = RectColor*clamp;
					} else {
						color = BorderColor*clamp;
					}

				}

			)";
		}
	}
}
