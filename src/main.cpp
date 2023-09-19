#include "FirstTask.h"
#include "SecondTask.h"
#include "ThirdTask.h"

//Кучинаев Ахмат Муратович
//13.09, потратил примерно 7ч

int main()
{
    std::cout << '0';

    //демонстрация первой задачи
    PrintBits(487); //111100111
    std::cout << "\n";

    //вторая задача
    char str[] = "AAA BBB   AAA";
    RemoveDups(str);
    printf("%s\n", str);
    std::cout << "\n";

    
    //третья задача
    List myList;

    try
    {
        myList.PushBack("third");
        myList.PushBack("fourth");
        myList.PushFront("first");
        myList.Insert(1, "second");
        myList[1]->rand = myList[3]; //second->rand == fourth

        auto currentPtr = myList.GetHead();
        auto count = myList.GetCount();

        //убеждаемся, что очередность корректная и данные корректны
        for (size_t i = 0; i < count; ++i)
        {
            std::cout << "data: " << currentPtr->data;
            if (currentPtr->rand)
            {
                std::cout << " rand data: " << currentPtr->rand->data;
            }
            std::cout << "\n";
            currentPtr = currentPtr->next;
        }
        std::cout << std::endl;

        //Убеждаемся, что после сериилизации/десериализации список остался неизменным
        FILE* filePtr;
        fopen_s(&filePtr, "test.txt", "wb");
        myList.Serialize(filePtr);
        fclose(filePtr);

        //Чистим список и загружаемся
        List loadList;
        fopen_s(&filePtr, "test.txt", "rb");
        loadList.Deserialize(filePtr);

        //еще раз проверяем очередность
        currentPtr = loadList.GetHead();
        for (size_t i = 0; i < count; ++i)
        {
            std::cout << "data: " << currentPtr->data;
            if (currentPtr->rand)
            {
                std::cout << " rand data: " << currentPtr->rand->data;
            }
            std::cout << "\n";
            currentPtr = currentPtr->next;
        }
        fclose(filePtr);
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
        return -1;
    }

    return 0;
}