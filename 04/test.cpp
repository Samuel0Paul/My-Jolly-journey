#define BOOST_TEST_MODULE 04 test 1
#include <boost/test/included/unit_test.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <utility>

BOOST_AUTO_TEST_CASE(first_test)
{
    glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
    glm::mat4 trans;
    trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
    vec = trans * vec;
    BOOST_TEST(vec.x == 2.0f);
    BOOST_TEST(vec.y == 1.0f);
    BOOST_TEST(vec.z == 0.0f);

    trans = glm::rotate(trans, 90.0f, glm::vec3(0.0, 0.0, 1.0));
    std::cout << glm::to_string(trans);
    trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
    vec = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    vec = trans * vec;
    BOOST_TEST(vec.x == 0.775963187f);
    BOOST_TEST(vec.y == 1.44699836f);
    BOOST_TEST(vec.z == 0.0f);
}
