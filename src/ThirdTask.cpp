#include "ThirdTask.h"

namespace
{

constexpr auto LIST_NODE_PTR_SIZE = sizeof(ListNode*);
constexpr auto CHAR_SIZE = sizeof(char);

template <typename T>
void WriteBits(T original, FILE* file)
{
    //���������� � �������� ������� ���������� original � file

    const auto numBits = sizeof(T) * 8;
    const auto bitset = std::bitset<numBits>(original);
    const auto str = bitset.to_string();
    fwrite(str.c_str(), CHAR_SIZE, numBits, file);
}

template <typename T>
T ReadBits(FILE* file)
{
    //���������� �� ��������� ����� ���������� T

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

    //���������� ���������� ��������� � list
    WriteBits<int>(count, file);

    for (auto currentPtr = head; currentPtr; currentPtr = currentPtr->next)
    {
        //���������� ������ ������ data
        const auto dataSize = currentPtr->data.size();
        WriteBits<size_t>(dataSize, file); //�� ����������� '\0'

        //���������� data
        for (const auto& symbol : currentPtr->data)
        {
            WriteBits<char>(symbol, file);
        }

        //���������� ��������� �� rand
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

    //��������� ���������� ��������� list
    const auto elemCount = ReadBits<int>(file);

    //��������� �� ���� ������ �� �������
    std::vector<ListNode*> nodes;
    //��������������� ���� ����� ������� rand
    std::vector<size_t> randIdxs;

    //���������, ���� �� �������� �� ���� ���������
    for (size_t i = 0; i < elemCount; ++i)
    {
        //��������� ���������� �������� � data
        const auto dataSize = ReadBits<size_t>(file);

        //C-style ������ ��� data
        auto cStr = new char[dataSize + 1]; //������� '\0', ��� ��������� ��� ������������ std::string
        for (size_t j = 0; j < dataSize; ++j)
        {
            cStr[j] = ReadBits<char>(file);
        }
        cStr[dataSize] = '\0';

        //��������� ���� � list, �������� �� �����
        const auto nodePtr = PushBack(cStr);
        //��������� ����� � nodes
        nodes.emplace_back(nodePtr);

        //��������� � randIdxs �������
        randIdxs.emplace_back(ReadBits<size_t>(file));
    }

    //������������ ������ rand �����. randIdxs � nodes ����� ���������� ������
    for (size_t i = 0; i < randIdxs.size(); ++i)
    {
        //���������� ���������� � 1. ���� �������� nullptr - � ���� ������ ������ �� ������
        const auto randIdx = randIdxs[i];
        if (randIdx)
        {
            nodes[i]->rand = nodes[randIdx - 1];
        }
    }
}

ListNode* List::PushBack(const std::string& data)
{
    //���������� � ����� ������

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
    //���������� � ������ ������

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
    //������� �� ������� idx

    if (idx >= count)
    {
        throw(std::runtime_error("#Insert: Index is greater than list count"));
        return nullptr;
    }

    //������� ������, ��� ����� ������������ PushBack � PushFront
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
    

    //���� ����, ������� ��������� ������ �� idx
    auto idxNodePtr = head;
    for (size_t i = 0; i < idx; ++i)
    {
        idxNodePtr = idxNodePtr->next;
    }

    //������� ����� ����
    auto nodePtr = new ListNode;
    nodePtr->data = data;
    nodePtr->prev = idxNodePtr->prev;
    nodePtr->next = idxNodePtr;

    //��������� next � ����, ������� ���� ����� idxNode
    idxNodePtr->prev->next = nodePtr;

    //��������� prev � idxNode
    idxNodePtr->prev = nodePtr;
    ++count;

    return nodePtr;
}

size_t List::GetPosition(ListNode* nodePtr) const
{
    //���������� ������� � list (������ + 1) ������ ����. ����, ���� ���� nullptr
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