#include "FirstTask.h"

namespace
{

constexpr auto INT_SIZE = sizeof(int) * 8;

}

void PrintBits(const int integer)
{
    for (size_t i = 1; i <= INT_SIZE; ++i)
    {
        //������� ����� ������ �� i-��
        const auto rightBits = INT_SIZE - i;

        //�������� ������� ����� integer, �������� �������� i-�� ����
        const auto bitValue = static_cast<char>((integer >> rightBits) & 1);
        printf("%d", bitValue);
    }
}
// ����� ��� �������������, ��� ����� �� ����������