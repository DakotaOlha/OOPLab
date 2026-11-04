// Лабораторна робота №5, третій рівень, варіант 14 «Система Платежі».
// ЛР5 п.16: заголовний файл функцій запису/читання продуктів (Service).

#ifndef SERVICE_H_
#define SERVICE_H_

#include <iosfwd>
#include <string>

#include "financial_product.h"

// ЛР5 п.7: ім'я файлу з argv[1]; без аргументу — FileError.
std::string FileNameFromArgs(int argc, char* argv[]);

void WriteToStream(const FinancialProduct& product, std::ostream& out);
void WriteProduct(const FinancialProduct& product,
                  const std::string& file_name);
void ReadProduct(FinancialProduct& product, const std::string& file_name);

// ЛР5 п.7: записує поля продукту у файл через Write() і читає їх назад
// через Read(); FileError передається далі тому, хто викликав.
void Service(FinancialProduct& product, const std::string& file_name);

#endif  // SERVICE_H_
