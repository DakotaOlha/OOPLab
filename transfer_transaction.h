// Лабораторна робота №5, третій рівень, варіант 14 «Система Платежі».
// ЛР5 п.16: заголовний файл класу TransferTransaction (переказ).

#ifndef TRANSFER_TRANSACTION_H_
#define TRANSFER_TRANSACTION_H_

#include <string>

#include "legacy_transaction.h"

// ЛР5 п.3: похідний від класу з невіртуальним деструктором — при delete
// через LegacyTransaction* цей деструктор НЕ викликається.
class TransferTransaction : public LegacyTransaction {
 public:
  TransferTransaction(double amount, const std::string& from,
                      const std::string& to);
  ~TransferTransaction();

  // ЛР5 п.4: інша поведінка — друкує маршрут переказу.
  void Print() const override;

 private:
  std::string from_;
  std::string to_;
};

#endif  // TRANSFER_TRANSACTION_H_
