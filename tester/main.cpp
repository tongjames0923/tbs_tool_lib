#include <array>
#include <iostream>
#include <optional>
#include <queue>
#include <string>
#include <vector>

#include <string>
#include <tbs/Option.h>
#include <tbs/compile_time_function.h>
#include <tbs/constexpr_value.h>
#include <tbs/containers/CircleQueue.h>
#include <tbs/containers/iterator/Iteratable.h>
#include <tbs/match/match_macro.h>
#include <tbs/time_utils.hpp>
auto test()
{
    int a[] = {1, 2, 3, 4};
    return SOME_OPTION(a);
}
#include <tbs/log/log.hpp>

int main()
{
    std::cout << LOG_FORMAT("hello world{}", 1) << std::endl;
    return 0;
}
