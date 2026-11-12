// Лабораторна робота №5, третій рівень, варіант 14 «Система Платежі».
// ЛР5 п.16: заголовний файл класу Insurance (страховий поліс).

#ifndef INSURANCE_H_
#define INSURANCE_H_

#include <iosfwd>
#include <string>

#include "financial_product.h"

// Одна з трьох баз для множинного наслідування PremiumAccount.
// ЛР5 п.1, п.2: похідний від абстрактного, віртуальне наслідування.
class Insurance : public virtual FinancialProduct {
 public:
  Insurance(const std::string& owner, int start_year,
            const std::string& policy_number, const std::string& insurer,
            double coverage_amount, double rate_percent, int term_years);
  ~Insurance() override;

  // ЛР5 п.4: щомісячна вартість — 1/12 річного страхового внеску.
  double MonthlyCost() const override;
  std::string TypeName() const override;
  void Describe() const override;
  void Write(std::ostream& out) const override;
  void Read(std::istream& in) override;
  // ЛР5 п.4: власна віртуальна функція — залишок страхового покриття.
  virtual double CoverageLeft() const;

  double AnnualPremium() const;
  double RegisterClaim(double damage);
  // Та сама назва, що й LoyaltyProgram::Print().
  void Print() const;

  const std::string& policy_number() const { return policy_number_; }

 protected:
  void DescribeOwn() const;
  void WriteOwn(std::ostream& out) const;
  void ReadOwn(std::istream& in);

 private:
  std::string policy_number_;
  std::string insurer_;
  double coverage_amount_;
  double rate_percent_;
  int term_years_;
  int claims_count_;
  double paid_out_;
  bool is_policy_active_;
};

#endif  // INSURANCE_H_
