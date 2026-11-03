// Лабораторна робота №4, третій рівень, варіант 14 «Система Платежі».
// ЛР4 п.13: заголовний файл базового класу BankAccount.
//
// ЛР4 п.1: ієрархія класів:
//   BankAccount
//     +-- public    CurrentAccount
//     |               +-- private InstallmentPlan
//     |               +-- public  PremiumAccount (+ public Insurance,
//     |                                             public LoyaltyProgram)
//     +-- protected SavingsAccount
//     +-- private   CreditAccount
//
// Примітка: public-поля, protected/private- та множинне наслідування
// порушують Google C++ Style Guide, але їх вимагає завдання ЛР4.

#ifndef BANK_ACCOUNT_H_
#define BANK_ACCOUNT_H_

#include <string>

class BankAccount {
 public:
  // ЛР4 п.2: три public-поля бази для демонстрації керування доступом.
  std::string owner_name_;
  std::string bank_name_;
  std::string currency_;

  BankAccount(const std::string& owner_name, const std::string& bank_name,
              const std::string& currency, const std::string& account_number,
              double balance, int opening_year);
  // ЛР4 п.7, п.8: деструктор повідомляє, коли об'єкт знищується.
  ~BankAccount();

  // ЛР4 п.2: три public-методи бази для демонстрації керування доступом.
  bool Deposit(double amount);
  bool Withdraw(double amount);
  void PrintInfo() const;
  void Close();

  const std::string& account_number() const { return account_number_; }
  double balance() const { return balance_; }
  int transaction_count() const { return transaction_count_; }
  int opening_year() const { return opening_year_; }
  bool is_active() const { return is_active_; }

 protected:
  std::string account_number_;
  double balance_;
  int transaction_count_;

 private:
  // У похідних класах ці поля доступні лише через геттери.
  int opening_year_;
  bool is_active_;
};

#endif  // BANK_ACCOUNT_H_
