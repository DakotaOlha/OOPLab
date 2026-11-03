// Лабораторна робота №4, третій рівень, варіант 14 «Система Платежі».
// ЛР4 п.13: реалізація класу SavingsAccount.

#include "savings_account.h"

#include <iomanip>
#include <iostream>

// ЛР4 п.5: 6 параметрів передаються базі, 3 — власним полям.
SavingsAccount::SavingsAccount(const std::string& owner_name,
                               const std::string& bank_name,
                               const std::string& currency,
                               const std::string& account_number,
                               double balance, int opening_year,
                               double interest_rate, int term_months,
                               double min_balance)
    : BankAccount(owner_name, bank_name, currency, account_number, balance,
                  opening_year),
      interest_rate_(interest_rate),
      term_months_(term_months),
      min_balance_(min_balance),
      months_passed_(0) {
  std::cout << "    [2] SavingsAccount(...): депозит " << interest_rate_
            << "% на " << term_months_ << " міс.\n";
}

SavingsAccount::~SavingsAccount() {
  std::cout << "    [~2] ~SavingsAccount(): депозит " << account_number_
            << '\n';
}

bool SavingsAccount::TopUp(double amount) {
  // Deposit() тут protected, але всередині похідного класу доступний.
  return Deposit(amount);
}

void SavingsAccount::AddMonthlyInterest() {
  const double interest = balance_ * interest_rate_ / 100.0 / 12.0;
  balance_ += interest;
  ++transaction_count_;
  ++months_passed_;
  std::cout << std::fixed << std::setprecision(2) << "  [" << account_number_
            << "] Нараховано відсотки за місяць " << months_passed_ << ": "
            << interest << ' ' << currency_ << ". Баланс: " << balance_
            << ' ' << currency_ << '\n';
}

bool SavingsAccount::EarlyWithdraw(double amount) {
  const double penalty = months_passed_ < term_months_
                             ? amount * kEarlyWithdrawPenaltyPercent / 100.0
                             : 0.0;
  if (balance_ - amount - penalty < min_balance_) {
    std::cout << std::fixed << std::setprecision(2) << "  ["
              << account_number_ << "] Відмова: залишок має бути не менше "
              << min_balance_ << ' ' << currency_ << '\n';
    return false;
  }

  std::cout << std::fixed << std::setprecision(2) << "  [" << account_number_
            << "] Дострокове зняття " << amount << ' ' << currency_
            << ", штраф " << penalty << ' ' << currency_ << '\n';
  return Withdraw(amount + penalty);
}

void SavingsAccount::PrintSavings() const {
  PrintInfo();
  std::cout << std::fixed << std::setprecision(2) << "    депозит: ставка "
            << interest_rate_ << "% | строк: " << term_months_
            << " міс. | минуло: " << months_passed_
            << " міс. | мін. залишок: " << min_balance_ << '\n';
}
