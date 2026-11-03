// Лабораторна робота №4, третій рівень, варіант 14 «Система Платежі».
// ЛР4 п.13: реалізація базового класу BankAccount.

#include "bank_account.h"

#include <iomanip>
#include <iostream>

BankAccount::BankAccount(const std::string& owner_name,
                         const std::string& bank_name,
                         const std::string& currency,
                         const std::string& account_number, double balance,
                         int opening_year)
    : owner_name_(owner_name),
      bank_name_(bank_name),
      currency_(currency),
      account_number_(account_number),
      balance_(balance),
      transaction_count_(0),
      opening_year_(opening_year),
      is_active_(true) {
  std::cout << "    [1] BankAccount(...): рахунок " << account_number_
            << " (" << owner_name_ << ")\n";
}

BankAccount::~BankAccount() {
  std::cout << "    [~1] ~BankAccount(): рахунок " << account_number_
            << " (" << owner_name_ << ")\n";
}

bool BankAccount::Deposit(double amount) {
  if (!is_active_ || amount <= 0.0) {
    std::cout << "  [" << account_number_ << "] Відмова у зарахуванні.\n";
    return false;
  }

  balance_ += amount;
  ++transaction_count_;
  std::cout << std::fixed << std::setprecision(2) << "  [" << account_number_
            << "] Зараховано " << amount << ' ' << currency_
            << ". Баланс: " << balance_ << ' ' << currency_ << '\n';
  return true;
}

bool BankAccount::Withdraw(double amount) {
  if (!is_active_ || amount <= 0.0 || balance_ < amount) {
    std::cout << std::fixed << std::setprecision(2) << "  ["
              << account_number_ << "] Відмова у списанні " << amount << ' '
              << currency_ << ": недостатньо коштів.\n";
    return false;
  }

  balance_ -= amount;
  ++transaction_count_;
  std::cout << std::fixed << std::setprecision(2) << "  [" << account_number_
            << "] Списано " << amount << ' ' << currency_
            << ". Баланс: " << balance_ << ' ' << currency_ << '\n';
  return true;
}

void BankAccount::PrintInfo() const {
  std::cout << std::fixed << std::setprecision(2) << "  Рахунок "
            << account_number_ << " | " << owner_name_ << " | " << bank_name_
            << " | баланс: " << balance_ << ' ' << currency_
            << " | відкрито: " << opening_year_
            << " | операцій: " << transaction_count_ << " | "
            << (is_active_ ? "активний" : "закритий") << '\n';
}

void BankAccount::Close() {
  is_active_ = false;
  std::cout << "  [" << account_number_ << "] Рахунок закрито.\n";
}
