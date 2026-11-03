// Лабораторна робота №4, третій рівень, варіант 14 «Система Платежі».
// ЛР4 п.13: заголовний файл класу SavingsAccount (депозитний рахунок).

#ifndef SAVINGS_ACCOUNT_H_
#define SAVINGS_ACCOUNT_H_

#include <string>

#include "bank_account.h"

constexpr double kEarlyWithdrawPenaltyPercent = 3.0;

// ЛР4 п.2: protected-наслідування — public-члени бази стають protected,
// тому ззовні недоступні без using.
class SavingsAccount : protected BankAccount {
 public:
  SavingsAccount(const std::string& owner_name, const std::string& bank_name,
                 const std::string& currency,
                 const std::string& account_number, double balance,
                 int opening_year, double interest_rate, int term_months,
                 double min_balance);
  ~SavingsAccount();

  // ЛР4 п.6: using повертає обраним членам бази public-доступ.
  using BankAccount::PrintInfo;
  using BankAccount::balance;
  using BankAccount::owner_name_;

  bool TopUp(double amount);
  void AddMonthlyInterest();
  bool EarlyWithdraw(double amount);
  void PrintSavings() const;

 private:
  double interest_rate_;  // річна ставка, %
  int term_months_;
  double min_balance_;
  int months_passed_;
};

#endif  // SAVINGS_ACCOUNT_H_
