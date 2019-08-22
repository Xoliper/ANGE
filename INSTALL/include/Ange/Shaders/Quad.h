#pragma once

#include <string>

namespace Ange {
	namespace Shader {
		namespace Quad {

			//--------------------------------------------------------
			//Shader definition
			//--------------------------------------------------------

			static const char* Name = "Quad";

			static const int UniformsAmount = 8;

			namespace Uniforms {
				static const char* MVP			= "MVP";
				static const char* Tint			= "RectColor";
				static const char* BorderColor	= "BorderColor";
				static const char* Anchor		= "AnchorVec";
				static const char* ChildPos		= "ChildPosVec";
				static const char* BorderData	= "BorderData";
				static const char* Center		= "Center";
				static const char* Radiuses		= "Radiuses";
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
				uniform vec2 Center;
				uniform vec4 Radiuses;

				float clamp = 1.0;

				float min(float first, float second)
				{
					if(first > second) return second;
					return first;
				}

				void main(void)
				{

					//----------------------------------------------
					//Base color
					//----------------------------------------------
					vec4 final = vec4(0.0f, 0.0f, 0.0f, 0.0f);

					//----------------------------------------------
					//Draw normal rectangle with border
					//----------------------------------------------
					if(
					  abs(vertexPos.x - BorderData.x) >= BorderData.z  && abs(vertexPos.x + BorderData.x) > BorderData.z &&
					  abs(vertexPos.y - BorderData.y) >= BorderData.w && abs(vertexPos.y + BorderData.y) > BorderData.w
					){	
						final = RectColor*clamp;
					} else {
						final = BorderColor*clamp;
					}

					//----------------------------------------------
					//Add corners
					//----------------------------------------------

				//------------------------------------------------------------------

					vec2 lb = vec2(Radiuses.x, Radiuses.x) - Center;
					//Erase parts outside circle
					if(distance(vertexPos.xy, lb) >= Radiuses.x && vertexPos.x <= lb.x && vertexPos.y <= lb.y){
						//Add aliasing
						float val = 1.0f-(distance(vertexPos.xy, lb) - Radiuses.x); 
						final = vec4(1.0f, 1.0f, 1.0f, val)* BorderColor;
					}
					
					//Fix border
					if(distance(vertexPos.xy, lb) <= Radiuses.x && vertexPos.x <= lb.x && vertexPos.y <= lb.y){
						if(distance(vertexPos.xy, lb) > Radiuses.x-min(BorderData.z, BorderData.w)*2){
							float val = (distance(vertexPos.xy, lb) - (Radiuses.x-min(BorderData.z, BorderData.w)*2));
							if(val < 0.5f) val = 0.5f;
							final = vec4(BorderColor.xyz, val);
						}
					}


				//----------------------------------------------------------------

					vec2 rt = vec2(-Radiuses.z, -Radiuses.z) + Center;
					//Erase parts outside circle
					if(distance(vertexPos.xy, rt) >= Radiuses.z && vertexPos.x >= rt.x && vertexPos.y >= rt.y){
						//Add aliasing
						float val = 1.0f-(distance(vertexPos.xy, rt) - Radiuses.z); 
						final = vec4(1.0f, 1.0f, 1.0f, val)* BorderColor;
					}
					
					//Fix border
					if(distance(vertexPos.xy, rt) <= Radiuses.z && vertexPos.x >= rt.x && vertexPos.y >= rt.y){
						if(distance(vertexPos.xy, rt) > Radiuses.z-min(BorderData.z, BorderData.w)*2){
							float val = (distance(vertexPos.xy, rt) - (Radiuses.z-min(BorderData.z, BorderData.w)*2));
							if(val < 0.5f) val = 0.5f;
							final = vec4(BorderColor.xyz, val);
						}
					}

				//----------------------------------------------------------------

					vec2 lt = vec2(Radiuses.y, -Radiuses.y) + vec2(-Center.x, Center.y);
					//Erase parts outside circle
					if(distance(vertexPos.xy, lt) >= Radiuses.y && vertexPos.x <= lt.x && vertexPos.y >= lt.y){
						//Add aliasing
						float val = 1.0f-(distance(vertexPos.xy, lt) - Radiuses.y); 
						final = vec4(1.0f, 1.0f, 1.0f, val)* BorderColor;
					}
					
					//Fix border
					if(distance(vertexPos.xy, lt) <= Radiuses.y && vertexPos.x <= lt.x && vertexPos.y >= lt.y){
						if(distance(vertexPos.xy, lt) > Radiuses.y-min(BorderData.z, BorderData.w)*2){
							float val = (distance(vertexPos.xy, lt) - (Radiuses.y-min(BorderData.z, BorderData.w)*2));
							if(val < 0.5f) val = 0.5f;
							final = vec4(BorderColor.xyz, val);
						}
					}

				//----------------------------------------------------------------

					vec2 rb = vec2(-Radiuses.w, Radiuses.w) + vec2(Center.x, -Center.y);;
					//Erase parts outside circle
					if(distance(vertexPos.xy, rb) >= Radiuses.w && vertexPos.x >= rb.x && vertexPos.y <= rb.y){
						//Add aliasing
						float val = 1.0f-(distance(vertexPos.xy, rb) - Radiuses.w); 
						final = vec4(1.0f, 1.0f, 1.0f, val)* BorderColor;
					}
					
					//Fix border
					if(distance(vertexPos.xy, rb) <= Radiuses.w && vertexPos.x >= rb.x && vertexPos.y <= rb.y){
						if(distance(vertexPos.xy, rb) > Radiuses.w-min(BorderData.z, BorderData.w)*2){
							float val = (distance(vertexPos.xy, rb) - (Radiuses.w-min(BorderData.z, BorderData.w)*2));
							if(val < 0.5f) val = 0.5f;
							final = vec4(BorderColor.xyz, val);
						}
					}

				//----------------------------------------------------------------

					color = final;

				}

			)";
		}
	}
}
