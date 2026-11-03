// Лабораторна робота №3, варіант 14 «Система Платежі».
// п.17: заголовний файл класу Payment.
//
// п.11: Payment — клас асоціації між Client і Order. Сам зв'язок
// «клієнт оплачує замовлення» має власні атрибути: номер, дату, тип
// платежу та статус (це і є запис в історії платежів).

#ifndef PAYMENT_H_
#define PAYMENT_H_

#include <string>

class Client;
class Order;

enum class PaymentType { kCard, kAccount };

class Payment {
 public:
  // п.2: конструктор зі списком ініціалізації. Константні поля та поля-
  // посилання можна задати тільки у списку ініціалізації.
  Payment(int id, const std::string& date, PaymentType type, Client& payer,
          Order& order);
  // п.1: конструктор копіювання.
  Payment(const Payment& other);
  Payment& operator=(const Payment& other) = delete;

  bool Execute();
  void Print() const;

 private:
  // п.2: поля-константи.
  const int id_;
  const std::string date_;
  const PaymentType type_;
  // п.2: поля-посилання на об'єкти, які пов'язує асоціація.
  Client& payer_;
  Order& order_;
  bool completed_;
};

#endif
