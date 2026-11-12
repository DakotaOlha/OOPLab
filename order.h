// Лабораторна робота №6, третій рівень, варіант 14 «Система Платежі».
// ЛР6 п.2: заголовний файл класу Order (бінарні оператори + і -).

#ifndef ORDER_H_
#define ORDER_H_

#include <string>

class Order {
 public:
  Order();
  Order(int id, const std::string& description, double amount);

  // ЛР6 п.2: + – об'єднане замовлення, - – замовлення зі знижкою.
  // Обидва повертають новий об'єкт і не змінюють операндів.
  Order operator+(const Order& other) const;
  Order operator-(double discount) const;

  void MarkPaid();
  void Print() const;

  int id() const { return id_; }
  const std::string& description() const { return description_; }
  double amount() const { return amount_; }
  bool is_paid() const { return paid_; }

 private:
  int id_;
  std::string description_;
  double amount_;
  bool paid_;
};

#endif  // ORDER_H_
