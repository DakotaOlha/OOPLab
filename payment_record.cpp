// Лабораторна робота №6, третій рівень, варіант 14 «Система Платежі».
// ЛР6 п.6: реалізація класу PaymentRecord.

#include "payment_record.h"

#include <cstddef>
#include <iomanip>
#include <ostream>
#include <string>

#include "exceptions.h"

namespace {

constexpr char kRangeSeparator[] = "..";

std::string MergeName(const std::string& a, const std::string& b) {
  return a == b ? a : "різні";
}

// Підсумок охоплює період «перша дата..остання дата».
std::string MergeDates(const std::string& first, const std::string& last) {
  const std::string from = first.substr(0, first.find(kRangeSeparator));
  const std::size_t pos = last.rfind(kRangeSeparator);
  const std::string to =
      pos == std::string::npos ? last : last.substr(pos + 2);
  return from + kRangeSeparator + to;
}

}  // namespace

PaymentRecord::PaymentRecord()
    : date_("—"),
      amount_(0.0),
      type_(PaymentType::kCard),
      sender_("—"),
      receiver_("—") {}

PaymentRecord::PaymentRecord(const std::string& date, double amount,
                             PaymentType type, const std::string& sender,
                             const std::string& receiver)
    : date_(date),
      amount_(amount),
      type_(type),
      sender_(sender),
      receiver_(receiver) {}

PaymentRecord PaymentRecord::operator+(const PaymentRecord& other) const {
  return PaymentRecord(MergeDates(date_, other.date_), amount_ + other.amount_,
                       type_, MergeName(sender_, other.sender_),
                       MergeName(receiver_, other.receiver_));
}

PaymentRecord PaymentRecord::operator/(int count) const {
  if (count <= 0)
    throw NotFoundError("середній платіж: кількість має бути додатною");
  PaymentRecord result(*this);
  result.amount_ = amount_ / count;
  return result;
}

bool PaymentRecord::operator<(const PaymentRecord& other) const {
  return amount_ < other.amount_;
}

bool PaymentRecord::operator>(const PaymentRecord& other) const {
  return other < *this;
}

bool PaymentRecord::operator==(const PaymentRecord& other) const {
  return date_ == other.date_ && amount_ == other.amount_ &&
         type_ == other.type_ && sender_ == other.sender_ &&
         receiver_ == other.receiver_;
}

bool PaymentRecord::operator!=(const PaymentRecord& other) const {
  return !(*this == other);
}

std::ostream& operator<<(std::ostream& out, const PaymentRecord& record) {
  out << std::fixed << std::setprecision(2) << record.date_ << " | "
      << record.amount_ << " грн | "
      << (record.type_ == PaymentType::kCard ? "картка" : "рахунок")
      << " | " << record.sender_ << " → " << record.receiver_;
  return out;
}
