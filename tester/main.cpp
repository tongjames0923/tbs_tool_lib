#include <array>
#include <iostream>
#include <optional>
#include <string>

#include <tbs/compile_time_function.h>
#include <tbs/constexpr_value.h>

int main()
{
    using namespace std;
    constexpr auto i = COMPILE_TIME_IF(make_ConstexprValue({1, 2, 3, 4}) == make_ConstexprValue({1, 2, 5, 4}), []() { return; }, []() { return 68; });
    if constexpr (i.has_value())
    {
        cout << i.value() << endl;
    }
    else
    {
        cout << "no value" << endl;
    }

    return 0;
}
