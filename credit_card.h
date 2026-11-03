// Лабораторна робота №3, третій рівень, варіант 14 «Система Платежі».
// п.17: заголовний файл класу CreditCard (кредитна картка).
//
// Примітка до п.16 (Google C++ Style): стиль рекомендує робити всі поля
// класу приватними. Три public-поля залишені з ЛР2, бо цього вимагало
// завдання ЛР2 (3 загальні + 5 приватних елементів).

#ifndef CREDIT_CARD_H_
#define CREDIT_CARD_H_

#include <fstream>
#include <string>

// Комісія (у відсотках) за переказ з картки на картку.
constexpr double kTransferCommissionPercent = 1.0;

class CreditCard {
 public:
  // 3 загальні (public) елементи.
  std::string bank_name_;  // назва банку
  std::string card_type_;  // платіжна система (Visa, Mastercard)
  int expiry_year_;        // рік закінчення дії

  // п.1: конструктор без параметрів.
  CreditCard();
  // п.1: конструктор з параметрами.
  CreditCard(const std::string& number, const std::string& bank_name,
             const std::string& card_type, int expiry_year,
             double credit_limit);
  // п.1: конструктор копіювання.
  CreditCard(const CreditCard& other);
  CreditCard& operator=(const CreditCard& other) = default;
  // п.3: деструктор — показує, коли об'єкт виходить з блоку програми.
  ~CreditCard();

  // Методи обробки даних (з ЛР2).
  bool Pay(double amount);
  bool Pay(double amount, double commission_percent);
  void TopUp(double amount);
  bool TransferTo(CreditCard& other, double amount);
  CreditCard Reissue(int new_expiry_year) const;
  bool CheckCreditLimit();

  void Block();
  void Unblock();
  void Print() const;

  // Запис у файл та читання з файлу (з ЛР2).
  void SaveToFile(std::ofstream& out) const;
  bool LoadFromFile(std::ifstream& in);

  // Випадкова динамічна пам'ять + сортування (з ЛР2).
  void SortRandomExpiryYears() const;

  // Методи доступу до приватних полів.
  const std::string& number() const { return number_; }
  std::string short_number() const;
  double balance() const { return balance_; }
  double credit_limit() const { return credit_limit_; }
  int transaction_count() const { return transaction_count_; }
  bool is_blocked() const { return blocked_; }
  void set_credit_limit(double credit_limit);

  // п.4: статичні методи класу — викликаються без об'єкта:
  // CreditCard::alive_count(), CreditCard::PrintStatistics().
  static int alive_count() { return alive_count_; }
  static int created_count() { return created_count_; }
  static void PrintStatistics();

 private:
  // 5 приватних елементів.
  std::string number_;     // номер картки
  double balance_;         // баланс (від'ємний = використаний кредит)
  double credit_limit_;    // кредитний ліміт
  int transaction_count_;  // кількість операцій
  bool blocked_;           // чи заблокована картка

  // п.4: статичні поля — одні на всі об'єкти класу.
  static int alive_count_;    // скільки карток існує зараз
  static int created_count_;  // скільки карток створено за весь час
};

#endif  // CREDIT_CARD_H_
