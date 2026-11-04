// Лабораторна робота №4, третій рівень, варіант 14 «Система Платежі».
// ЛР4 п.13: заголовний файл класу InstallmentPlan (план виплат).

#ifndef INSTALLMENT_PLAN_H_
#define INSTALLMENT_PLAN_H_

#include <iosfwd>
#include <string>

#include "current_account.h"
#include "financial_product.h"

// ЛР4 п.3: додатковий похідний клас з private-наслідуванням; його методи
// використовують методи BankAccount і CurrentAccount.
// ЛР5 п.2: відкритий шлях до віртуального підоб'єкта FinancialProduct.
class InstallmentPlan : private CurrentAccount,
                        public virtual FinancialProduct {
 public:
  InstallmentPlan(const std::string& owner_name, const std::string& bank_name,
                  const std::string& account_number, double balance,
                  int opening_year, double overdraft_limit,
                  const std::string& purchase_description,
                  double total_amount, int parts_count,
                  int first_payment_month);
  ~InstallmentPlan() override;

  // ЛР5 п.4: щомісячна вартість — чергова частина (0, якщо все сплачено).
  double MonthlyCost() const override;
  std::string TypeName() const override;
  void Describe() const override;
  void Write(std::ostream& out) const override;
  void Read(std::istream& in) override;

  // ЛР4 п.6: using відкриває члени непрямої та прямої бази.
  using BankAccount::owner_name_;
  using CurrentAccount::balance;

  bool PayNextPart();
  bool TopUp(double amount);
  void PrintSchedule() const;

  bool is_completed() const { return paid_parts_ == parts_count_; }

 private:
  std::string purchase_description_;
  double total_amount_;
  int parts_count_;
  int paid_parts_;
  int first_payment_month_;
};

#endif  // INSTALLMENT_PLAN_H_
