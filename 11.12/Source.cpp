#include <iostream>
#include <utility>

using namespace std;

// Реализация класса MyUniquePtr
template <typename T>
class MyUniquePtr 
{
private:
    T* ptr;

public:
    explicit MyUniquePtr(T* p = nullptr) : ptr(p) {}

    MyUniquePtr(MyUniquePtr&& other) noexcept : ptr(other.ptr) 
    {
        other.ptr = nullptr;
    }

    MyUniquePtr& operator=(MyUniquePtr&& other) noexcept 
    {
        if (this != &other) 
        {
            delete ptr;
            ptr = other.ptr;
            other.ptr = nullptr;
        }
        return *this;
    }

    ~MyUniquePtr() 
    {
        delete ptr;
    }

    T& operator*() const { return *ptr; }
    T* operator->() const { return ptr; }

    MyUniquePtr(const MyUniquePtr&) = delete;
    MyUniquePtr& operator=(const MyUniquePtr&) = delete;
};

template <typename T>
class MySharedPtr
{
private:
    T* ptr;
    int* count;

public:
    explicit MySharedPtr(T* p = nullptr) : ptr(p), count(new int(1)) {}

    MySharedPtr(const MySharedPtr& other) : ptr(other.ptr), count(other.count) 
    {
        ++(*count);
    }

    // Оператор присваивания
    MySharedPtr& operator=(const MySharedPtr& other) 
    {
        if (this != &other)
        {
            release();
            ptr = other.ptr;
            count = other.count;
            ++(*count);
        }
        return *this;
    }

    ~MySharedPtr() 
    {
        release();
    }

    void release() 
    {
        if (--(*count) == 0) 
        {
            delete ptr;
            delete count;
        }
    }

    T& operator*() const { return *ptr; }
    T* operator->() const { return ptr; }

    int getCount() const
    {
        return *count;
    }
};

void testUniquePtr() 
{
    MyUniquePtr<int> ptr1(new int(10));
    cout << "UniquePtr Value: " << *ptr1 << endl;

    MyUniquePtr<int> ptr2 = move(ptr1);
    cout << "Moved UniquePtr Value: " << *ptr2 << endl;
}

void testSharedPtr() 
{
    MySharedPtr<int> ptr1(new int(20));
    MySharedPtr<int> ptr2 = ptr1;
    cout << "SharedPtr Value: " << *ptr1 << " | Count: " << ptr1.getCount() << endl;
    cout << "SharedPtr Value: " << *ptr2 << " | Count: " << ptr2.getCount() << endl;
}

int main() 
{
    testUniquePtr();
    testSharedPtr();
    return 0;
}
