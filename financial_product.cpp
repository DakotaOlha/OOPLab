// Лабораторна робота №5, третій рівень, варіант 14 «Система Платежі».
// ЛР5 п.16: реалізація абстрактного класу FinancialProduct.

#include "financial_product.h"

#include <algorithm>
#include <climits>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

#include "exceptions.h"

namespace {

std::vector<const FinancialProduct*>& Registry() {
  static std::vector<const FinancialProduct*> registry;
  return registry;
}

}  // namespace

int FinancialProduct::created_count_ = 0;
int FinancialProduct::alive_count_ = 0;

FinancialProduct::FinancialProduct(const std::string& owner, int start_year)
    : id_(++created_count_), owner_(owner), start_year_(start_year) {
  ++alive_count_;
  Registry().push_back(this);
  std::cout << "    [0] FinancialProduct(...): продукт №" << id_ << " ("
            << owner_ << ")\n";
}

FinancialProduct::FinancialProduct(const FinancialProduct& other)
    : id_(++created_count_),
      owner_(other.owner_),
      start_year_(other.start_year_) {
  ++alive_count_;
  Registry().push_back(this);
  std::cout << "    [0] FinancialProduct(копія №" << other.id_
            << "): продукт №" << id_ << '\n';
}

FinancialProduct::~FinancialProduct() {
  --alive_count_;
  std::vector<const FinancialProduct*>& registry = Registry();
  registry.erase(std::remove(registry.begin(), registry.end(), this),
                 registry.end());
  std::cout << "    [~0] ~FinancialProduct(): продукт №" << id_ << '\n';
}

void FinancialProduct::Describe() const {
  std::cout << std::fixed << std::setprecision(2) << "  №" << id_ << ' '
            << TypeName() << " | власник: " << owner_ << " | з "
            << start_year_ << " р. | вартість/міс.: " << MonthlyCost()
            << " грн\n";
}

void FinancialProduct::ShowAll() {
  std::cout << "  Створено продуктів: " << created_count_
            << ", живих: " << alive_count_ << '\n';
  for (const FinancialProduct* product : Registry()) {
    // ЛР5 п.11: обхід віртуального механізму — лише рядок-заголовок.
    product->FinancialProduct::Describe();
  }
}

void FinancialProduct::WriteHeader(std::ostream& out) const {
  out << std::fixed << std::setprecision(2) << TypeName() << '\n'
      << id_ << '\n'
      << owner_ << '\n'
      << start_year_ << '\n';
}

void FinancialProduct::ReadHeader(std::istream& in) {
  const std::string type = ReadField(in, "тип");
  if (type != TypeName()) {
    // ЛР5 п.7: виняток — у файлі записано продукт іншого типу.
    throw FileError("у файлі продукт типу «" + type + "», а очікувався «" +
                    TypeName() + "»");
  }
  // Номер — ідентичність живого об'єкта в реєстрі, тому лише перевіряється.
  ReadInteger(in, "номер");
  owner_ =ReadField(in, "власник");
  start_year_ = ReadInteger(in, "рік");
}

std::string FinancialProduct::ReadField(std::istream& in,
                                        const std::string& name) {
  std::string value;
  if (!std::getline(in, value)) {
    // ЛР5 п.7: виняток — неповні дані (файл закінчився раніше).
    throw FileError("неповні дані: бракує поля «" + name + "»");
  }
  if (!value.empty() && value.back() == '\r')
    value.pop_back();
  return value;
}

double FinancialProduct::ReadNumber(std::istream& in,
                                    const std::string& name) {
  const std::string text = ReadField(in, name);
  std::size_t parsed = 0;
  double value = 0.0;
  try {
    value = std::stod(text, &parsed);
  } catch (const std::exception&) {
    parsed = 0;
  }
  if (parsed == 0 || parsed != text.size()) {
    // ЛР5 п.7: виняток — пошкоджені дані (не число).
    throw FileError("пошкоджені дані у полі «" + name + "»: \"" + text +
                    "\"");
  }
  return value;
}

int FinancialProduct::ReadInteger(std::istream& in, const std::string& name) {
  const double value = ReadNumber(in, name);
  if (value < INT_MIN || value > INT_MAX || value != std::floor(value))
    throw FileError("поле «" + name + "» має бути цілим числом");
  return static_cast<int>(value);
}
