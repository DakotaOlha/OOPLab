// Лабораторна робота №5, третій рівень, варіант 14 «Система Платежі».
// ЛР5 п.16: заголовний файл класу LegacyTransaction (стара транзакція).

#ifndef LEGACY_TRANSACTION_H_
#define LEGACY_TRANSACTION_H_

// ЛР5 п.3: навчальний клас з НЕвіртуальним деструктором — для порівняння
// з ієрархією FinancialProduct, де деструктор віртуальний.
class LegacyTransaction {
 public:
  explicit LegacyTransaction(double amount);
  ~LegacyTransaction();  // навмисно без virtual

  // ЛР5 п.4: віртуальна функція класу з відкритим наслідуванням.
  virtual void Print() const;

 protected:
  double amount_;
};

#endif  // LEGACY_TRANSACTION_H_
