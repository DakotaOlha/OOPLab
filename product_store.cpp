// Лабораторна робота №6, третій рівень, варіант 14 «Система Платежі».
// ЛР6 п.3: реалізація класу ProductStore (наскрізний operator[]).

#include "product_store.h"

#include <iomanip>
#include <iostream>
#include <new>
#include <sstream>
#include <string>

#include "exceptions.h"

namespace {

std::string NoMatchMessage(const std::string& array_name, double threshold) {
  std::ostringstream message;
  message << std::fixed << std::setprecision(2) << "у масиві " << array_name
          << " немає продуктів з вартістю > " << threshold << " грн/міс.";
  return message.str();
}

}  // namespace

ProductStore::ProductStore(std::size_t capacity)
    : capacity_(capacity),
      static_items_(nullptr),
      dynamic_items_(nullptr),
      static_size_(0),
      dynamic_size_(0) {
  // ЛР5 п.5: виняток — new не може виділити пам'ять (std::bad_alloc).
  try {
    static_items_ = new FinancialProduct*[capacity_];
  } catch (const std::bad_alloc&) {
    throw MemoryError("не вдалося виділити масив на " +
                      std::to_string(capacity_) + " покажчиків");
  }
  try {
    dynamic_items_ = new FinancialProduct*[capacity_];
  } catch (const std::bad_alloc&) {
    delete[] static_items_;
    throw MemoryError("не вдалося виділити другий масив");
  }
}

ProductStore::~ProductStore() {
  if (dynamic_size_ > 0) {
    std::cout << "  ~ProductStore(): знищення " << dynamic_size_
              << " динамічних об'єктів\n";
  }
  for (std::size_t i = 0; i < dynamic_size_; ++i)
    delete dynamic_items_[i];
  delete[] dynamic_items_;
  delete[] static_items_;
}

void ProductStore::AddStatic(FinancialProduct* product) {
  if (product == nullptr)
    throw MemoryError("спроба додати nullptr у масив статичних об'єктів");
  if (static_size_ == capacity_) {
    throw MemoryError("масив статичних об'єктів переповнено (місткість " +
                      std::to_string(capacity_) + ")");
  }
  static_items_[static_size_++] = product;
}

void ProductStore::AddDynamic(FinancialProduct* product) {
  // ЛР5 п.5: виняток — nullptr замість адреси об'єкта.
  if (product == nullptr)
    throw MemoryError("спроба додати nullptr у масив динамічних об'єктів");
  // ЛР5 п.5: виняток — переповнення масиву.
  if (dynamic_size_ == capacity_) {
    const int id = product->id();
    delete product;
    throw MemoryError("масив динамічних об'єктів переповнено (місткість " +
                      std::to_string(capacity_) + "), продукт №" +
                      std::to_string(id) + " знищено");
  }
  dynamic_items_[dynamic_size_++] = product;
}

// ЛР6 п.3: operator[] – спочатку статичні, потім динамічні продукти.
FinancialProduct& ProductStore::operator[](int index) {
  return *Combined(index);
}

const FinancialProduct& ProductStore::operator[](int index) const {
  return *Combined(index);
}

FinancialProduct* ProductStore::Combined(int index) const {
  if (index < 0 || static_cast<std::size_t>(index) >= size()) {
    throw MemoryError("operator[]: індекс " + std::to_string(index) +
                      " поза межами (продуктів " + std::to_string(size()) +
                      ")");
  }
  const std::size_t i = static_cast<std::size_t>(index);
  return i < static_size_ ? static_items_[i]
                          : dynamic_items_[i - static_size_];
}

FinancialProduct* ProductStore::StaticAt(int index) const {
  return ItemAt(static_items_, static_size_, index);
}

FinancialProduct* ProductStore::DynamicAt(int index) const {
  return ItemAt(dynamic_items_, dynamic_size_, index);
}

FinancialProduct* ProductStore::ItemAt(FinancialProduct* const* items,
                                       std::size_t size, int index) {
  // ЛР5 п.5: виняток — вихід за межі масиву.
  if (index < 0 || static_cast<std::size_t>(index) >= size) {
    throw MemoryError("вихід за межі масиву: індекс " +
                      std::to_string(index) + ", розмір " +
                      std::to_string(size));
  }
  return items[index];
}

void ProductStore::RemoveAt(FinancialProduct** items, std::size_t* size,
                            std::size_t index) {
  for (std::size_t i = index; i + 1 < *size; ++i)
    items[i] = items[i + 1];
  --*size;
}

void ProductStore::RemoveDynamicById(int id) {
  for (std::size_t i = 0; i < dynamic_size_; ++i) {
    if (dynamic_items_[i]->id() == id) {
      delete dynamic_items_[i];
      RemoveAt(dynamic_items_, &dynamic_size_, i);
      return;
    }
  }
  // ЛР5 п.6: виняток — повторне видалення (об'єкта вже немає в масиві).
  throw NotFoundError("продукту №" + std::to_string(id) +
                      " немає в масиві — повторне видалення заборонене");
}

int ProductStore::Service_Static(double max_monthly_cost) {
  // ЛР5 п.6: виняток — видалення з порожнього масиву.
  if (static_size_ == 0)
    throw MemoryError("масив статичних об'єктів порожній");

  int removed = 0;
  std::size_t i = 0;
  while (i < static_size_) {
    const FinancialProduct* product = static_items_[i];
    if (product->MonthlyCost() > max_monthly_cost) {
      std::cout << std::fixed << std::setprecision(2) << "  Вилучено №"
                << product->id() << ' ' << product->TypeName() << " ("
                << product->MonthlyCost()
                << " грн/міс.) — об'єкт на стеку живе до кінця блоку\n";
      RemoveAt(static_items_, &static_size_, i);
      ++removed;
    } else {
      ++i;
    }
  }
  // ЛР5 п.6: виняток — жоден об'єкт не відповідає критерію видалення.
  if (removed == 0)
    throw NotFoundError(NoMatchMessage("статичних", max_monthly_cost));
  return removed;
}

int ProductStore::Service_Dinamic(double max_monthly_cost) {
  // ЛР5 п.6: виняток — видалення з порожнього масиву.
  if (dynamic_size_ == 0)
    throw MemoryError("масив динамічних об'єктів порожній");

  int removed = 0;
  std::size_t i = 0;
  while (i < dynamic_size_) {
    FinancialProduct* product = dynamic_items_[i];
    if (product->MonthlyCost() > max_monthly_cost) {
      std::cout << std::fixed << std::setprecision(2) << "  Видалено №"
                << product->id() << ' ' << product->TypeName() << " ("
                << product->MonthlyCost() << " грн/міс.) через delete:\n";
      // ЛР5 п.3: деструктор віртуальний — спрацюють усі рівні.
      delete product;
      RemoveAt(dynamic_items_, &dynamic_size_, i);
      ++removed;
    } else {
      ++i;
    }
  }
  // ЛР5 п.6: виняток — жоден об'єкт не відповідає критерію видалення.
  if (removed == 0)
    throw NotFoundError(NoMatchMessage("динамічних", max_monthly_cost));
  return removed;
}

void ProductStore::Print() const {
  std::cout << std::fixed << std::setprecision(2)
            << "  Статичні (" << static_size_ << '/' << capacity_ << "):\n";
  for (std::size_t i = 0; i < static_size_; ++i) {
    std::cout << "    [" << i << "] №" << static_items_[i]->id() << ' '
              << static_items_[i]->TypeName() << ", "
              << static_items_[i]->MonthlyCost() << " грн/міс.\n";
  }
  std::cout << "  Динамічні (" << dynamic_size_ << '/' << capacity_
            << "):\n";
  for (std::size_t i = 0; i < dynamic_size_; ++i) {
    std::cout << "    [" << i << "] №" << dynamic_items_[i]->id() << ' '
              << dynamic_items_[i]->TypeName() << ", "
              << dynamic_items_[i]->MonthlyCost() << " грн/міс.\n";
  }
}
