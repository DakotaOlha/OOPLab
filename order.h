// Лабораторна робота №3, варіант 14 «Система Платежі».
// п.17: заголовний файл класу Order.

#ifndef ORDER_H_
#define ORDER_H_

#include <string>

class Order {
 public:
  Order();
  Order(int id, const std::string& description, double amount);

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

#endif
