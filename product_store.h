// Лабораторна робота №5, третій рівень, варіант 14 «Система Платежі».
// ЛР5 п.16: заголовний файл класу ProductStore (масиви продуктів).

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

#endif  // PRODUCT_STORE_H_
