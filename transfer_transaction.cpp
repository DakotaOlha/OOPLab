// Лабораторна робота №5, третій рівень, варіант 14 «Система Платежі».
// ЛР5 п.16: реалізація класу TransferTransaction.

#include "transfer_transaction.h"

#include <iomanip>
#include <iostream>

TransferTransaction::TransferTransaction(double amount,
                                         const std::string& from,
                                         const std::string& to)
    : LegacyTransaction(amount), from_(from), to_(to) {
  std::cout << "    TransferTransaction(): " << from_ << " -> " << to_
            << '\n';
}

TransferTransaction::~TransferTransaction() {
  std::cout << "    ~TransferTransaction(): " << from_ << " -> " << to_
            << '\n';
}

void TransferTransaction::Print() const {
  std::cout << std::fixed << std::setprecision(2) << "  Переказ " << amount_
            << " грн: " << from_ << " -> " << to_ << '\n';
}
