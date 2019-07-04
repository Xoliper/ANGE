#pragma once

#include <string>

namespace Ange {
	namespace Shader {
		namespace Text {

			//--------------------------------------------------------
			//Shader definition
			//--------------------------------------------------------

			static const char* Name = "Text";

			static const int UniformsAmount = 7;

			namespace Uniforms {
				static const char* MVP			= "MVP";
				static const char* Tint			= "TextColor";
				static const char* DiffuseTex	= "DiffuseTexture";
				static const char* Anchor		= "AnchorVec";
				static const char* ChildPos		= "ChildPosVec";
				static const char* ClipArea		= "ClipArea";
				static const char* DispOffset	= "DispOffset";
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
				uniform vec4 ClipArea;
				uniform vec2 DispOffset;

				void main(void)
				{
					vec3 temp = vertexPosition+AnchorVec+ChildPosVec + vec3(ClipArea.x, ClipArea.y, 0.0f) + vec3(DispOffset.xy, 0.0f);
					gl_Position = MVP * vec4(temp.xyz, 1.0f);

					vertexPos = vertexPosition + vec3(DispOffset.xy, 0.0f);
					UV = vertexUV;
				}

			)";


			//Kolor 1, Kolor 2(border), BorderSize, T

			static const std::string Fragment = R"(

				#version 330 core

				in vec3 vertexPos;
				in vec2 UV;
				out vec4 color;

				uniform vec4 TextColor;
				uniform vec4 ClipArea;
				uniform sampler2D DiffuseTexture;
				
				float clamp = 1.0f;

				void main(void)
				{
																						// || vertexPos.x <= ClipArea.x/2
					if(abs(vertexPos.x) > ClipArea.z || abs(vertexPos.y) > ClipArea.w || vertexPos.x < 0){
						clamp = 0.0;
					}
					color = vec4(1.0f, 1.0f, 1.0f, texture(DiffuseTexture, UV, -10.0f).r) * TextColor * clamp;
				}

			)";
		}
	}
}
