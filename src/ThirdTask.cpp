#include "ThirdTask.h"

namespace
{

constexpr auto LIST_NODE_PTR_SIZE = sizeof(ListNode*);
constexpr auto CHAR_SIZE = sizeof(char);

template <typename T>
void WriteBits(T original, FILE* file)
{
    //Записывает в бинарном формате переменную original в file

    const auto numBits = sizeof(T) * 8;
    const auto bitset = std::bitset<numBits>(original);
    const auto str = bitset.to_string();
    fwrite(str.c_str(), CHAR_SIZE, numBits, file);
}

template <typename T>
T ReadBits(FILE* file)
{
    //Вычитывает из бинарного файла переменную T

    const auto numBits = sizeof(T) * 8;
    char str[numBits + 1];
    fread(str, CHAR_SIZE, numBits, file);
    str[numBits] = '\0';
    
    const auto bitset = std::bitset<numBits>(str);
    return static_cast<T>(bitset.to_ullong());
}

}

List::~List()
{
    auto currentPtr = head;
    for (size_t i = 0; i < count; ++i)
    {
        auto nextPtr = currentPtr->next;
        delete currentPtr;
        currentPtr = nextPtr;
    }
}

void List::Serialize(FILE* file)
{
    if (!file)
    {
        throw(std::runtime_error("#List::Serialize: File was not opened"));
        return;
    }

    //Записываем количество элементов в list
    WriteBits<int>(count, file);

    for (auto currentPtr = head; currentPtr; currentPtr = currentPtr->next)
    {
        //Записываем размер строки data
        const auto dataSize = currentPtr->data.size();
        WriteBits<size_t>(dataSize, file); //не учитывается '\0'

        //Записываем data
        for (const auto& symbol : currentPtr->data)
        {
            WriteBits<char>(symbol, file);
        }

        //Записываем указатель на rand
        WriteBits<size_t>(GetPosition(currentPtr->rand), file);
    }
}

void List::Deserialize(FILE* file)
{
    if (!file)
    {
        throw(std::runtime_error("#List::Deserialize: File was not opened"));
        return;
    }

    //Считываем количество элементов list
    const auto elemCount = ReadBits<int>(file);

    //Указатели на ноды строго по порядку
    std::vector<ListNode*> nodes;
    //Соответствующие этим нодам позиции rand
    std::vector<size_t> randIdxs;

    //Считываем, пока не прошлись по всем элементам
    for (size_t i = 0; i < elemCount; ++i)
    {
        //Считываем количество символов в data
        const auto dataSize = ReadBits<size_t>(file);

        //C-style строка для data
        auto cStr = new char[dataSize + 1]; //добавим '\0', это небходимо для конструктора std::string
        for (size_t j = 0; j < dataSize; ++j)
        {
            cStr[j] = ReadBits<char>(file);
        }
        cStr[dataSize] = '\0';

        //Добавляем ноду в list, получаем ее адрес
        const auto nodePtr = PushBack(cStr);
        //Добавляем адрес в nodes
        nodes.emplace_back(nodePtr);

        //Добавляем в randIdxs позицию
        randIdxs.emplace_back(ReadBits<size_t>(file));
    }

    //Сопоставляем адреса rand нодам. randIdxs и nodes имеют одинаковый размер
    for (size_t i = 0; i < randIdxs.size(); ++i)
    {
        //Индексация начинается с 1. Ноль означает nullptr - в этом случае ничего не делаем
        const auto randIdx = randIdxs[i];
        if (randIdx)
        {
            nodes[i]->rand = nodes[randIdx - 1];
        }
    }
}

ListNode* List::PushBack(const std::string& data)
{
    //Добавление в конец списка

    auto nodePtr = new ListNode;
    nodePtr->data = data;

    if (!head)
    {
        head = nodePtr;
    }

    if (tail)
    {
        tail->next = nodePtr;
        nodePtr->prev = tail;
    }
    tail = nodePtr;
    ++count;

    return nodePtr;
}

ListNode* List::PushFront(const std::string& data)
{
    //Добавление в начало списка

    auto nodePtr = new ListNode;
    nodePtr->data = data;

    if (!tail)
    {
        tail = nodePtr;
    }

    if (head)
    {
        head->prev = nodePtr;
        nodePtr->next = head;
    }
    head = nodePtr;
    ++count;

    return nodePtr;
}

ListNode* List::Insert(const size_t idx, const std::string& data)
{
    //Вставка по индексу idx

    if (idx >= count)
    {
        throw(std::runtime_error("#Insert: Index is greater than list count"));
        return nullptr;
    }

    //Простые случаи, где можно ограничиться PushBack и PushFront
    if (idx == 0)
    {
        return PushFront(data);
    }
    else
    {
        if (idx == count - 1)
        {
            return PushBack(data);
        }
    }
    

    //Ищем ноду, которая находится сейчас по idx
    auto idxNodePtr = head;
    for (size_t i = 0; i < idx; ++i)
    {
        idxNodePtr = idxNodePtr->next;
    }

    //Создаем новую ноду
    auto nodePtr = new ListNode;
    nodePtr->data = data;
    nodePtr->prev = idxNodePtr->prev;
    nodePtr->next = idxNodePtr;

    //Обновляем next у ноды, которая была перед idxNode
    idxNodePtr->prev->next = nodePtr;

    //Обновляем prev у idxNode
    idxNodePtr->prev = nodePtr;
    ++count;

    return nodePtr;
}

size_t List::GetPosition(ListNode* nodePtr) const
{
    //Возвращает позицию в list (индекс + 1) данной ноды. Ноль, если ищем nullptr
    if (!nodePtr)
    {
        return 0;
    }

    size_t idx = 1;
    for (auto currentPtr = head; currentPtr; currentPtr = currentPtr->next)
    {
        if (currentPtr == nodePtr)
        {
            return idx;
        }
        ++idx;
    }
    throw (std::runtime_error("#GetPosition: nodePtr not in list"));
}