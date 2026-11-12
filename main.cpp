// Лабораторна робота №6, третій рівень, варіант 14 «Система Платежі».
// ЛР6: main.cpp – головне меню; разом з lab6_demo.cpp це одна програма.

#include <exception>
#include <iostream>
#include <limits>

#ifdef _WIN32
#include <windows.h>
#endif

#include "financial_product.h"
#include "lab6_demo.h"

namespace {

// Некоректне введення не завершує програму; false – лише кінець вводу.
bool ReadChoice(int* value) {
  while (!(std::cin >> *value)) {
    if (std::cin.eof())
      return false;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Некоректне введення, повторіть: ";
  }
  return true;
}

}  // namespace

int main() {
#ifdef _WIN32
  SetConsoleOutputCP(CP_UTF8);
  SetConsoleCP(CP_UTF8);
#endif

  while (true) {
    std::cout << "\nОберіть: 1 – демонстрація ЛР6, 0 – вихід: ";
    int choice = 0;
    if (!ReadChoice(&choice) || choice == 0)
      break;

    try {
      if (choice == 1)
        RunLab6Demo();
      else
        std::cout << "Невідомий вибір.\n";
    } catch (const std::exception& error) {
      std::cout << "  catch у main(): " << error.what() << '\n';
    }
  }

  std::cout << "\nЖивих фінансових продуктів: "
            << FinancialProduct::alive_count() << '\n';
  return 0;
}
