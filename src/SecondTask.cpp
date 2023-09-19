#include "SecondTask.h"

void RemoveDups(char* str)
{
    if (str == nullptr || str == '\0')
    {
        return;
    }

    const auto strSize = strlen(str);

    //Проходимся по str, начиная со второго символа. Индекс записи тоже показывает на второй символ.
    //Если элемент i не равен i-1му, то по writeIdx присваиваем этот символ и инкрементируем writeIdx
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