// Лабораторна робота №4, третій рівень, варіант 14 «Система Платежі».
// ЛР4 п.13: заголовний файл класу CurrentAccount (поточний рахунок).

#ifndef CURRENT_ACCOUNT_H_
#define CURRENT_ACCOUNT_H_

#include <string>

#include "bank_account.h"

// ЛР4 п.2: public-наслідування — public і protected члени бази зберігають
// свій рівень доступу.
class CurrentAccount : public BankAccount {
 public:
  CurrentAccount(const std::string& owner_name, const std::string& bank_name,
                 const std::string& currency,
                 const std::string& account_number, double balance,
                 int opening_year, const std::string& card_number,
                 double overdraft_limit, double monthly_fee,
                 double daily_limit);
  ~CurrentAccount();

  // ЛР4 п.6: без using власний Withdraw приховав би BankAccount::Withdraw.
  using BankAccount::Withdraw;
  bool Withdraw(double amount, bool use_overdraft);

  void PrintInfo() const;
  void ChargeMonthlyFee();
  void StartNewDay();
  int AccountAgeYears(int current_year) const;

  const std::string& card_number() const { return card_number_; }
  double overdraft_limit() const { return overdraft_limit_; }
  double spent_today() const { return spent_today_; }

 private:
  std::string card_number_;
  double overdraft_limit_;
  double monthly_fee_;
  double daily_limit_;
  double spent_today_;
};

#endif  // CURRENT_ACCOUNT_H_
