// Лабораторна робота №7, третій рівень, варіант 14 «Система Платежі».
// ЛР7 п.4: реалізація власних функціональних об'єктів.

#include "functors.h"

CommissionCalculator::CommissionCalculator(double percent)
    : percent_(percent > 0.0 ? percent : 0.0) {}

double CommissionCalculator::operator()(double amount) const {
  return amount * percent_ / 100.0;
}

double CommissionCalculator::operator()(const PaymentRecord& record) const {
  return (*this)(record.amount());
}

AmountInRange::AmountInRange(double min_amount, double max_amount)
    : min_amount_(min_amount), max_amount_(max_amount) {}

bool AmountInRange::operator()(double amount) const {
  return amount >= min_amount_ && amount <= max_amount_;
}

bool AmountInRange::operator()(const PaymentRecord& record) const {
  return (*this)(record.amount());
}

bool EarlierDate::operator()(const PaymentRecord& a,
                             const PaymentRecord& b) const {
  return a.date() < b.date();
}
