// Лабораторна робота №4, третій рівень, варіант 14 «Система Платежі».
// ЛР4 п.13: заголовний файл класу Insurance (страховий поліс).

#ifndef INSURANCE_H_
#define INSURANCE_H_

#include <string>

// ЛР4 п.4: одна з трьох баз для множинного наслідування PremiumAccount.
class Insurance {
 public:
  Insurance(const std::string& policy_number, const std::string& insurer,
            double coverage_amount, double rate_percent, int term_years);
  ~Insurance();

  double AnnualPremium() const;
  double RegisterClaim(double damage);
  // ЛР4 п.6: та сама назва, що й LoyaltyProgram::Print().
  void Print() const;

  const std::string& policy_number() const { return policy_number_; }

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
