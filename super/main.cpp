#ifdef _DEBUG
#undef _NDEBUG
#undef NDEBUG
#else
#define _NDEBUG
#define NDEBUG
#endif

#include "stdafx.h"

#include "../lib/mylib.hpp"
#include "../lib/camera.hpp"

#include <epoxy/gl.h>
#ifdef _WIN32
#include <epoxy/wgl.h>
#else
#include <epoxy/glx.h>
#endif

#include <SOIL.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include <iostream>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cassert>

int main(int argc, char **argv)
{
	try
	{
		mylib::Window window;

	start:
		std::cout << "(1) MultiLightApp \n"
			<< std::endl;
		unsigned int option{ 0 };
		std::cin >> option;

		switch (option)
		{
		default:
			std::cout << "select any one of the option please." << std::endl;
			goto start;
			break;
		}
	}
	catch (const std::exception& e)
	{
		throw e;
	}

	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::cin.get();
	std::exit(EXIT_SUCCESS);
}
