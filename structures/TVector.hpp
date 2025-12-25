#pragma once
#include <stddef.h>
#include <utility>
#include <iostream>
#include <fstream>

template<typename T>
class TVectorIterator;

template<typename T>
class TVector
{
protected:
  size_t capacity;
  size_t length;
  T* memory;
  
public:
  TVector();
  explicit TVector(const size_t size);
  TVector(const TVector& other);
  TVector(TVector&& other);
  ~TVector();

  TVector& operator=(const TVector& other);
  TVector& operator=(TVector&& other);

  bool operator==(const TVector& other) const;
  bool operator!=(const TVector& other) const;

  T& operator[](const size_t index) const;

  size_t GetSize() const;
  bool Empty() const;
  bool Full() const;

  size_t CountOccurrences(const T& value) const;
  TVector<size_t> FindAllOccurrences(const T& value) const;
  
  template<typename F>
  void ApplyToAll(F func);

  void SaveToFile(const std::string& filename) const;
  void LoadFromFile(const std::string& filename);

  void PushBack(T&& value);
  void PopBack();
  void PushFront(T&& value);
  void PopFront();

  TVectorIterator<T> begin();
  TVectorIterator<T> end();
};

template<typename T>
TVector<T>::TVector()
{
  capacity = 0;
  length = 0;
  memory = nullptr;
}

template<typename T>
TVector<T>::TVector(const size_t size) : capacity(size), length(size)
{
  if (size == 0)
  {
    memory = nullptr;
  }
  else
  {
    memory = new T[size];
    for (size_t i = 0; i < size; ++i)
    {
      memory[i] = T();
    }
  };
}

template<typename T>
TVector<T>::TVector(const TVector& other)
{
  capacity = other.capacity;
  length = other.length;
  if (capacity == 0)
  {
    memory = nullptr;
  }
  else
  {
    memory = new T[capacity];
    for (size_t i = 0; i < length; ++i)
    {
      memory[i] = other.memory[i];
    }
  }
}

template<typename T>
TVector<T>::TVector(TVector&& other)
{
  capacity = other.capacity;
  length = other.length;
  memory = other.memory;
  other.capacity = 0;
  other.length = 0;
  other.memory = nullptr;
}

template<typename T>
TVector<T>::~TVector()
{
  if (memory != nullptr)
  {
    delete[] memory;
  }
}

template<typename T>
TVector<T>& TVector<T>::operator=(const TVector& other)
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
  length = other.length;
  if (capacity == 0)
  {
    memory = nullptr;
  }
  else
  {
    memory = new T[capacity];
    for (size_t i = 0; i < length; ++i)
    {
      memory[i] = other.memory[i];
    }
  }
  return *this;
}

template<typename T>
TVector<T>& TVector<T>::operator=(TVector&& other)
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
  length = other.length;
  memory = other.memory;
  other.capacity = 0;
  other.length = 0;
  other.memory = nullptr;
  return *this;
}

template<typename T>
bool TVector<T>::operator==(const TVector& other) const
{
  if (length != other.length)
  {
    return false;
  }
  for (size_t i = 0; i < length; ++i)
  {
    if (memory[i] != other.memory[i])
    {
      return false;
    }
  }
  return true;
}

template<typename T>
bool TVector<T>::operator!=(const TVector& other) const
{
  return !(*this == other);
}

template<typename T>
T& TVector<T>::operator[](const size_t index) const
{
  if (index >= length)
  {
    throw "Index out of range";
  }
  return memory[index];
}

template<typename T>
size_t TVector<T>::GetSize() const
{
  return length;
}

template<typename T>
bool TVector<T>::Empty() const
{
  return length == 0;
}

template<typename T>
void TVector<T>::PushBack(T&& value)
{
  if (capacity == 0)
  {
    capacity = 1;
    length = 0;
    memory = new T[capacity];
  }
  else if (length == capacity)
  {
    capacity <<= 1;
    T* pNewMemory = new T[capacity];
    for (size_t i = 0; i < length; ++i)
    {
      pNewMemory[i] = memory[i];
    }
    delete[] memory;
    memory = pNewMemory;
  }
  memory[length] = value;
  ++length;
}

template<typename T>
void TVector<T>::PopBack()
{
  if (length == 0)
  {
    throw "Vector is empty";
  }
  --length;
}

template<typename T>
void TVector<T>::PushFront(T&& value)
{
  this->PushBack(std::move(value));
  for (size_t i = length - 1; i > 0; --i)
  {
    memory[i] = memory[i - 1];
  }
  memory[0] = value;
}

template<typename T>
void TVector<T>::PopFront()
{
  if (length == 0)
  {
    throw "Vector is empty";
  }
  for (size_t i = 0; i < length - 1; ++i)
  {
    memory[i] = memory[i + 1];
  }
  --length;
}

template<typename T>
TVectorIterator<T> TVector<T>::begin()
{
  return TVectorIterator<T>(*this, 0);
}

template<typename T>
TVectorIterator<T> TVector<T>::end()
{
  return TVectorIterator<T>(*this, length);
}

template<typename T>
bool TVector<T>::Full() const
{
  return capacity > 0 && length == capacity;
}

template<typename T>
size_t TVector<T>::CountOccurrences(const T& value) const
{
  size_t count = 0;
  for (size_t i = 0; i < length; ++i)
  {
    if (memory[i] == value)
    {
      ++count;
    }
  }
  return count;
}

template<typename T>
TVector<size_t> TVector<T>::FindAllOccurrences(const T& value) const
{
  TVector<size_t> indices;
  for (size_t i = 0; i < length; ++i)
  {
    if (memory[i] == value)
    {
      indices.PushBack(std::move(i));
    }
  }
  return indices;
}

template<typename T>
template<typename F>
void TVector<T>::ApplyToAll(F func)
{
  for (size_t i = 0; i < length; ++i)
  {
    func(memory[i]);
  }
}

template<typename T>
void TVector<T>::SaveToFile(const std::string& filename) const
{
  std::ofstream file(filename);
  if (!file.is_open())
  {
    throw "Cannot open file for writing";
  }
  file << length << std::endl;
  for (size_t i = 0; i < length; ++i)
  {
    file << memory[i];
    if (i < length - 1)
    {
      file << " ";
    }
  }
  file.close();
}

template<typename T>
void TVector<T>::LoadFromFile(const std::string& filename)
{
  std::ifstream file(filename);
  if (!file.is_open())
  {
    throw "Cannot open file for reading";
  }
  size_t size;
  file >> size;
  *this = TVector<T>(size);
  for (size_t i = 0; i < size; ++i)
  {
    file >> memory[i];
  }
  file.close();
}

template<typename T>
class TVectorIterator
{
private:
  TVector<T>& vector;
  size_t      index;
  
public:
  TVectorIterator(TVector<T>& vector_, size_t index_ = 0);
  TVectorIterator(const TVectorIterator<T>& other);

  T& operator*();
  TVectorIterator<T> operator++();
  TVectorIterator<T> operator++(int);

  bool operator == (const TVectorIterator<T>& other) const;
  bool operator != (const TVectorIterator<T>& other) const;
};

template<typename T>
TVectorIterator<T>::TVectorIterator(TVector<T>& vector_, size_t index_) : vector(vector_), index(index_)
{

}

template<typename T>
TVectorIterator<T>::TVectorIterator(const TVectorIterator<T>& other) : vector(other.vector), index(other.index)
{

}

template<typename T>
T& TVectorIterator<T>::operator*()
{
  return vector[index];
}

template<typename T>
TVectorIterator<T> TVectorIterator<T>::operator++()
{
  ++index;
  return *this;
}

template<typename T>
TVectorIterator<T> TVectorIterator<T>::operator++(int)
{
  TVectorIterator<T> iterator(*this);
  ++index;
  return iterator;
}

template<typename T>
bool TVectorIterator<T>::operator == (const TVectorIterator<T>& other) const
{
  return (&vector == &other.vector) && (index == other.index);
}

template<typename T>
bool TVectorIterator<T>::operator != (const TVectorIterator<T>& other) const
{
  return !(*this == other);
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const TVector<T>& vector)
{
  os << "[";
  for (size_t i = 0; i < vector.GetSize(); ++i)
  {
    os << vector[i];
    if (i < vector.GetSize() - 1)
    {
      os << ", ";
    }
  }
  os << "]";
  return os;
}

template<typename T>
std::istream& operator>>(std::istream& is, TVector<T>& vector)
{
  size_t size;
  is >> size;
  vector = TVector<T>(size);
  for (size_t i = 0; i < size; ++i)
  {
    is >> vector[i];
  }
  return is;
}