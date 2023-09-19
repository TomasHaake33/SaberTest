#include "FirstTask.h"

namespace
{

constexpr auto INT_SIZE = sizeof(int) * 8;

}

void PrintBits(const int integer)
{
    for (size_t i = 1; i <= INT_SIZE; ++i)
    {
        //—колько битов справа от i-го
        const auto rightBits = INT_SIZE - i;

        //—двигаем битовую маску integer, вы€сн€ем значение i-го бита
        const auto bitValue = static_cast<char>((integer >> rightBits) & 1);
        printf("%d", bitValue);
    }
}
// когда лев программирует, его лучше не перебивать