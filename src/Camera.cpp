#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/norm.hpp>
//#include "RayMarcher.cpp"

#include <iostream>
#include <vector>
//#include "Color.cpp"

using namespace glm;

#ifndef WORLD_CAM_HEADER
#define WORLD_CAM_HEADER

class WorldCamera 
{
	public:
		/* Position and rotation */
		vec3 position;
		vec3 rotation;

		/* Camera settings */
		float fieldOfView;


	private:
		void ImageColor(int& x, int& y, int& width, vec3 color, char* &image)
		{
			image[x * 3 + y * (width * 3)] = '0';
			image[x * 3 + y * (width * 3) + 1] = '1';
			image[x * 3 + y * (width * 3) + 2] = '2';
		}

		vec3 rotationRadians()
		{
			return vec3(rotation.x * 3.1415926536 / 180.0, rotation.y * 3.1415926536 / 180.0, rotation.z * 3.1415926536 / 180.0);
		};


	public:
		WorldCamera(vec3 position, vec3 rotation)
		{
			this->position = position;
			this->rotation = rotation;
			fieldOfView = 90;
		}

		vec3 Forward() {
			vec3 rad = rotationRadians();

			float pitch = rotation.x;
			float yaw = rotation.y;
			float roll = rotation.z;

			float x = -cos(yaw) * sin(pitch) * sin(roll) - sin(yaw) * cos(roll);
			float y = -sin(yaw) * sin(pitch) * sin(roll) + cos(yaw) * cos(roll);
			float z = cos(pitch) * sin(roll);
			
			return vec3(x, y, z);
			//return vec3(sin(rad.y) * cos(rad.x), sin(rad.x), cos(rad.x) * cos(rad.y));
		}

		vec3 GetPosition() 
		{
			return position;
		}

		mat3x3 GetRotation() 
		{
			return toMat4(quat(rotationRadians()));
		}

		void Rotate(vec3 r) 
		{
			rotation += r;
		}

		/*void RotateY(float theta) 
		{
			mat3x3 rm =
			{
				{ cos(theta),  0.0, sin(theta) },
				{ 0.0,		   1.0, 0.0 },
				{ -sin(theta), 0.0, cos(theta) }
			};


		}*/

		//VoxelColor* CalculateFrame(int screenWidth, int screenHeight)
		//{
		//	/* Convert from euler angles to rotation matrix */
		//	mat3x3 m_rotation = toMat4(quat(rotationRadians()));

		//	/* Create the image variable */
		//	VoxelColor* image = new VoxelColor[screenWidth * screenHeight];

		//	vec2 renderResolution = vec2(4, 4);

		//	/* Loop over each pixel on the screen */
		//	for (int y = 0; y < screenHeight / renderResolution.y; y++)
		//	{
		//		for (int x = 0; x < screenWidth / renderResolution.x; x++)
		//		{
		//			/* Calculate the UV coordinates */
		//			vec2 uv = (vec2(x * renderResolution.x, y * renderResolution.y) - 0.5f * vec2(screenWidth, screenHeight)) / (float)screenHeight;

		//			/* Calculate the Ray Direction */
		//			vec3 rd = m_rotation * normalize(vec3(uv, 1.0));
		//			/* Calculate the Ray Origin */
		//			vec3 ro = position;

		//			/* Calculate the distance using ray marching */
		//			float d = RayMarch(ro, rd);

		//			/* Change the color on the image */
		//			for (int i = 0; i < renderResolution.y; i++)
		//			{
		//				for (int j = 0; j < renderResolution.x; j++)
		//				{
		//					image[(y + i) * screenWidth + (x + j)] = VoxelColor(d, d, d);
		//				}
		//			}
		//		}
		//	}

		//	//std::cout << image[0].r() << image[0].g() << image[0].b() << std::endl;

		//	return image;
		//}

		struct vec2i {
			int x;
			int y;

			vec2i(int x, int y) {
				this->x = x;
				this->y = y;
			}
		};

		//unsigned char* CalculateFrameC(const int screenWidth, const int screenHeight)
		//{
		//	/* Convert from euler angles to rotation matrix */
		//	mat3x3 m_rotation = toMat4(quat(rotationRadians()));

		//	/* Create the image variable */
		//	int imageSize = screenWidth * screenHeight * 3;
		//	unsigned char* image = new unsigned char[imageSize];

		//	vec2i renderResolution = vec2i(1, 1);

		//	/* Loop over each pixel on the screen */
		//	for (int xy = 0; xy < (screenWidth / renderResolution.x) * (screenHeight / renderResolution.y); xy++)
		//	{
		//		int x = xy % (screenWidth / renderResolution.x);
		//		int y = xy / (screenWidth / renderResolution.x);

		//		/* Calculate the UV coordinates */
		//		vec2 uv = (vec2(x * renderResolution.x, y * renderResolution.y) - 0.5f * vec2(screenWidth, screenHeight)) / (float)screenHeight;

		//		/* Calculate the Ray Direction */
		//		vec3 rd = m_rotation * normalize(vec3(uv, 1.0));
		//		/* Calculate the Ray Origin */
		//		vec3 ro = position;

		//		/* Calculate the distance using ray marching */
		//		float d = RayMarch(ro, rd);

		//		for (int i = 0; i < renderResolution.y; i++)
		//		{
		//			for (int j = 0; j < renderResolution.x; j++)
		//			{
		//				image[(xy + (i * screenWidth + j)) * 3 + 0] = d;
		//				image[(xy + (i * screenWidth + j)) * 3 + 1] = d;
		//				image[(xy + (i * screenWidth + j)) * 3 + 2] = d;
		//			}
		//		}
		//	}

		//	//for (int y = 0; y < screenHeight / renderResolution.y; y++)
		//	//{
		//	//	for (int x = 0; x < screenWidth / renderResolution.x; x++)
		//	//	{
		//	//		/* Calculate the UV coordinates */
		//	//		vec2 uv = (vec2(x * renderResolution.x, y * renderResolution.y) - 0.5f * vec2(screenWidth, screenHeight)) / (float)screenHeight;

		//	//		/* Calculate the Ray Direction */
		//	//		vec3 rd = m_rotation * normalize(vec3(uv, 1.0));
		//	//		/* Calculate the Ray Origin */
		//	//		vec3 ro = position;

		//	//		/* Calculate the distance using ray marching */
		//	//		float d = RayMarch(ro, rd);

		//	//		image[y * (screenWidth * 3) + x * 3 + 0] = d;
		//	//		image[y * (screenWidth * 3) + x * 3 + 1] = d;
		//	//		image[y * (screenWidth * 3) + x * 3 + 2] = d;

		//	//		//int arrayPosition = (y * renderResolution.y) * (screenWidth / renderResolution.x * 3) + x * renderResolution.x * 3;

		//	//		/* Change the color on the image */
		//	//		/*for (int i = 0; i < renderResolution.y; i++)
		//	//		{
		//	//			for (int j = 0; j < renderResolution.x; j++)
		//	//			{
		//	//				int renderPosition = arrayPosition + (i * (screenWidth / renderResolution.x * 3) + j * 3);

		//	//				image[renderPosition + 0] = d;
		//	//				image[renderPosition + 1] = d;
		//	//				image[renderPosition + 2] = d;
		//	//			}
		//	//		}*/
		//	//	}
		//	//}

		//	//std::cout << image[0].r() << image[0].g() << image[0].b() << std::endl;

		//	return image;
		//}
};

#endif