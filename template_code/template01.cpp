#undef _NDEBUG

#include "../lib/mylib.hpp"

#include <epoxy/gl.h>
#include <epoxy/glx.h>

#include <SOIL/SOIL.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include <iostream>
#include <vector>
#include <cstdio>
#include <cstdlib>

/*
*
*/

class myApp: public mylib::App
{
public:
    myApp(mylib::Window&& window):
        mylib::App(std::move(window)) {}

    void startup() override
    {

    }

    ~myApp()
    {

    }
    
    void update(double time) override
    {

    }

    void render() override
    {

    }

protected:

private:

};

int main(const int argc, const char* const argv[])
{

    std::exit(EXIT_SUCCESS);
}
