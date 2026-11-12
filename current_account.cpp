// Лабораторна робота №5, третій рівень, варіант 14 «Система Платежі».
// ЛР5 п.16: реалізація класу CurrentAccount.

#include "current_account.h"

#include <algorithm>
#include <iomanip>
#include <iostream>

// 10 параметрів — 6 передаються конструктору бази у списку
// ініціалізації, решта 4 ініціалізують власні поля.
CurrentAccount::CurrentAccount(const std::string& owner_name,
                               const std::string& bank_name,
                               const std::string& currency,
                               const std::string& account_number,
                               double balance, int opening_year,
                               const std::string& card_number,
                               double overdraft_limit, double monthly_fee,
                               double daily_limit)
    : FinancialProduct(owner_name, opening_year),
      BankAccount(owner_name, bank_name, currency, account_number, balance,
                  opening_year),
      card_number_(card_number),
      overdraft_limit_(overdraft_limit),
      monthly_fee_(monthly_fee),
      daily_limit_(daily_limit),
      spent_today_(0.0) {
  std::cout << "    [2] CurrentAccount(...): картка " << card_number_
            << '\n';
}

CurrentAccount::~CurrentAccount() {
  std::cout << "    [~2] ~CurrentAccount(): картка " << card_number_ << '\n';
}

bool CurrentAccount::Withdraw(double amount, bool use_overdraft) {
  // Private-поле бази недоступне, лише через public-геттер.
  // ПОМИЛКА: if (!is_active_) — 'is_active_' is private within BankAccount.
  if (!is_active() || amount <= 0.0) {
    std::cout << "  [" << account_number_ << "] Відмова: рахунок закрито.\n";
    return false;
  }

  if (spent_today_ + amount > daily_limit_) {
    std::cout << std::fixed << std::setprecision(2) << "  ["
              << account_number_ << "] Відмова: перевищено денний ліміт "
              << daily_limit_ << ' ' << currency_ << '\n';
    return false;
  }

  // Protected-поле бази balance_ доступне в похідному класі.
  const double available =
      balance_ + (use_overdraft ? overdraft_limit_ : 0.0);
  if (available < amount) {
    std::cout << std::fixed << std::setprecision(2) << "  ["
              << account_number_ << "] Відмова: доступно лише " << available
              << ' ' << currency_ << '\n';
    return false;
  }

  balance_ -= amount;
  spent_today_ += amount;
  ++transaction_count_;
  std::cout << std::fixed << std::setprecision(2) << "  [" << account_number_
            << "] Оплата карткою " << card_number_ << ": " << amount << ' '
            << currency_ << (use_overdraft ? " (з овердрафтом)" : "")
            << ". Баланс: " << balance_ << ' ' << currency_ << '\n';
  return true;
}

void CurrentAccount::PrintInfo() const {
  // Виклик прихованого методу бази через оператор ::.
  BankAccount::PrintInfo();
  std::cout << std::fixed << std::setprecision(2) << "    поточний: картка "
            << card_number_ << " | овердрафт: " << overdraft_limit_
            << " | плата/міс.: " << monthly_fee_
            << " | денний ліміт: " << daily_limit_
            << " | витрачено сьогодні: " << spent_today_ << '\n';
}

void CurrentAccount::ChargeMonthlyFee() {
  balance_ -= monthly_fee_;
  ++transaction_count_;
  std::cout << std::fixed << std::setprecision(2) << "  [" << account_number_
            << "] Щомісячна плата " << monthly_fee_ << ' ' << currency_
            << ". Баланс: " << balance_ << ' ' << currency_ << '\n';
}

void CurrentAccount::StartNewDay() {
  spent_today_ = 0.0;
  std::cout << "  [" << account_number_ << "] Новий день: денний ліміт "
            << "оновлено.\n";
}

int CurrentAccount::AccountAgeYears(int current_year) const {
  // ПОМИЛКА: return current_year - opening_year_; — поле private у бази.
  return current_year - opening_year();
}

double CurrentAccount::MonthlyCost() const { return monthly_fee_; }

std::string CurrentAccount::TypeName() const { return "CurrentAccount"; }

double CurrentAccount::AvailableFunds() const {
  return std::min(balance_ + overdraft_limit_, daily_limit_ - spent_today_);
}

void CurrentAccount::Describe() const {
  FinancialProduct::Describe();
  BankAccount::DescribeOwn();
  DescribeOwn();
}

void CurrentAccount::DescribeOwn() const {
  std::cout << std::fixed << std::setprecision(2) << "    картка "
            << card_number_ << " | овердрафт: " << overdraft_limit_
            << " | доступно зараз: " << AvailableFunds() << ' ' << currency_
            << '\n';
}

void CurrentAccount::Write(std::ostream& out) const {
  WriteHeader(out);
  BankAccount::WriteOwn(out);
  WriteOwn(out);
}

void CurrentAccount::Read(std::istream& in) {
  ReadHeader(in);
  BankAccount::ReadOwn(in);
  ReadOwn(in);
}

void CurrentAccount::WriteOwn(std::ostream& out) const {
  out << card_number_ << '\n'
      << overdraft_limit_ << '\n'
      << monthly_fee_ << '\n'
      << daily_limit_ << '\n'
      << spent_today_ << '\n';
}

void CurrentAccount::ReadOwn(std::istream& in) {
  card_number_ = ReadField(in, "номер картки");
  overdraft_limit_ = ReadNumber(in, "овердрафт");
  monthly_fee_ = ReadNumber(in, "плата за місяць");
  daily_limit_ = ReadNumber(in, "денний ліміт");
  spent_today_ = ReadNumber(in, "витрачено сьогодні");
}
