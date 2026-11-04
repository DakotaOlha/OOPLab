// Лабораторна робота №5, третій рівень, варіант 14 «Система Платежі».
// ЛР5 п.16: реалізація класу LegacyTransaction.

#include "legacy_transaction.h"

#include <iomanip>
#include <iostream>

LegacyTransaction::LegacyTransaction(double amount) : amount_(amount) {
  std::cout << "    LegacyTransaction(): транзакція на " << std::fixed
            << std::setprecision(2) << amount_ << " грн\n";
}

LegacyTransaction::~LegacyTransaction() {
  std::cout << "    ~LegacyTransaction()\n";
}

void LegacyTransaction::Print() const {
  std::cout << std::fixed << std::setprecision(2) << "  Транзакція на "
            << amount_ << " грн\n";
}
