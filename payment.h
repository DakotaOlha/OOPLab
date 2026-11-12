// Лабораторна робота №5, третій рівень, варіант 14 «Система Платежі».
// ЛР5 п.16: заголовний файл класу Payment.

#ifndef PAYMENT_H_
#define PAYMENT_H_

#include <string>

class Client;
class Order;

enum class PaymentType { kCard, kAccount };

// Клас асоціації між Client і Order — сам зв'язок «клієнт
// оплачує замовлення» має власні атрибути (номер, дата, тип, статус).
class Payment {
 public:
  // Const-поля і поля-посилання можна задати тільки у списку
  // ініціалізації конструктора.
  Payment(int id, const std::string& date, PaymentType type, Client& payer,
          Order& order);
  // Копія посилається на тих самих клієнта і замовлення.
  Payment(const Payment& other);
  Payment& operator=(const Payment& other) = delete;

  bool Execute();
  void Print() const;

 private:
  // Поля-константи та поля-посилання.
  const int id_;
  const std::string date_;
  const PaymentType type_;
  Client& payer_;
  Order& order_;
  bool completed_;
};

#endif  // PAYMENT_H_
