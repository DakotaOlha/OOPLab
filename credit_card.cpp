// Лабораторна робота №5, третій рівень, варіант 14 «Система Платежі».
// ЛР5 п.16: реалізація класу CreditCard.

#include "credit_card.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <limits>
#include <random>

namespace {

std::mt19937& RandomEngine() {
  static std::mt19937 engine(std::random_device{}());
  return engine;
}

int RandomInt(int min_value, int max_value) {
  std::uniform_int_distribution<int> dist(min_value, max_value);
  return dist(RandomEngine());
}

void SkipRestOfLine(std::ifstream& in) {
  in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

}  // namespace

// Визначення статичних полів (пам'ять виділяється один раз).
int CreditCard::alive_count_ = 0;
int CreditCard::created_count_ = 0;

CreditCard::CreditCard()
    : bank_name_("—"),
      card_type_("—"),
      expiry_year_(0),
      number_("0000 0000 0000 0000"),
      balance_(0.0),
      credit_limit_(0.0),
      transaction_count_(0),
      blocked_(false) {
  ++alive_count_;
  ++created_count_;
  std::cout << "    [CreditCard()] конструктор без параметрів: картка "
            << short_number() << '\n';
}

// Поля задаються списком ініціалізації.
CreditCard::CreditCard(const std::string& number, const std::string& bank_name,
                       const std::string& card_type, int expiry_year,
                       double credit_limit)
    : bank_name_(bank_name),
      card_type_(card_type),
      expiry_year_(expiry_year),
      number_(number),
      balance_(0.0),
      credit_limit_(credit_limit),
      transaction_count_(0),
      blocked_(false) {
  ++alive_count_;
  ++created_count_;
  std::cout << "    [CreditCard(...)] конструктор з параметрами: картка "
            << short_number() << '\n';
}

CreditCard::CreditCard(const CreditCard& other)
    : bank_name_(other.bank_name_),
      card_type_(other.card_type_),
      expiry_year_(other.expiry_year_),
      number_(other.number_),
      balance_(other.balance_),
      credit_limit_(other.credit_limit_),
      transaction_count_(other.transaction_count_),
      blocked_(other.blocked_) {
  ++alive_count_;
  ++created_count_;
  std::cout << "    [CreditCard(const CreditCard&)] копіювання картки "
            << short_number() << '\n';
}

CreditCard::~CreditCard() {
  --alive_count_;
  std::cout << "    [~CreditCard()] знищено картку " << short_number()
            << " (карток залишилось: " << alive_count_ << ")\n";
}

void CreditCard::PrintStatistics() {
  std::cout << "  Статистика CreditCard: існує " << alive_count_
            << ", створено за весь час " << created_count_ << '\n';
}

std::string CreditCard::short_number() const {
  if (number_.size() < 4)
    return number_;

  return "*" + number_.substr(number_.size() - 4);
}

bool CreditCard::Pay(double amount) {
  return Pay(amount, 0.0);
}

bool CreditCard::Pay(double amount, double commission_percent) {
  if (blocked_) {
    std::cout << "  [Картка " << short_number()
              << "] Відмова: картку заблоковано.\n";
    return false;
  }

  const double total = amount * (1.0 + commission_percent / 100.0);
  if (balance_ - total < -credit_limit_) {
    std::cout << "  [Картка " << short_number()
              << "] Відмова: недостатньо коштів з урахуванням ліміту.\n";
    return false;
  }

  balance_ -= total;
  ++transaction_count_;
  std::cout << std::fixed << std::setprecision(2) << "  [Картка "
            << short_number() << "] Списано " << total << " грн";
  if (commission_percent > 0.0) {
    std::cout << " (з них комісія " << total - amount << " грн)";
  }

  std::cout << ". Баланс: " << balance_ << " грн\n";

  return true;
}

void CreditCard::TopUp(double amount) {
  if (amount <= 0.0)
    return;

  balance_ += amount;
  ++transaction_count_;
  std::cout << std::fixed << std::setprecision(2) << "  [Картка "
            << short_number() << "] Зараховано " << amount
            << " грн. Баланс: " << balance_ << " грн\n";
}

bool CreditCard::TransferTo(CreditCard& other, double amount) {
  if (!Pay(amount, kTransferCommissionPercent))
    return false;

  other.TopUp(amount);
  return true;
}

CreditCard CreditCard::Reissue(int new_expiry_year) const {
  std::string new_number = number_.substr(0, number_.size() - 4);
  new_number += std::to_string(RandomInt(1000, 9999));
  CreditCard new_card(new_number, bank_name_, card_type_, new_expiry_year,
                      credit_limit_);
  new_card.balance_ = balance_;
  return new_card;
}

bool CreditCard::CheckCreditLimit() {
  if (balance_ < -credit_limit_) {
    blocked_ = true;
    std::cout << "  [Адміністратор] Картку " << short_number()
              << " заблоковано за перевищення кредиту!\n";
    return false;
  }

  std::cout << "  [Адміністратор] Картка " << short_number()
            << ": кредитний ліміт не перевищено.\n";

  return true;
}

void CreditCard::Block() {
  blocked_ = true;
  std::cout << "  [Картка " << short_number() << "] Заблоковано.\n";
}

void CreditCard::Unblock() {
  blocked_ = false;
  std::cout << "  [Картка " << short_number() << "] Розблоковано.\n";
}

void CreditCard::set_credit_limit(double credit_limit) {
  if (credit_limit >= 0.0)
    credit_limit_ = credit_limit;
}

void CreditCard::Print() const {
  std::cout << std::fixed << std::setprecision(2) << "    Картка " << number_
            << " | " << card_type_ << " | " << bank_name_ << " | до "
            << expiry_year_ << " | баланс: " << balance_
            << " | ліміт: " << credit_limit_
            << " | операцій: " << transaction_count_
            << " | стан: " << (blocked_ ? "заблокована" : "активна") << '\n';
}

void CreditCard::SaveToFile(std::ofstream& out) const {
  out << number_ << '\n'
      << bank_name_ << '\n'
      << card_type_ << '\n'
      << expiry_year_ << ' ' << balance_ << ' ' << credit_limit_ << ' '
      << transaction_count_ << ' ' << blocked_ << '\n';
}

bool CreditCard::LoadFromFile(std::ifstream& in) {
  if (!std::getline(in, number_))
    return false;

  std::getline(in, bank_name_);
  std::getline(in, card_type_);
  in >> expiry_year_ >> balance_ >> credit_limit_ >> transaction_count_ >>
      blocked_;
  SkipRestOfLine(in);
  return static_cast<bool>(in);
}

void CreditCard::SortRandomExpiryYears() const {
  const int size = RandomInt(5, 15);
  int* years = new int[size];
  for (int i = 0; i < size; ++i)
    years[i] = expiry_year_ + RandomInt(-5, 5);

  std::cout << "  Виділено пам'ять під " << size << " значень.\n"
            << "  До сортування:   ";

  for (int i = 0; i < size; ++i)
    std::cout << years[i] << ' ';

  std::sort(years, years + size);
  std::cout << "\n  Після сортування: ";
  for (int i = 0; i < size; ++i)
    std::cout << years[i] << ' ';

  std::cout << '\n';

  delete[] years;
}
