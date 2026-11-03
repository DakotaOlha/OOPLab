// Лабораторна робота №4, третій рівень, варіант 14 «Система Платежі».
// ЛР4 п.13: заголовний файл класу LoyaltyProgram (програма лояльності).

#ifndef LOYALTY_PROGRAM_H_
#define LOYALTY_PROGRAM_H_

#include <string>

// ЛР4 п.4: одна з трьох баз для множинного наслідування PremiumAccount.
class LoyaltyProgram {
 public:
  LoyaltyProgram(const std::string& program_name, const std::string& level,
                 double cashback_percent, int points_per_100, int join_year);
  ~LoyaltyProgram();

  double AddPurchase(double amount);
  // ЛР4 п.6: та сама назва, що й Insurance::Print().
  void Print() const;

  double cashback_earned() const { return cashback_earned_; }
  int points() const { return points_; }

 private:
  std::string program_name_;
  std::string level_;
  double cashback_percent_;
  double cashback_earned_;
  int points_;
  int points_per_100_;  // балів за кожні 100 грн покупки
  int join_year_;
  int purchases_count_;
};

#endif  // LOYALTY_PROGRAM_H_
