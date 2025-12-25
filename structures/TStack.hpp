#pragma once
#include <iostream>

template<typename T>
class TStack
{
private:
  int capacity;
  int size;
  T* memory;
public:
  TStack();
  TStack(const int capacity_);
  TStack(const TStack<T>& other);
  TStack(TStack<T>&& other);
  ~TStack();

  TStack& operator=(const TStack<T>& other);
  TStack& operator=(TStack<T>&& other);

  void Push(T value);
  void Pop();
  T& Top();

  bool IsEmpty() const;
  bool IsFull() const;
  int GetSize() const;
  int GetCapacity() const;
  
  bool operator==(const TStack<T>& other) const;
  bool operator!=(const TStack<T>& other) const;
  
  void Resize(const int newCapacity);
  
  template<typename U>
  friend std::ostream& operator<<(std::ostream& os, const TStack<U>& stack);
  
  template<typename U>
  friend std::istream& operator>>(std::istream& is, TStack<U>& stack);
};

template<typename T>
TStack<T>::TStack()
{
  capacity = 0;
  size = 0;
  memory = nullptr; 
}

template<typename T>
TStack<T>::TStack(const int capacity_)
{
  if (capacity_ < 0)
  {
    throw "Negative capacity";
  }
  capacity = capacity_;
  size = 0;
  if (capacity != 0) 
  {
    memory = new T[capacity];
  }
  else
  {
    memory = nullptr;
  }
}

template<typename T>
TStack<T>::TStack(const TStack<T>& other)
{
  capacity = other.capacity;
  size = other.size;
  if (capacity != 0)
  {
    memory = new T[capacity];
  for (int i = 0; i < size; ++i)
    {
      memory[i] = other.memory[i];
    }
  }
  else
  {
    memory = nullptr;
  }
}

template<typename T>
TStack<T>::TStack(TStack<T>&& other)
{
  capacity = other.capacity;
  size = other.size;
  memory = other.memory;
  other.memory = nullptr;
}

template<typename T>
TStack<T>::~TStack()
{
  if (memory != nullptr)
  {
    delete[] memory;
  }
}

template<typename T>
TStack<T>& TStack<T>::operator=(const TStack<T>& other)
{
  if (this == &other)
  {
    return *this;
  }
  
  if (memory != nullptr)
  {
    delete[] memory;
  }
  
  capacity = other.capacity;
  size = other.size;
  if (capacity != 0)
  {
    memory = new T[capacity];
    for (int i = 0; i < size; ++i)
    {
      memory[i] = other.memory[i];
    }
  }
  else
  {
    memory = nullptr;
  }
  return *this;
}

template<typename T>
TStack<T>& TStack<T>::operator=(TStack<T>&& other)
{
  if (this == &other)
  {
    return *this;
  }
  
  if (memory != nullptr)
  {
    delete[] memory;
  }
  
  capacity = other.capacity;
  size = other.size;
  memory = other.memory;
  other.memory = nullptr;
  other.capacity = 0;
  other.size = 0;
  return *this;
}

template<typename T>
void TStack<T>::Push(T value)
{
  if (size == capacity)
  {
    throw "Stack is full";
  }
  memory[size++] = value;
}

template<typename T>
void TStack<T>::Pop()
{
  if (size == 0)
  {
    throw "Stack is empty";
  }
  --size;
}

template<typename T>
T& TStack<T>::Top()
{
  if (size == 0)
  {
    throw "Stack is empty";
  }
  return memory[size - 1];
}

template<typename T>
bool TStack<T>::IsEmpty() const
{
  return size == 0;
}

template<typename T>
bool TStack<T>::IsFull() const
{
  return size == capacity;
}

template<typename T>
int TStack<T>::GetSize() const
{
  return size;
}

template<typename T>
int TStack<T>::GetCapacity() const
{
  return capacity;
}

template<typename T>
void TStack<T>::Resize(const int newCapacity)
{
  if (newCapacity < 0)
  {
    throw "Negative size";
  }
  if (capacity == newCapacity) 
  {
    return;
  }
  
  T* oldMemory = memory;
  int oldSize = size;
  
  capacity = newCapacity;
  if (size > capacity)
  {
    size = capacity;
  }
  
  if (capacity != 0)
  {
    memory = new T[capacity];
    if (oldMemory != nullptr)
    {
      for (int i = 0; i < size; ++i)
      {
        memory[i] = oldMemory[i];
      }
    }
  }
  else
  {
    memory = nullptr;
  }
  if (oldMemory != nullptr)
  {
    delete[] oldMemory;
  }
}

template<typename T>
bool TStack<T>::operator==(const TStack<T>& other) const
{
  if (size != other.size)
  {
    return false;
  }
  
  for (int i = 0; i < size; ++i)
  {
    if (memory[i] != other.memory[i])
    {
      return false;
    }
  }
  return true;
}

template<typename T>
bool TStack<T>::operator!=(const TStack<T>& other) const
{
  return !(*this == other);
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const TStack<T>& stack)
{
  os << "Stack[" << stack.size << "/" << stack.capacity << "]: ";
  if (stack.size == 0)
  {
    os << "(empty)";
  }
  else
  {
    os << "[";
    for (int i = 0; i < stack.size; ++i)
    {
      if (i > 0) os << ", ";
      os << stack.memory[i];
    }
    os << "] (top: " << stack.memory[stack.size - 1] << ")";
  }
  return os;
}

template<typename T>
std::istream& operator>>(std::istream& is, TStack<T>& stack)
{
  int newCapacity, numElements;
  
  is >> newCapacity;
  if (newCapacity < 0)
  {
    throw "Negative size";
  }
  
  is >> numElements;
  if (numElements < 0 || numElements > newCapacity)
  {
    throw "Sizes error";
  }
  
  {
    delete[] stack.memory;
  }
  
  stack.capacity = newCapacity;
  stack.size = 0;
  if (newCapacity > 0)
  {
    stack.memory = new T[newCapacity];
  }
  else
  {
    stack.memory = nullptr;
  }
  
  for (int i = 0; i < numElements; ++i)
  {
    T element;
    is >> element;
    stack.memory[stack.size++] = element;
  }
  
  return is;
}