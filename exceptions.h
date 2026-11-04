// Лабораторна робота №5, третій рівень, варіант 14 «Система Платежі».
// ЛР5 п.16: заголовний файл власних класів винятків.

#ifndef EXCEPTIONS_H_
#define EXCEPTIONS_H_

#include <stdexcept>
#include <string>

// ЛР5 п.5, п.6: помилки роботи з динамічною пам'яттю та масивами.
class MemoryError : public std::runtime_error {
 public:
  explicit MemoryError(const std::string& message);
};

// ЛР5 п.7: помилки роботи з файлами.
class FileError : public std::runtime_error {
 public:
  explicit FileError(const std::string& message);
};

// ЛР5 п.6, п.8: об'єкт не знайдено (зокрема при повторному видаленні).
class NotFoundError : public std::runtime_error {
 public:
  explicit NotFoundError(const std::string& message);
};

#endif  // EXCEPTIONS_H_
