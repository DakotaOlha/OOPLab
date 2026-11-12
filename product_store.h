// Лабораторна робота №6, третій рівень, варіант 14 «Система Платежі».
// ЛР6 п.3, п.4: заголовний файл класу ProductStore (operator[], шаблони).

#ifndef PRODUCT_STORE_H_
#define PRODUCT_STORE_H_

#include <cstddef>

#include "financial_product.h"

// ЛР5 п.5: два масиви покажчиків FinancialProduct*: на статичні об'єкти
// (стек, не володіє ними) і на динамічні (new, володіє і видаляє).
class ProductStore {
 public:
  explicit ProductStore(std::size_t capacity);
  ~ProductStore();
  ProductStore(const ProductStore&) = delete;
  ProductStore& operator=(const ProductStore&) = delete;

  void AddStatic(FinancialProduct* product);
  // Приймає володіння; при переповненні знищує об'єкт, щоб не було витоку.
  void AddDynamic(FinancialProduct* product);

  // ЛР6 п.3: наскрізний індекс – спочатку статичні продукти
  // (0 .. static_size-1), потім динамічні; поза межами – MemoryError.
  FinancialProduct& operator[](int index);
  const FinancialProduct& operator[](int index) const;

  // ЛР6 п.4: шаблонні функції-члени – підрахунок і пошук продуктів за
  // довільною умовою. Шаблон інстанціюється в місці виклику, тому його
  // визначення мусить бути в .h. Від Predicate потрібен виклик
  // pred(const FinancialProduct&), що повертає bool.
  template <typename Predicate>
  int CountIf(Predicate pred) const;
  template <typename Predicate>
  FinancialProduct* FindIf(Predicate pred) const;

  std::size_t size() const { return static_size_ + dynamic_size_; }

  FinancialProduct* StaticAt(int index) const;
  FinancialProduct* DynamicAt(int index) const;
  void RemoveDynamicById(int id);
  void Print() const;

  // ЛР5 п.6: видаляють з масиву продукти з MonthlyCost() > max_monthly_cost.
  // Винятки: масив порожній (MemoryError), жоден не підійшов (NotFoundError).
  // Назви з підкресленням — вимога завдання (Google Style: CamelCase).
  int Service_Static(double max_monthly_cost);
  int Service_Dinamic(double max_monthly_cost);

  std::size_t static_size() const { return static_size_; }
  std::size_t dynamic_size() const { return dynamic_size_; }

 private:
  FinancialProduct* Combined(int index) const;
  static FinancialProduct* ItemAt(FinancialProduct* const* items,
                                  std::size_t size, int index);
  static void RemoveAt(FinancialProduct** items, std::size_t* size,
                       std::size_t index);

  std::size_t capacity_;
  FinancialProduct** static_items_;
  FinancialProduct** dynamic_items_;
  std::size_t static_size_;
  std::size_t dynamic_size_;
};

template <typename Predicate>
int ProductStore::CountIf(Predicate pred) const {
  int count = 0;
  for (std::size_t i = 0; i < size(); ++i) {
    if (pred(*Combined(static_cast<int>(i))))
      ++count;
  }
  return count;
}

template <typename Predicate>
FinancialProduct* ProductStore::FindIf(Predicate pred) const {
  for (std::size_t i = 0; i < size(); ++i) {
    FinancialProduct* product = Combined(static_cast<int>(i));
    if (pred(*product))
      return product;
  }
  return nullptr;
}

#endif  // PRODUCT_STORE_H_
