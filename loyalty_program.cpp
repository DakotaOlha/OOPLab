// Лабораторна робота №5, третій рівень, варіант 14 «Система Платежі».
// ЛР5 п.16: реалізація класу LoyaltyProgram.

#include "loyalty_program.h"

#include <iomanip>
#include <iostream>

namespace {

constexpr double kPointValue = 0.1;  // грн за один бал

}  // namespace

LoyaltyProgram::LoyaltyProgram(const std::string& owner,
                               const std::string& program_name,
                               const std::string& level,
                               double cashback_percent, int points_per_100,
                               int join_year)
    : FinancialProduct(owner, join_year),
      program_name_(program_name),
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

double LoyaltyProgram::MonthlyCost() const { return -cashback_earned_; }

std::string LoyaltyProgram::TypeName() const { return "LoyaltyProgram"; }

double LoyaltyProgram::BonusValue() const { return points_ * kPointValue; }

void LoyaltyProgram::Describe() const {
  FinancialProduct::Describe();
  DescribeOwn();
}

void LoyaltyProgram::DescribeOwn() const {
  std::cout << std::fixed << std::setprecision(2) << "    програма «"
            << program_name_ << "» (" << level_ << ") | бали: " << points_
            << " = " << BonusValue() << " грн\n";
}

void LoyaltyProgram::Write(std::ostream& out) const {
  WriteHeader(out);
  WriteOwn(out);
}

void LoyaltyProgram::Read(std::istream& in) {
  ReadHeader(in);
  ReadOwn(in);
}

void LoyaltyProgram::WriteOwn(std::ostream& out) const {
  out << program_name_ << '\n'
      << level_ << '\n'
      << cashback_percent_ << '\n'
      << cashback_earned_ << '\n'
      << points_ << '\n'
      << points_per_100_ << '\n'
      << join_year_ << '\n'
      << purchases_count_ << '\n';
}

void LoyaltyProgram::ReadOwn(std::istream& in) {
  program_name_ = ReadField(in, "назва програми");
  level_ = ReadField(in, "рівень");
  cashback_percent_ = ReadNumber(in, "відсоток кешбеку");
  cashback_earned_ = ReadNumber(in, "нараховано кешбеку");
  points_ = ReadInteger(in, "бали");
  points_per_100_ = ReadInteger(in, "балів за 100 грн");
  join_year_ = ReadInteger(in, "рік вступу");
  purchases_count_ = ReadInteger(in, "кількість покупок");
}
