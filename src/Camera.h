#pragma once

#include <glm/glm.hpp>
using namespace glm;

//class WorldCamera
//{
//	public:
//		/* Variables */
//		int screenWidth, screenHeight;
//
//		vec3 position;
//		vec3 rotation;
//
//		float fieldOfView;
//
//		/* Methods */
//		WorldCamera(int screenWidth, int screenHeight, vec3 position, vec3 rotation);
//
//		vec3 rotationRadians() { return vec3(rotation.x * 3.1415926536 / 180.0, rotation.y * 3.1415926536 / 180.0, rotation.z * 3.1415926536 / 180.0); };
//
//		char* CalculateFrame();
//
//	private:
//		/* Methods */
//		void ImageColor(int x, int y, int& width, vec3 color, char* image);
//};