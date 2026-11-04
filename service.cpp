// Лабораторна робота №5, третій рівень, варіант 14 «Система Платежі».
// ЛР5 п.16: реалізація функцій запису/читання продуктів (Service).

#include "service.h"

#include <fstream>
#include <iostream>

#include "exceptions.h"

namespace {

void CheckFileName(const std::string& file_name) {
  // ЛР5 п.7: виняток — ім'я файлу не задано.
  if (file_name.empty()) {
    throw FileError("ім'я файлу не задано (немає аргументу командного "
                    "рядка)");
  }
}

}  // namespace

std::string FileNameFromArgs(int argc, char* argv[]) {
  if (argc < 2 || argv[1][0] == '\0')
    throw FileError("не передано ім'я файлу в командному рядку");
  return argv[1];
}

void WriteToStream(const FinancialProduct& product, std::ostream& out) {
  product.Write(out);
  out.flush();
  // ЛР5 п.7: виняток — помилка запису (потік у стані fail/bad).
  if (!out)
    throw FileError("помилка запису даних продукту №" +
                    std::to_string(product.id()));
}

void WriteProduct(const FinancialProduct& product,
                  const std::string& file_name) {
  CheckFileName(file_name);
  std::ofstream out(file_name);
  // ЛР5 п.7: виняток — файл не вдалося відкрити для запису.
  if (!out.is_open())
    throw FileError("не вдалося відкрити «" + file_name + "» для запису");
  WriteToStream(product, out);
}

void ReadProduct(FinancialProduct& product, const std::string& file_name) {
  CheckFileName(file_name);
  std::ifstream in(file_name);
  // ЛР5 п.7: виняток — файл не вдалося відкрити для читання.
  if (!in.is_open()) {
    throw FileError("не вдалося відкрити «" + file_name +
                    "» для читання (файл не існує?)");
  }
  // ЛР5 п.7: виняток — порожній файл.
  if (in.peek() == std::ifstream::traits_type::eof())
    throw FileError("файл «" + file_name + "» порожній");
  product.Read(in);
}

void Service(FinancialProduct& product, const std::string& file_name) {
  std::cout << "  Service(): продукт №" << product.id() << ' '
            << product.TypeName() << " <-> файл «" << file_name << "»\n";
  try {
    WriteProduct(product, file_name);
    std::cout << "  Write(): поля записано у файл.\n";
    ReadProduct(product, file_name);
    std::cout << "  Read(): поля прочитано з файлу:\n";
    product.Describe();
  } catch (const FileError&) {
    std::cout << "  Service(): операцію перервано.\n";
    throw;
  }
}
