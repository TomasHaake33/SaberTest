#ifndef THIRD_TASK_H_
#define THIRD_TASK_H_

#include <fstream>
#include <string>
#include <bitset>
#include <vector>

struct ListNode
{
    ListNode* prev = nullptr;
    ListNode* next = nullptr;
    ListNode* rand = nullptr; 
    std::string data; 
};

class List
{
public:
    List() = default;
    ~List();

    void Serialize(FILE* file); 
    void Deserialize(FILE* file); 

    ListNode* PushBack(const std::string& data);
    ListNode* PushFront(const std::string& data);
    ListNode* Insert(const size_t idx, const std::string& data);  

    //функции просто для main, их можно убрать
    ListNode* GetHead()
    {
        return head;
    }
    int GetCount()
    {
        return count;
    }
    ListNode* operator[](size_t idx)
    {
        auto currentPtr = head;
        for (size_t i = 0; i < idx; ++i)
        {
            currentPtr = currentPtr->next;
        }
        return currentPtr;
    }

private:
    size_t GetPosition(ListNode* nodePtr) const;

private:
    ListNode* head = nullptr;
    ListNode* tail = nullptr;
    int count = 0;
};


#endif //THIRD_TASK_H_
