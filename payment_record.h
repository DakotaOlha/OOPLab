// Лабораторна робота №6, третій рівень, варіант 14 «Система Платежі».
// ЛР6 п.6: заголовний файл класу PaymentRecord (запис історії платежів).

#ifndef PAYMENT_RECORD_H_
#define PAYMENT_RECORD_H_

#include <iosfwd>
#include <string>

#include "payment.h"

// ЛР6 п.6: користувацький тип – параметр шаблону History<T>. Має всі
// оператори, яких вимагає шаблон: +, /(int), <, >, ==, !=, <<.
class PaymentRecord {
 public:
  PaymentRecord();
  PaymentRecord(const std::string& date, double amount, PaymentType type,
                const std::string& sender, const std::string& receiver);

  // ЛР6 п.6: + накопичує суму платежів (History::Total).
  PaymentRecord operator+(const PaymentRecord& other) const;
  // ЛР6 п.6: / – «середній платіж» (History::Average).
  PaymentRecord operator/(int count) const;
  // ЛР6 п.6: порівняння за сумою (History::Max/Min, MaxOf).
  bool operator<(const PaymentRecord& other) const;
  bool operator>(const PaymentRecord& other) const;
  // ЛР6 п.6: рівність усіх полів (History::IndexOf/Contains).
  bool operator==(const PaymentRecord& other) const;
  bool operator!=(const PaymentRecord& other) const;
  // ЛР6 п.6: виведення в потік (History::Print).
  friend std::ostream& operator<<(std::ostream& out,
                                  const PaymentRecord& record);

  const std::string& date() const { return date_; }
  double amount() const { return amount_; }
  PaymentType type() const { return type_; }
  const std::string& sender() const { return sender_; }
  const std::string& receiver() const { return receiver_; }

 private:
  std::string date_;
  double amount_;
  PaymentType type_;
  std::string sender_;
  std::string receiver_;
};

#endif  // PAYMENT_RECORD_H_
