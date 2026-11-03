// Лабораторна робота №4, третій рівень, варіант 14 «Система Платежі».
// ЛР4 п.13: заголовний файл класу Payment (з ЛР3, ЛР3 п.17).

#ifndef PAYMENT_H_
#define PAYMENT_H_

#include <string>

class Client;
class Order;

enum class PaymentType { kCard, kAccount };

// ЛР3 п.11: клас асоціації між Client і Order — сам зв'язок «клієнт
// оплачує замовлення» має власні атрибути (номер, дата, тип, статус).
class Payment {
 public:
  // ЛР3 п.2: const-поля і поля-посилання можна задати тільки у списку
  // ініціалізації конструктора.
  Payment(int id, const std::string& date, PaymentType type, Client& payer,
          Order& order);
  // ЛР3 п.1: копія посилається на тих самих клієнта і замовлення.
  Payment(const Payment& other);
  Payment& operator=(const Payment& other) = delete;

  bool Execute();
  void Print() const;

 private:
  // ЛР3 п.2: поля-константи та поля-посилання.
  const int id_;
  const std::string date_;
  const PaymentType type_;
  Client& payer_;
  Order& order_;
  bool completed_;
};

#endif  // PAYMENT_H_
