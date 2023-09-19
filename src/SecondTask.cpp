#include "SecondTask.h"

void RemoveDups(char* str)
{
    if (str == nullptr || str == '\0')
    {
        return;
    }

    const auto strSize = strlen(str);

    //���������� �� str, ������� �� ������� �������. ������ ������ ���� ���������� �� ������ ������.
    //���� ������� i �� ����� i-1��, �� �� writeIdx ����������� ���� ������ � �������������� writeIdx
    size_t writeIdx = 1;
    for (size_t i = 1; i < strSize; ++i)
    {
        const auto& current = str[i];
        if (current != str[i - 1])
        {
            str[writeIdx] = current;
            ++writeIdx;
        }
    }

    str[writeIdx] = '\0';
}