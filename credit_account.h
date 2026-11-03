// Лабораторна робота №4, третій рівень, варіант 14 «Система Платежі».
// ЛР4 п.13: заголовний файл класу CreditAccount (кредитний рахунок).

#ifndef CREDIT_ACCOUNT_H_
#define CREDIT_ACCOUNT_H_

#include <string>

#include "bank_account.h"

// ЛР4 п.2: private-наслідування — усі члени бази стають private, ззовні
// доступні лише власні public-методи CreditAccount.
class CreditAccount : private BankAccount {
 public:
  CreditAccount(const std::string& owner_name, const std::string& bank_name,
                const std::string& currency,
                const std::string& account_number, double balance,
                int opening_year, double credit_limit, double interest_rate,
                int overdue_days);
  ~CreditAccount();

  bool Borrow(double amount);
  void Repay(double amount);
  bool TransferTo(BankAccount& target, double amount);
  void AccrueMonthlyInterest();
  void Print() const;

  double debt() const { return debt_; }

 private:
  double credit_limit_;
  double interest_rate_;  // місячна ставка, %
  double debt_;
  int overdue_days_;
};

#endif  // CREDIT_ACCOUNT_H_
