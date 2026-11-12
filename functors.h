// Лабораторна робота №7, третій рівень, варіант 14 «Система Платежі».
// ЛР7 п.4: заголовний файл власних функціональних об'єктів (функторів).

#ifndef FUNCTORS_H_
#define FUNCTORS_H_

#include "payment_record.h"

// ЛР7 п.4: функтор зі станом – комісія за переказ; ставка зберігається в
// полі, тому один об'єкт можна передати в transform чи інший компонент.
class CommissionCalculator {
 public:
  explicit CommissionCalculator(double percent);

  double operator()(double amount) const;
  double operator()(const PaymentRecord& record) const;

  double percent() const { return percent_; }

 private:
  double percent_;
};

// ЛР7 п.4: предикат «сума платежу в діапазоні [min, max]».
class AmountInRange {
 public:
  AmountInRange(double min_amount, double max_amount);

  bool operator()(double amount) const;
  bool operator()(const PaymentRecord& record) const;

 private:
  double min_amount_;
  double max_amount_;
};

// ЛР7 п.4: компаратор «раніша дата»; дати «РРРР-ММ-ДД» коректно
// порівнюються як рядки.
class EarlierDate {
 public:
  bool operator()(const PaymentRecord& a, const PaymentRecord& b) const;
};

#endif  // FUNCTORS_H_
