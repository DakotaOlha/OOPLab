// Лабораторна робота №6, третій рівень, варіант 14 «Система Платежі».
// ЛР6 п.5: шаблон класу History<T> – історія платежів (лише .h-файл).

#ifndef HISTORY_H_
#define HISTORY_H_

#include <cstddef>
#include <iostream>
#include <new>
#include <string>

#include "exceptions.h"

// ЛР6 п.5: шаблон класу з шаблонними даними items_, total_, largest_,
// smallest_, last_added_ та методами Add, Total, Average, Max, Min, Last,
// IndexOf, Contains, operator[], Print.
// Шаблон повністю в .h: компілятор створює History<T> для конкретного T
// у місці використання, тому має бачити всі визначення методів.
// Вимоги до T: конструктор за замовчуванням, копіювання і присвоєння,
// operator+ (Total), operator/(int) (Average), operator> (Max),
// operator< (Min), operator== (IndexOf), operator<< (Print).
template <typename T>
class History {
 public:
  explicit History(const std::string& title, std::size_t capacity = 4);
  ~History() { delete[] items_; }
  History(const History&) = delete;
  History& operator=(const History&) = delete;

  void Add(const T& item);
  T Total() const { return total_; }
  T Average() const;
  T Max() const;
  T Min() const;
  T Last() const;
  int IndexOf(const T& value) const;
  bool Contains(const T& value) const { return IndexOf(value) >= 0; }
  T& operator[](int index);
  const T& operator[](int index) const;
  void Print() const;

  std::size_t Size() const { return size_; }
  bool IsEmpty() const { return size_ == 0; }
  const std::string& title() const { return title_; }

 private:
  void Grow();
  void CheckNotEmpty(const std::string& operation) const;
  void CheckIndex(int index) const;

  T* items_;
  T total_;
  T largest_;
  T smallest_;
  T last_added_;
  std::size_t size_;
  std::size_t capacity_;
  std::string title_;
};

// ЛР6 п.6: вільна шаблонна функція; від T потрібен operator>.
template <typename T>
const T& MaxOf(const T& a, const T& b) {
  return a > b ? a : b;
}

template <typename T>
History<T>::History(const std::string& title, std::size_t capacity)
    : items_(nullptr),
      total_(),
      largest_(),
      smallest_(),
      last_added_(),
      size_(0),
      capacity_(capacity > 0 ? capacity : 1),
      title_(title) {
  try {
    items_ = new T[capacity_];
  } catch (const std::bad_alloc&) {
    throw MemoryError("History: не вдалося виділити пам'ять на " +
                      std::to_string(capacity_) + " елементів");
  }
}

template <typename T>
void History<T>::Grow() {
  const std::size_t new_capacity = capacity_ * 2;
  T* bigger = nullptr;
  try {
    bigger = new T[new_capacity];
  } catch (const std::bad_alloc&) {
    throw MemoryError("History: не вдалося збільшити масив до " +
                      std::to_string(new_capacity) + " елементів");
  }
  for (std::size_t i = 0; i < size_; ++i)
    bigger[i] = items_[i];
  delete[] items_;
  items_ = bigger;
  capacity_ = new_capacity;
}

template <typename T>
void History<T>::Add(const T& item) {
  if (size_ == capacity_)
    Grow();
  items_[size_] = item;
  if (size_ == 0) {
    total_ = item;
    largest_ = item;
    smallest_ = item;
  } else {
    total_ = total_ + item;
    if (item > largest_)
      largest_ = item;
    if (item < smallest_)
      smallest_ = item;
  }
  last_added_ = item;
  ++size_;
}

template <typename T>
void History<T>::CheckNotEmpty(const std::string& operation) const {
  if (size_ == 0)
    throw NotFoundError("History «" + title_ + "»: " + operation +
                        " для порожньої історії");
}

template <typename T>
T History<T>::Average() const {
  CheckNotEmpty("Average");
  return total_ / static_cast<int>(size_);
}

template <typename T>
T History<T>::Max() const {
  CheckNotEmpty("Max");
  return largest_;
}

template <typename T>
T History<T>::Min() const {
  CheckNotEmpty("Min");
  return smallest_;
}

template <typename T>
T History<T>::Last() const {
  CheckNotEmpty("Last");
  return last_added_;
}

template <typename T>
int History<T>::IndexOf(const T& value) const {
  for (std::size_t i = 0; i < size_; ++i) {
    if (items_[i] == value)
      return static_cast<int>(i);
  }
  return -1;
}

template <typename T>
void History<T>::CheckIndex(int index) const {
  if (index < 0 || static_cast<std::size_t>(index) >= size_) {
    throw MemoryError("History «" + title_ + "»: індекс " +
                      std::to_string(index) + " поза межами (розмір " +
                      std::to_string(size_) + ")");
  }
}

template <typename T>
T& History<T>::operator[](int index) {
  CheckIndex(index);
  return items_[index];
}

template <typename T>
const T& History<T>::operator[](int index) const {
  CheckIndex(index);
  return items_[index];
}

template <typename T>
void History<T>::Print() const {
  std::cout << "  " << title_ << " (" << size_ << " з " << capacity_
            << "):\n";
  for (std::size_t i = 0; i < size_; ++i)
    std::cout << "    [" << i << "] " << items_[i] << '\n';
  if (size_ > 0)
    std::cout << "    Разом: " << total_ << '\n';
}

#endif  // HISTORY_H_
