// Лабораторна робота №5, третій рівень, варіант 14 «Система Платежі».
// ЛР5 п.16: заголовний файл класу CreditAccount (кредитний рахунок).

#ifndef CREDIT_ACCOUNT_H_
#define CREDIT_ACCOUNT_H_

#include <iosfwd>
#include <string>

#include "bank_account.h"
#include "financial_product.h"

// Private-наслідування — усі члени бази стають private, ззовні
// доступні лише власні public-методи CreditAccount.
// ЛР5 п.2: як і в SavingsAccount, FinancialProduct доступний відкрито
// другим шляхом; це той самий підоб'єкт завдяки virtual.
class CreditAccount : private BankAccount, public virtual FinancialProduct {
 public:
  CreditAccount(const std::string& owner_name, const std::string& bank_name,
                const std::string& currency,
                const std::string& account_number, double balance,
                int opening_year, double credit_limit, double interest_rate,
                int overdue_days);
  ~CreditAccount() override;

  // ЛР5 п.4: щомісячна вартість — відсотки на поточний борг.
  double MonthlyCost() const override;
  std::string TypeName() const override;
  void Describe() const override;
  void Write(std::ostream& out) const override;
  void Read(std::istream& in) override;
  // ЛР5 п.4: кредитний рахунок не можна закрити, поки є борг.
  bool CanBeClosed() const override;

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
