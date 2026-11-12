// Лабораторна робота №5, третій рівень, варіант 14 «Система Платежі».
// ЛР5 п.16: реалізація класу Client.

#include "client.h"

#include <iomanip>
#include <iostream>
#include <limits>

namespace {

void SkipRestOfLine(std::ifstream& in) {
  in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

}  // namespace

// Визначення статичних полів.
double Client::commission_percent_ = 1.0;
int Client::alive_count_ = 0;

Client::Client()
    : first_name_("—"),
      last_name_("Невідомий"),
      phone_("—"),
      client_id_(0),
      account_number_("—"),
      account_balance_(0.0),
      account_active_(false),
      card_() {
  ++alive_count_;
  std::cout << "    [Client()] конструктор без параметрів: клієнт "
            << last_name_ << '\n';
}

// Поля і вкладений об'єкт card_ задаються списком
// ініціалізації.
Client::Client(int client_id, const std::string& first_name,
               const std::string& last_name, const std::string& phone,
               const std::string& account_number, double account_balance,
               const CreditCard& card)
    : first_name_(first_name),
      last_name_(last_name),
      phone_(phone),
      client_id_(client_id),
      account_number_(account_number),
      account_balance_(account_balance),
      account_active_(true),
      card_(card) {
  ++alive_count_;
  std::cout << "    [Client(...)] конструктор з параметрами: клієнт "
            << last_name_ << '\n';
}

Client::Client(const Client& other)
    : first_name_(other.first_name_),
      last_name_(other.last_name_),
      phone_(other.phone_),
      client_id_(other.client_id_),
      account_number_(other.account_number_),
      account_balance_(other.account_balance_),
      account_active_(other.account_active_),
      card_(other.card_) {
  ++alive_count_;
  std::cout << "    [Client(const Client&)] копіювання клієнта "
            << last_name_ << '\n';
}

Client::~Client() {
  --alive_count_;
  std::cout << "    [~Client()] знищено клієнта " << last_name_
            << " (клієнтів залишилось: " << alive_count_ << ")\n";
}

void Client::set_commission_percent(double percent) {
  if (percent >= 0.0)
    commission_percent_ = percent;
}

bool Client::PayOrder(double amount) {
  std::cout << "  " << last_name_ << " оплачує " << amount
            << " грн карткою.\n";
  return card_.Pay(amount);
}

bool Client::PayOrder(double amount, bool from_account) {
  if (!from_account)
    return PayOrder(amount);

  std::cout << "  " << last_name_ << " оплачує " << amount
            << " грн з рахунку " << account_number_ << ".\n";
  if (!account_active_) {
    std::cout << "  Відмова: рахунок анульовано.\n";
    return false;
  }

  if (account_balance_ < amount) {
    std::cout << "  Відмова: недостатньо коштів на рахунку.\n";
    return false;
  }

  account_balance_ -= amount;
  std::cout << std::fixed << std::setprecision(2)
            << "  Оплачено. Залишок на рахунку: " << account_balance_
            << " грн\n";

  return true;
}

bool Client::TransferToClient(Client& other, double amount) {
  const double total = amount * (1.0 + commission_percent_ / 100.0);

  std::cout << std::fixed << std::setprecision(2) << "  Платіж " << amount
            << " грн: " << last_name_ << " -> " << other.last_name_
            << " (комісія " << commission_percent_ << "%)\n";
  if (!account_active_ || !other.account_active_) {
    std::cout << "  Відмова: один із рахунків анульовано.\n";
    return false;
  }

  if (account_balance_ < total) {
    std::cout << "  Відмова: недостатньо коштів на рахунку.\n";
    return false;
  }

  account_balance_ -= total;
  other.account_balance_ += amount;
  std::cout << "  Успішно. Списано " << total << " грн\n";

  return true;
}

Client Client::ReissueCard(int new_expiry_year) const {
  Client updated(*this);
  updated.card_ = card_.Reissue(new_expiry_year);
  std::cout << "  Картку клієнта " << last_name_ << " перевипущено: "
            << card_.short_number() << " -> " << updated.card_.short_number()
            << '\n';

  return updated;
}

void Client::BlockCard() {
  std::cout << "  " << last_name_ << " блокує свою картку.\n";
  card_.Block();
}

void Client::CloseAccount() {
  std::cout << std::fixed << std::setprecision(2) << "  Рахунок "
            << account_number_ << " клієнта " << last_name_
            << " анульовано. Повернуто залишок: " << account_balance_
            << " грн\n";
  account_balance_ = 0.0;
  account_active_ = false;
  card_.Block();
}

void Client::Print() const {
  std::cout << std::fixed << std::setprecision(2) << "  Клієнт #"
            << client_id_ << ": " << last_name_ << ' ' << first_name_
            << " | тел. " << phone_ << " | рахунок " << account_number_
            << ": " << account_balance_ << " грн ("
            << (account_active_ ? "активний" : "анульований") << ")\n";
  card_.Print();
}

void Client::SaveToFile(std::ofstream& out) const {
  out << client_id_ << '\n'
      << first_name_ << '\n'
      << last_name_ << '\n'
      << phone_ << '\n'
      << account_number_ << '\n'
      << account_balance_ << ' ' << account_active_ << '\n';
  card_.SaveToFile(out);
}

bool Client::LoadFromFile(std::ifstream& in) {
  if (!(in >> client_id_))
    return false;

  SkipRestOfLine(in);

  std::getline(in, first_name_);
  std::getline(in, last_name_);
  std::getline(in, phone_);
  std::getline(in, account_number_);

  in >> account_balance_ >> account_active_;
  SkipRestOfLine(in);
  return card_.LoadFromFile(in);
}
