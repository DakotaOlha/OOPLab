// Лабораторна робота №4, третій рівень, варіант 14 «Система Платежі».
// ЛР4 п.13: заголовний файл класу LoyaltyProgram (програма лояльності).

#ifndef LOYALTY_PROGRAM_H_
#define LOYALTY_PROGRAM_H_

#include <iosfwd>
#include <string>

#include "financial_product.h"

// ЛР4 п.4: одна з трьох баз для множинного наслідування PremiumAccount.
// ЛР5 п.1, п.2: похідний від абстрактного, віртуальне наслідування.
class LoyaltyProgram : public virtual FinancialProduct {
 public:
  LoyaltyProgram(const std::string& owner, const std::string& program_name,
                 const std::string& level, double cashback_percent,
                 int points_per_100, int join_year);
  ~LoyaltyProgram() override;

  // ЛР5 п.4: «вартість» від'ємна — нарахований клієнту кешбек.
  double MonthlyCost() const override;
  std::string TypeName() const override;
  void Describe() const override;
  void Write(std::ostream& out) const override;
  void Read(std::istream& in) override;
  // ЛР5 п.4: власна віртуальна функція — вартість балів у гривнях.
  virtual double BonusValue() const;

  double AddPurchase(double amount);
  // ЛР4 п.6: та сама назва, що й Insurance::Print().
  void Print() const;

  double cashback_earned() const { return cashback_earned_; }
  int points() const { return points_; }

 protected:
  void DescribeOwn() const;
  void WriteOwn(std::ostream& out) const;
  void ReadOwn(std::istream& in);

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
