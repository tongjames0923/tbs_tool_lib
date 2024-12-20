#include <array>
#include <iostream>
#include <optional>
#include <queue>
#include <string>
#include <vector>

#include <tbs/Option.h>
#include <tbs/compile_time_function.h>
#include <tbs/constexpr_value.h>
#include <tbs/containers/CircleQueue.h>
#include <tbs/containers/iterator/Iteratable.h>
#include <tbs/match/match_macro.h>
#include <tbs/time_utils.hpp>

Option<int> test()
{
    return SOME_OPTION(3);
}


int main()
{

    auto k = match(test(), EQUAL_MATCH(SOME_OPTION(3), return SOME_OPTION(8);), []() { return SOME_OPTION(6); });
    matchWithoutReturn(test(),
                       EQUAL_MATCH(SOME_OPTION(3), std::cout << "3" << std::endl;),
                       EQUAL_MATCH(SOME_OPTION(4), std::cout << "4" << std::endl;),
                       []() { std::cout << "none" << std::endl; });
    NO_RETURNED_MATCH(test(), EQUAL_MATCH(SOME_OPTION(3), std::cout << "3" << std::endl;), );
    return 0;
}
