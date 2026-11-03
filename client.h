// Лабораторна робота №3, варіант 14 «Система Платежі».
// п.17: заголовний файл класу Client.

#ifndef CLIENT_H_
#define CLIENT_H_

#include <fstream>
#include <string>

#include "credit_card.h"

class Client {
 public:
  std::string first_name_;
  std::string last_name_;
  std::string phone_;

  // п.1: конструктор без параметрів.
  Client();
  // п.1: конструктор з параметрами.
  Client(int client_id, const std::string& first_name,
         const std::string& last_name, const std::string& phone,
         const std::string& account_number, double account_balance,
         const CreditCard& card);
  // п.1: конструктор копіювання.
  Client(const Client& other);
  Client& operator=(const Client& other) = default;
  // п.3: деструктор — показує вихід об'єкта з блоку програми.
  ~Client();

  bool PayOrder(double amount);
  bool PayOrder(double amount, bool from_account);
  bool TransferToClient(Client& other, double amount);
  Client ReissueCard(int new_expiry_year) const;
  void BlockCard();
  void CloseAccount();

  void Print() const;

  void SaveToFile(std::ofstream& out) const;
  bool LoadFromFile(std::ifstream& in);

  int client_id() const { return client_id_; }
  const std::string& account_number() const { return account_number_; }
  double account_balance() const { return account_balance_; }
  bool is_account_active() const { return account_active_; }
  const CreditCard& card() const { return card_; }
  CreditCard& mutable_card() { return card_; }

  // п.4: статичні методи — спільна комісія для всіх клієнтів банку.
  static double commission_percent() { return commission_percent_; }
  static void set_commission_percent(double percent);
  static int alive_count() { return alive_count_; }

 private:
  int client_id_;
  std::string account_number_;
  double account_balance_;
  bool account_active_;
  // п.13: композиція — картка є частиною клієнта і живе стільки ж, скільки клієнт
  CreditCard card_;

  // п.4: статичні поля — одні на всі об'єкти класу Client.
  static double commission_percent_;
  static int alive_count_;
};

#endif
