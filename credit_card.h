// Лабораторна робота №6, третій рівень, варіант 14 «Система Платежі».
// ЛР6 п.1, п.3: заголовний файл класу CreditCard (оператори картки).

#ifndef CREDIT_CARD_H_
#define CREDIT_CARD_H_

#include <fstream>
#include <string>

constexpr double kTransferCommissionPercent = 1.0;
constexpr int kMinExpiryYear = 2026;  // строк дії не раніше поточного року

class CreditCard {
 public:
  std::string bank_name_;
  std::string card_type_;  // Visa, Mastercard
  int expiry_year_;

  // Конструктори без параметрів, з параметрами та копіювання.
  CreditCard();
  CreditCard(const std::string& number, const std::string& bank_name,
             const std::string& card_type, int expiry_year,
             double credit_limit);
  CreditCard(const CreditCard& other);
  // ЛР6 п.3: явний оператор присвоєння.
  CreditCard& operator=(const CreditCard& other);
  // Деструктор показує, коли об'єкт виходить з блоку програми.
  ~CreditCard();

  // ЛР6 п.1: ++/-- продовжують/скорочують строк дії картки на рік.
  // Префіксна форма змінює об'єкт і повертає посилання на нього (без копії);
  // постфіксна повертає копію стану ДО зміни, тому повертає значення.
  // Фіктивний параметр int лише відрізняє постфіксну форму від префіксної.
  CreditCard& operator++();
  CreditCard operator++(int);
  CreditCard& operator--();
  CreditCard operator--(int);

  // ЛР6 п.3: += поповнення, -= оплата, *= і * – зміна кредитного ліміту.
  CreditCard& operator+=(double amount);
  CreditCard& operator-=(double amount);
  CreditCard& operator*=(double factor);
  CreditCard operator*(double factor) const;

  bool Pay(double amount);
  bool Pay(double amount, double commission_percent);
  void TopUp(double amount);
  bool TransferTo(CreditCard& other, double amount);
  CreditCard Reissue(int new_expiry_year) const;
  bool CheckCreditLimit();

  void Block();
  void Unblock();
  void Print() const;

  void SaveToFile(std::ofstream& out) const;
  bool LoadFromFile(std::ifstream& in);

  void SortRandomExpiryYears() const;

  const std::string& number() const { return number_; }
  std::string short_number() const;
  double balance() const { return balance_; }
  double credit_limit() const { return credit_limit_; }
  int transaction_count() const { return transaction_count_; }
  bool is_blocked() const { return blocked_; }
  void set_credit_limit(double credit_limit);

  // Статичні методи викликаються без об'єкта і працюють лише
  // зі статичними полями.
  static int alive_count() { return alive_count_; }
  static int created_count() { return created_count_; }
  static void PrintStatistics();

 private:
  std::string number_;
  double balance_;  // від'ємний баланс = використаний кредит
  double credit_limit_;
  int transaction_count_;
  bool blocked_;

  // Статичні поля — одні на всі об'єкти класу.
  static int alive_count_;
  static int created_count_;
};

#endif  // CREDIT_CARD_H_
