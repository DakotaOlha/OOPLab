// Лабораторна робота №4, третій рівень, варіант 14 «Система Платежі».
// ЛР4 п.13: реалізація класу Insurance.

#include "insurance.h"

#include <iomanip>
#include <iostream>

Insurance::Insurance(const std::string& owner, int start_year,
                     const std::string& policy_number,
                     const std::string& insurer, double coverage_amount,
                     double rate_percent, int term_years)
    : FinancialProduct(owner, start_year),
      policy_number_(policy_number),
      insurer_(insurer),
      coverage_amount_(coverage_amount),
      rate_percent_(rate_percent),
      term_years_(term_years),
      claims_count_(0),
      paid_out_(0.0),
      is_policy_active_(true) {
  std::cout << "    [3] Insurance(...): поліс " << policy_number_ << '\n';
}

Insurance::~Insurance() {
  std::cout << "    [~3] ~Insurance(): поліс " << policy_number_ << '\n';
}

double Insurance::AnnualPremium() const {
  return coverage_amount_ * rate_percent_ / 100.0;
}

double Insurance::RegisterClaim(double damage) {
  const double remaining = coverage_amount_ - paid_out_;
  if (!is_policy_active_ || damage <= 0.0 || remaining <= 0.0) {
    std::cout << "  [Поліс " << policy_number_ << "] Відмова у виплаті.\n";
    return 0.0;
  }

  const double payout = damage < remaining ? damage : remaining;
  paid_out_ += payout;
  ++claims_count_;
  if (paid_out_ >= coverage_amount_)
    is_policy_active_ = false;

  std::cout << std::fixed << std::setprecision(2) << "  [Поліс "
            << policy_number_ << "] Страховий випадок №" << claims_count_
            << ": збиток " << damage << " грн, виплата " << payout
            << " грн\n";
  return payout;
}

void Insurance::Print() const {
  std::cout << std::fixed << std::setprecision(2) << "    страховка: поліс "
            << policy_number_ << " | " << insurer_ << " | покриття: "
            << coverage_amount_ << " | тариф: " << rate_percent_
            << "% (" << AnnualPremium() << " грн/рік) | строк: "
            << term_years_ << " р. | випадків: " << claims_count_
            << " | виплачено: " << paid_out_ << " | "
            << (is_policy_active_ ? "діє" : "вичерпано") << '\n';
}

double Insurance::MonthlyCost() const { return AnnualPremium() / 12.0; }

std::string Insurance::TypeName() const { return "Insurance"; }

double Insurance::CoverageLeft() const {
  return is_policy_active_ ? coverage_amount_ - paid_out_ : 0.0;
}

void Insurance::Describe() const {
  FinancialProduct::Describe();
  DescribeOwn();
}

void Insurance::DescribeOwn() const {
  std::cout << std::fixed << std::setprecision(2) << "    поліс "
            << policy_number_ << " | " << insurer_
            << " | залишок покриття: " << CoverageLeft() << " грн\n";
}

void Insurance::Write(std::ostream& out) const {
  WriteHeader(out);
  WriteOwn(out);
}

void Insurance::Read(std::istream& in) {
  ReadHeader(in);
  ReadOwn(in);
}

void Insurance::WriteOwn(std::ostream& out) const {
  out << policy_number_ << '\n'
      << insurer_ << '\n'
      << coverage_amount_ << '\n'
      << rate_percent_ << '\n'
      << term_years_ << '\n'
      << claims_count_ << '\n'
      << paid_out_ << '\n'
      << (is_policy_active_ ? 1 : 0) << '\n';
}

void Insurance::ReadOwn(std::istream& in) {
  policy_number_ = ReadField(in, "номер поліса");
  insurer_ = ReadField(in, "страховик");
  coverage_amount_ = ReadNumber(in, "сума покриття");
  rate_percent_ = ReadNumber(in, "тариф");
  term_years_ = ReadInteger(in, "строк");
  claims_count_ = ReadInteger(in, "кількість випадків");
  paid_out_ = ReadNumber(in, "виплачено");
  is_policy_active_ = ReadInteger(in, "поліс діє") != 0;
}
