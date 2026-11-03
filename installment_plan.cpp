// Лабораторна робота №4, третій рівень, варіант 14 «Система Платежі».
// ЛР4 п.13: реалізація класу InstallmentPlan.

#include "installment_plan.h"

#include <iomanip>
#include <iostream>

namespace {

constexpr char kInstallmentCurrency[] = "грн";
constexpr char kInstallmentCard[] = "(розстрочка)";
constexpr double kInstallmentMonthlyFee = 0.0;

}  // namespace

// ЛР4 п.5: базі CurrentAccount передається 10 аргументів (6 параметрів
// конструктора і 4 значення плану), решта параметрів — власним полям.
InstallmentPlan::InstallmentPlan(const std::string& owner_name,
                                 const std::string& bank_name,
                                 const std::string& account_number,
                                 double balance, int opening_year,
                                 double overdraft_limit,
                                 const std::string& purchase_description,
                                 double total_amount, int parts_count,
                                 int first_payment_month)
    : CurrentAccount(owner_name, bank_name, kInstallmentCurrency,
                     account_number, balance, opening_year, kInstallmentCard,
                     overdraft_limit, kInstallmentMonthlyFee, total_amount),
      purchase_description_(purchase_description),
      total_amount_(total_amount),
      parts_count_(parts_count),
      paid_parts_(0),
      first_payment_month_(first_payment_month) {
  std::cout << "    [3] InstallmentPlan(...): «" << purchase_description_
            << "», кількість частин: " << parts_count_ << '\n';
}

InstallmentPlan::~InstallmentPlan() {
  std::cout << "    [~3] ~InstallmentPlan(): «" << purchase_description_
            << "»\n";
}

bool InstallmentPlan::PayNextPart() {
  if (is_completed()) {
    std::cout << "  План «" << purchase_description_ << "» уже виконано.\n";
    return false;
  }

  const double part = total_amount_ / parts_count_;
  std::cout << "  " << owner_name_ << " сплачує частину " << paid_parts_ + 1
            << '/' << parts_count_ << " за «" << purchase_description_
            << "»:\n";
  if (!CurrentAccount::Withdraw(part, true))
    return false;

  ++paid_parts_;
  return true;
}

bool InstallmentPlan::TopUp(double amount) {
  return BankAccount::Deposit(amount);
}

void InstallmentPlan::PrintSchedule() const {
  CurrentAccount::PrintInfo();
  const double part = total_amount_ / parts_count_;
  std::cout << std::fixed << std::setprecision(2) << "    План «"
            << purchase_description_ << "»: " << total_amount_ << " грн, "
            << parts_count_ << " част. по " << part << " грн\n";
  for (int i = 0; i < parts_count_; ++i) {
    const int month = (first_payment_month_ + i - 1) % 12 + 1;
    std::cout << "      частина " << i + 1 << " — місяць " << month << ": "
              << (i < paid_parts_ ? "оплачено" : "очікується") << '\n';
  }
  std::cout << "    Залишилось сплатити: "
            << part * (parts_count_ - paid_parts_) << " грн\n";
}
