// Лабораторна робота №5, третій рівень, варіант 14 «Система Платежі».
// ЛР5 п.16: заголовний файл абстрактного класу FinancialProduct.
//
// ЛР5 п.1, п.2: ієрархія (virtual — віртуальне наслідування):
//   FinancialProduct (абстрактний)
//     +-- public virtual BankAccount (абстрактний)
//     |     +-- public    CurrentAccount
//     |     |               +-- private InstallmentPlan (+ public virtual FP)
//     |     |               +-- public  PremiumAccount
//     |     +-- protected SavingsAccount (+ public virtual FP)
//     |     +-- private   CreditAccount  (+ public virtual FP)
//     +-- public virtual Insurance ------------+-- public PremiumAccount
//     +-- public virtual LoyaltyProgram -------+   (ромб: один підоб'єкт FP)

#ifndef FINANCIAL_PRODUCT_H_
#define FINANCIAL_PRODUCT_H_

#include <iosfwd>
#include <string>

// ЛР5 п.1: абстрактний клас — спільний інтерфейс усіх фінансових продуктів.
class FinancialProduct {
 public:
  FinancialProduct(const std::string& owner, int start_year);
  // Копія — новий продукт: отримує власний номер і потрапляє в реєстр.
  FinancialProduct(const FinancialProduct& other);
  FinancialProduct& operator=(const FinancialProduct&) = delete;
  // ЛР5 п.3: віртуальний деструктор — тому деструктори всіх похідних
  // класів теж віртуальні, і delete через FinancialProduct* коректний.
  virtual ~FinancialProduct();

  // ЛР5 п.1, п.4: чисто віртуальні функції.
  virtual double MonthlyCost() const = 0;
  virtual std::string TypeName() const = 0;
  // ЛР5 п.11: чисто віртуальна, але має тіло (рядок-заголовок), яке
  // викликають напряму: product->FinancialProduct::Describe().
  virtual void Describe() const = 0;
  // ЛР5 п.7: запис полів у потік і читання з потоку.
  virtual void Write(std::ostream& out) const = 0;
  virtual void Read(std::istream& in) = 0;

  // ЛР5 п.1: статичні методи викликаються через клас.
  static void ShowAll();
  static int created_count() { return created_count_; }
  static int alive_count() { return alive_count_; }
  // Друк конструктора/деструктора FinancialProduct вмикається лише в ЛР5,
  // щоб вивід демонстрацій ЛР3 і ЛР4 не змінився.
  static void set_trace(bool trace) { trace_ = trace; }

  int id() const { return id_; }
  const std::string& owner() const { return owner_; }
  int start_year() const { return start_year_; }

 protected:
  // ЛР5 п.2: у ромбі кожен клас записує лише свої поля (WriteOwn/ReadOwn),
  // а поля FinancialProduct — один раз через WriteHeader/ReadHeader.
  void WriteHeader(std::ostream& out) const;
  void ReadHeader(std::istream& in);

  static std::string ReadField(std::istream& in, const std::string& name);
  static double ReadNumber(std::istream& in, const std::string& name);
  static int ReadInteger(std::istream& in, const std::string& name);

 private:
  static int created_count_;
  static int alive_count_;
  static bool trace_;

  int id_;
  std::string owner_;
  int start_year_;
};

#endif  // FINANCIAL_PRODUCT_H_
