// Лабораторна робота №4, третій рівень, варіант 14 «Система Платежі».
// ЛР4 п.13: реалізація класу LoyaltyProgram.

#include "loyalty_program.h"

#include <iomanip>
#include <iostream>

LoyaltyProgram::LoyaltyProgram(const std::string& program_name,
                               const std::string& level,
                               double cashback_percent, int points_per_100,
                               int join_year)
    : program_name_(program_name),
      level_(level),
      cashback_percent_(cashback_percent),
      cashback_earned_(0.0),
      points_(0),
      points_per_100_(points_per_100),
      join_year_(join_year),
      purchases_count_(0) {
  std::cout << "    [4] LoyaltyProgram(...): «" << program_name_ << "», "
            << level_ << '\n';
}

LoyaltyProgram::~LoyaltyProgram() {
  std::cout << "    [~4] ~LoyaltyProgram(): «" << program_name_ << "»\n";
}

double LoyaltyProgram::AddPurchase(double amount) {
  const double cashback = amount * cashback_percent_ / 100.0;
  const int new_points = static_cast<int>(amount / 100.0) * points_per_100_;
  cashback_earned_ += cashback;
  points_ += new_points;
  ++purchases_count_;
  std::cout << std::fixed << std::setprecision(2) << "  [«" << program_name_
            << "»] Кешбек " << cashback << " грн, +" << new_points
            << " балів (усього " << points_ << ")\n";
  return cashback;
}

void LoyaltyProgram::Print() const {
  std::cout << std::fixed << std::setprecision(2) << "    лояльність: «"
            << program_name_ << "» | рівень: " << level_
            << " | кешбек: " << cashback_percent_
            << "% | нараховано: " << cashback_earned_
            << " грн | бали: " << points_ << " (" << points_per_100_
            << " за 100 грн) | з " << join_year_
            << " р. | покупок: " << purchases_count_ << '\n';
}
