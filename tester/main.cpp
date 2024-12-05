#include <array>
#include <iostream>
#include <optional>
#include <queue>
#include <string>
#include <vector>

#include <tbs/compile_time_function.h>
#include <tbs/constexpr_value.h>
#include <tbs/containers/CircleQueue.h>
#include <tbs/containers/iterator/Iteratable.h>
#include <tbs/time_utils.hpp>


int main()
{
    CircleQueue<int, 3> c;
    c.push(1);
    c.push(2);
    c.push(3);
    c.pop();
    c.push(4);
    for (auto& elem : c)
    {
        std::cout << elem << std::endl;
    }

    return 0;
}
