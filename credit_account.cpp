// Лабораторна робота №4, третій рівень, варіант 14 «Система Платежі».
// ЛР4 п.13: реалізація класу CreditAccount.

#include "credit_account.h"

#include <iomanip>
#include <iostream>

// ЛР4 п.5: 6 параметрів передаються базі, 3 — власним полям.
CreditAccount::CreditAccount(const std::string& owner_name,
                             const std::string& bank_name,
                             const std::string& currency,
                             const std::string& account_number,
                             double balance, int opening_year,
                             double credit_limit, double interest_rate,
                             int overdue_days)
    : FinancialProduct(owner_name, opening_year),
      BankAccount(owner_name, bank_name, currency, account_number, balance,
                  opening_year),
      credit_limit_(credit_limit),
      interest_rate_(interest_rate),
      debt_(0.0),
      overdue_days_(overdue_days) {
  std::cout << std::fixed << std::setprecision(2)
            << "    [2] CreditAccount(...): кредитний ліміт "
            << credit_limit_ << ' ' << currency_ << '\n';
}

CreditAccount::~CreditAccount() {
  std::cout << "    [~2] ~CreditAccount(): кредит " << account_number_
            << '\n';
}

bool CreditAccount::Borrow(double amount) {
  if (overdue_days_ > 0 || debt_ + amount > credit_limit_) {
    std::cout << "  [" << account_number_ << "] Відмова у кредиті: "
              << "прострочення або перевищення ліміту.\n";
    return false;
  }

  debt_ += amount;
  std::cout << std::fixed << std::setprecision(2) << "  [" << account_number_
            << "] Видано кредит " << amount << ' ' << currency_
            << ". Борг: " << debt_ << ' ' << currency_ << '\n';
  return Deposit(amount);
}

void CreditAccount::Repay(double amount) {
  if (amount > debt_)
    amount = debt_;

  debt_ -= amount;
  ++transaction_count_;
  if (debt_ == 0.0)
    overdue_days_ = 0;

  std::cout << std::fixed << std::setprecision(2) << "  [" << account_number_
            << "] Погашено " << amount << ' ' << currency_
            << ". Залишок боргу: " << debt_ << ' ' << currency_ << '\n';
}

bool CreditAccount::TransferTo(BankAccount& target, double amount) {
  std::cout << "  Переказ з кредитного рахунку " << account_number_
            << " на " << target.account_number() << ":\n";
  if (!Withdraw(amount))
    return false;

  return target.Deposit(amount);
}

void CreditAccount::AccrueMonthlyInterest() {
  const double interest = debt_ * interest_rate_ / 100.0;
  debt_ += interest;
  std::cout << std::fixed << std::setprecision(2) << "  [" << account_number_
            << "] Нараховано відсотки за кредитом: " << interest << ' '
            << currency_ << ". Борг: " << debt_ << ' ' << currency_ << '\n';
}

void CreditAccount::Print() const {
  PrintInfo();
  std::cout << std::fixed << std::setprecision(2) << "    кредит: ліміт "
            << credit_limit_ << " | ставка: " << interest_rate_
            << "%/міс. | борг: " << debt_
            << " | прострочено днів: " << overdue_days_ << '\n';
}

double CreditAccount::MonthlyCost() const {
  return debt_ * interest_rate_ / 100.0;
}

std::string CreditAccount::TypeName() const { return "CreditAccount"; }

bool CreditAccount::CanBeClosed() const {
  return debt_ == 0.0 && BankAccount::CanBeClosed();
}

void CreditAccount::Describe() const {
  FinancialProduct::Describe();
  BankAccount::DescribeOwn();
  std::cout << std::fixed << std::setprecision(2) << "    кредит: борг "
            << debt_ << " з " << credit_limit_ << " | ставка: "
            << interest_rate_ << "%/міс.\n";
}

void CreditAccount::Write(std::ostream& out) const {
  WriteHeader(out);
  BankAccount::WriteOwn(out);
  out << credit_limit_ << '\n'
      << interest_rate_ << '\n'
      << debt_ << '\n'
      << overdue_days_ << '\n';
}

void CreditAccount::Read(std::istream& in) {
  ReadHeader(in);
  BankAccount::ReadOwn(in);
  credit_limit_ = ReadNumber(in, "кредитний ліміт");
  interest_rate_ = ReadNumber(in, "ставка");
  debt_ = ReadNumber(in, "борг");
  overdue_days_ = ReadInteger(in, "днів прострочення");
}
