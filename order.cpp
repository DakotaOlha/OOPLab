// Лабораторна робота №3, варіант 14 «Система Платежі».
// п.17: Реалізація класу Order.

#include "order.h"

#include <iomanip>
#include <iostream>

Order::Order() : id_(0), description_("—"), amount_(0.0), paid_(false) {}

Order::Order(int id, const std::string& description, double amount)
    : id_(id), description_(description), amount_(amount), paid_(false) {}

void Order::MarkPaid() { paid_ = true; }

void Order::Print() const {
  std::cout << std::fixed << std::setprecision(2) << "  Замовлення #" << id_
            << " «" << description_ << "» на " << amount_ << " грн — "
            << (paid_ ? "оплачено" : "не оплачено") << '\n';
}
