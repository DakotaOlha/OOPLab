// Лабораторна робота №4, третій рівень, варіант 14 «Система Платежі».
// ЛР4 п.13: реалізація класу PremiumAccount.

#include "premium_account.h"

#include <iostream>

namespace {

constexpr char kPremiumBank[] = "ПриватБанк";
constexpr char kPremiumCurrency[] = "грн";
constexpr double kPremiumMonthlyFee = 150.0;
constexpr double kPremiumDailyLimit = 50000.0;
constexpr char kInsurer[] = "СК «Надійний захист»";
constexpr double kInsuranceRatePercent = 1.5;
constexpr int kInsuranceTermYears = 1;
constexpr char kLoyaltyLevel[] = "Gold";
constexpr int kPointsPer100 = 2;

}  // namespace

// ЛР4 п.4, п.5: 11 параметрів розподіляються між трьома базами.
PremiumAccount::PremiumAccount(const std::string& owner_name,
                               const std::string& account_number,
                               double balance, int opening_year,
                               const std::string& card_number,
                               double overdraft_limit,
                               const std::string& policy_number,
                               double coverage_amount,
                               const std::string& program_name,
                               double cashback_percent, int join_year)
    : CurrentAccount(owner_name, kPremiumBank, kPremiumCurrency,
                     account_number, balance, opening_year, card_number,
                     overdraft_limit, kPremiumMonthlyFee, kPremiumDailyLimit),
      Insurance(policy_number, kInsurer, coverage_amount,
                kInsuranceRatePercent, kInsuranceTermYears),
      LoyaltyProgram(program_name, kLoyaltyLevel, cashback_percent,
                     kPointsPer100, join_year) {
  std::cout << "    [5] PremiumAccount(...): преміальний рахунок "
            << account_number_ << '\n';
}

PremiumAccount::~PremiumAccount() {
  std::cout << "    [~5] ~PremiumAccount(): преміальний рахунок "
            << account_number_ << '\n';
}

bool PremiumAccount::PayWithCashback(double amount) {
  if (!Withdraw(amount, true))
    return false;

  Deposit(AddPurchase(amount));
  return true;
}

bool PremiumAccount::ReportDamage(double damage) {
  const double payout = RegisterClaim(damage);
  return payout > 0.0 && Deposit(payout);
}

void PremiumAccount::PrintInfo() const {
  CurrentAccount::PrintInfo();
  Insurance::Print();
  LoyaltyProgram::Print();
}
