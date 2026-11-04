// Лабораторна робота №4, третій рівень, варіант 14 «Система Платежі».
// ЛР4 п.13: заголовний файл класу PremiumAccount (преміальний рахунок).

#ifndef PREMIUM_ACCOUNT_H_
#define PREMIUM_ACCOUNT_H_

#include <iosfwd>
#include <string>

#include "current_account.h"
#include "financial_product.h"
#include "insurance.h"
#include "loyalty_program.h"

// ЛР4 п.4: множинне наслідування від трьох класів.
// ЛР4 п.7: бази створюються в порядку їх оголошення тут (BankAccount ->
// CurrentAccount -> Insurance -> LoyaltyProgram), а не в порядку списку
// ініціалізації; деструктори викликаються у зворотному порядку.
// ЛР5 п.2: ромб — FinancialProduct успадковано трьома шляхами, але завдяки
// virtual підоб'єкт один, і його конструктор викликає сам PremiumAccount
// (найпохідніший клас) першим, ще до BankAccount.
// Без virtual було б три підоб'єкти, і premium.id() / premium.Describe()
// не компілювались би: request for member 'id' is ambiguous.
class PremiumAccount : public CurrentAccount,
                       public Insurance,
                       public LoyaltyProgram {
 public:
  PremiumAccount(const std::string& owner_name,
                 const std::string& account_number, double balance,
                 int opening_year, const std::string& card_number,
                 double overdraft_limit, const std::string& policy_number,
                 double coverage_amount, const std::string& program_name,
                 double cashback_percent, int join_year);
  ~PremiumAccount() override;

  // ЛР5 п.2, п.4: override обов'язковий — інакше в ромбі три різні
  // «останні» перевизначення (CurrentAccount, Insurance, LoyaltyProgram).
  double MonthlyCost() const override;
  std::string TypeName() const override;
  void Describe() const override;
  void Write(std::ostream& out) const override;
  void Read(std::istream& in) override;

  bool PayWithCashback(double amount);
  bool ReportDamage(double damage);
  void PrintInfo() const;
};

#endif  // PREMIUM_ACCOUNT_H_
