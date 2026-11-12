// Лабораторна робота №6, третій рівень, варіант 14 «Система Платежі».
// ЛР6 п.2: реалізація класу Order (бінарні оператори + і -).

#include "order.h"

#include <iomanip>
#include <iostream>

Order::Order() : id_(0), description_("—"), amount_(0.0), paid_(false) {}

Order::Order(int id, const std::string& description, double amount)
    : id_(id), description_(description), amount_(amount), paid_(false) {}

// ЛР6 п.2: об'єднане замовлення – номер лівого, описи через « + », сума.
Order Order::operator+(const Order& other) const {
  Order result(id_, description_ + " + " + other.description_,
               amount_ + other.amount_);
  result.paid_ = paid_ && other.paid_;
  return result;
}

// ЛР6 п.2: замовлення зі знижкою, сума не може стати від'ємною.
Order Order::operator-(double discount) const {
  Order result(*this);
  result.amount_ = amount_ > discount ? amount_ - discount : 0.0;
  return result;
}

void Order::MarkPaid() { paid_ = true; }

void Order::Print() const {
  std::cout << std::fixed << std::setprecision(2) << "  Замовлення #" << id_
            << " «" << description_ << "» на " << amount_ << " грн — "
            << (paid_ ? "оплачено" : "не оплачено") << '\n';
}
