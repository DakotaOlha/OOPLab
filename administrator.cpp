// Лабораторна робота №3, третій рівень, варіант 14 «Система Платежі».
// п.17: cpp-файл з реалізацією класу Administrator.

#include "administrator.h"

#include <iomanip>
#include <iostream>

#include "client.h"

Administrator::Administrator(const std::string& name) : name_(name) {
  std::cout << "    [Administrator(...)] адміністратор " << name_
            << " почав роботу\n";
}

// п.12: при знищенні адміністратора клієнти НЕ знищуються (агрегація).
Administrator::~Administrator() {
  std::cout << "    [~Administrator()] адміністратор " << name_
            << " завершив роботу; клієнтів у списку було: "
            << clients_.size() << " (вони продовжують існувати)\n";
}

void Administrator::AddClient(Client* client) {
  if (client == nullptr) return;
  clients_.push_back(client);
  std::cout << "  Адміністратор " << name_ << " обслуговує клієнта "
            << client->last_name_ << '\n';
}

void Administrator::CheckAllCards() {
  std::cout << "  Адміністратор " << name_ << " перевіряє картки:\n";
  for (Client* client : clients_) client->mutable_card().CheckCreditLimit();
}

// Запит клієнта на зміну кредитного ліміту (варіант 14).
// Правило: зменшення ліміту — завжди; збільшення — якщо картка не
// заблокована, а на рахунку є щонайменше 20% від нового ліміту.
bool Administrator::ReviewLimitRequest(Client* client, double new_limit) {
  if (client == nullptr) return false;
  CreditCard& card = client->mutable_card();
  std::cout << std::fixed << std::setprecision(2) << "  Запит "
            << client->last_name_ << ": змінити ліміт " << card.credit_limit()
            << " -> " << new_limit << " грн\n";

  const bool approved =
      new_limit <= card.credit_limit() ||
      (!card.is_blocked() && client->account_balance() >= 0.2 * new_limit);
  if (approved) {
    card.set_credit_limit(new_limit);
    std::cout << "  Адміністратор " << name_ << ": ЗАТВЕРДЖЕНО\n";
  } else {
    std::cout << "  Адміністратор " << name_
              << ": ВІДХИЛЕНО (замало коштів на рахунку або картку "
                 "заблоковано)\n";
  }
  return approved;
}

void Administrator::PrintClients() const {
  std::cout << "  Клієнти адміністратора " << name_ << ":\n";
  for (const Client* client : clients_) client->Print();
}
