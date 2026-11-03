// Лабораторна робота №4, третій рівень, варіант 14 «Система Платежі».
// ЛР4 п.13: реалізація класу асоціації Payment (з ЛР3, ЛР3 п.17).

#include "payment.h"

#include <iomanip>
#include <iostream>

#include "client.h"
#include "order.h"

Payment::Payment(int id, const std::string& date, PaymentType type,
                 Client& payer, Order& order)
    : id_(id),
      date_(date),
      type_(type),
      payer_(payer),
      order_(order),
      completed_(false) {
  std::cout << "    [Payment(...)] створено платіж #" << id_ << " від "
            << date_ << '\n';
}

Payment::Payment(const Payment& other)
    : id_(other.id_),
      date_(other.date_),
      type_(other.type_),
      payer_(other.payer_),
      order_(other.order_),
      completed_(other.completed_) {
  std::cout << "    [Payment(const Payment&)] копіювання платежу #" << id_
            << '\n';
}

// ЛР3 п.11: взаємодія трьох об'єктів — Payment пов'язує Client та Order.
bool Payment::Execute() {
  std::cout << "  Виконання платежу #" << id_ << " за замовлення #"
            << order_.id() << ":\n";
  if (order_.is_paid()) {
    std::cout << "  Відмова: замовлення вже оплачено.\n";
    return false;
  }

  completed_ =
      payer_.PayOrder(order_.amount(), type_ == PaymentType::kAccount);
  if (completed_)
    order_.MarkPaid();

  return completed_;
}

void Payment::Print() const {
  std::cout << std::fixed << std::setprecision(2) << "  Платіж #" << id_
            << " | " << date_ << " | " << payer_.last_name_ << " -> "
            << "замовлення #" << order_.id() << " «" << order_.description()
            << "» | " << order_.amount() << " грн | "
            << (type_ == PaymentType::kCard ? "картка" : "рахунок") << " | "
            << (completed_ ? "виконано" : "не виконано") << '\n';
}
