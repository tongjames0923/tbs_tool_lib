#include <array>
#include <iostream>
#include <optional>
#include <string>

#include <tbs/constexpr_value.h>


int main()
{
    using namespace std;
    auto m = make_ConstexprValue(1);
    cout << m.get() << endl;
    return 0;
}
