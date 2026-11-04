// Лабораторна робота №5, третій рівень, варіант 14 «Система Платежі».
// ЛР5 п.16: main.cpp — головне меню ЛР5; разом з lab5_demo.cpp це одна
// демонстраційна програма.

#include <cstddef>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>

#ifdef _WIN32
#include <windows.h>
#endif

#include "credit_account.h"
#include "current_account.h"
#include "exceptions.h"
#include "financial_product.h"
#include "installment_plan.h"
#include "insurance.h"
#include "lab5_demo.h"
#include "loyalty_program.h"
#include "premium_account.h"
#include "product_store.h"
#include "savings_account.h"
#include "service.h"

namespace {

constexpr int kCurrentYear = 2026;
constexpr std::size_t kMenuCapacity = 10;

void PrintHeader(const std::string& title) {
  std::cout << "\n========== " << title << " ==========\n";
}

void SkipLine() {
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// Некоректне введення не завершує програму; false — лише кінець вводу.
template <typename T>
bool ReadValue(T* value) {
  while (!(std::cin >> *value)) {
    if (std::cin.eof())
      return false;
    std::cin.clear();
    SkipLine();
    std::cout << "Некоректне введення, повторіть: ";
  }
  return true;
}

FinancialProduct* CreateProduct(int type, const std::string& owner,
                                int serial) {
  const std::string number = "UA6" + std::to_string(10000 + serial);
  switch (type) {
    case 1:
      return new CurrentAccount(owner, "ПриватБанк", "грн", number, 2000,
                                kCurrentYear, "5168 0000 0000 6000", 1000,
                                40, 10000);
    case 2:
      return new SavingsAccount(owner, "Ощадбанк", "грн", number, 10000,
                                kCurrentYear, 11.0, 12, 1000);
    case 3:
      return new CreditAccount(owner, "Ощадбанк", "грн", number, 0,
                               kCurrentYear, 20000, 2.5, 0);
    case 4:
      return new InstallmentPlan(owner, "Райффайзен", number, 3000,
                                 kCurrentYear, 2000, "Смартфон", 12000, 4,
                                 1);
    case 5:
      return new Insurance(owner, kCurrentYear, "POL-" + number,
                           "СК «Гарант»", 30000, 1.5, 1);
    case 6:
      return new LoyaltyProgram(owner, "Бонус+", "Silver", 2.0, 1,
                                kCurrentYear);
    case 7:
      return new PremiumAccount(owner, number, 10000, kCurrentYear,
                                "4441 0000 0000 6000", 5000, "POL-" + number,
                                40000, "Кешбек+", 3.0, kCurrentYear);
    default:
      throw NotFoundError("тип продукту " + std::to_string(type));
  }
}

}  // namespace

// ЛР5 п.8: меню, через яке користувач створює та знищує продукти.
void RunInteractiveMenu(const std::string& file_name) {
  std::cout << "Лабораторна робота №5 — інтерактивне меню\n";
  ProductStore store(kMenuCapacity);
  int serial = 0;
  while (true) {
    std::cout << "\n1 — створити продукт\n"
                 "2 — знищити продукт за номером\n"
                 "3 — знищити продукти з вартістю > X (Service_Dinamic)\n"
                 "4 — переглянути всі (FinancialProduct::ShowAll)\n"
                 "5 — Service: запис і читання продукту у файл\n"
                 "0 — назад\nВаш вибір: ";
    int choice = 0;
    if (!ReadValue(&choice) || choice == 0)
      break;

    try {
      if (choice == 1) {
        std::cout << "Тип: 1 — поточний, 2 — депозит, 3 — кредит, "
                     "4 — розстрочка, 5 — страховка,\n"
                     "     6 — лояльність, 7 — преміальний: ";
        int type = 0;
        if (!ReadValue(&type))
          break;
        SkipLine();
        std::cout << "Власник: ";
        std::string owner;
        std::getline(std::cin, owner);
        if (owner.empty())
          owner = "Без імені";
        store.AddDynamic(CreateProduct(type, owner, serial + 1));
        ++serial;
      } else if (choice == 2) {
        std::cout << "Номер продукту (№): ";
        int id = 0;
        if (!ReadValue(&id))
          break;
        store.RemoveDynamicById(id);
      } else if (choice == 3) {
        std::cout << "Поріг X, грн/міс.: ";
        double threshold = 0.0;
        if (!ReadValue(&threshold))
          break;
        const int removed = store.Service_Dinamic(threshold);
        std::cout << "Видалено: " << removed << '\n';
      } else if (choice == 4) {
        FinancialProduct::ShowAll();
        store.Print();
      } else if (choice == 5) {
        store.Print();
        std::cout << "Індекс у масиві динамічних: ";
        int index = 0;
        if (!ReadValue(&index))
          break;
        // ЛР5 п.7: Service викликається з main.cpp.
        Service(*store.DynamicAt(index), file_name);
      } else {
        std::cout << "Невідомий пункт меню.\n";
      }
    } catch (const std::runtime_error& error) {
      std::cout << "  catch: " << error.what() << '\n';
    }
  }
  std::cout << "Вихід з меню: знищення створених об'єктів\n";
}

int main(int argc, char* argv[]) {
#ifdef _WIN32
  SetConsoleOutputCP(CP_UTF8);
  SetConsoleCP(CP_UTF8);
#endif

  // ЛР5 п.7: ім'я файлу передається параметром командного рядка.
  std::string file_name;
  try {
    file_name = FileNameFromArgs(argc, argv);
    std::cout << "Файл даних: " << file_name << '\n';
  } catch (const FileError& error) {
    std::cout << error.what()
              << "\nЗадайте його в CLion: Run → Edit Configurations → "
                 "Program arguments (напр. data.txt).\n";
  }

  while (true) {
    std::cout << "\nОберіть: 1 — демонстрація ЛР5, 2 — інтерактивне меню, "
                 "0 — вихід: ";
    int choice = 0;
    if (!ReadValue(&choice) || choice == 0)
      break;

    try {
      if (choice == 1) {
        RunLab5Demo(file_name);
        // ЛР5 п.7: main() записує і читає продукт через виклик Service.
        PrintHeader("ЛР5 п.7: виклик Service з main()");
        PremiumAccount product("Ірина Коваленко", "UA500020", 12000, 2018,
                               "4441 0000 0000 5020", 8000, "POL-5020",
                               60000, "Кешбек+", 3.0, 2023);
        product.PayWithCashback(1500);
        Service(product, file_name);
      } else if (choice == 2) {
        RunInteractiveMenu(file_name);
      } else {
        std::cout << "Невідомий вибір.\n";
      }
    } catch (const std::exception& error) {
      std::cout << "  catch у main(): " << error.what() << '\n';
    }
  }

  std::cout << "\nЖивих фінансових продуктів: "
            << FinancialProduct::alive_count() << '\n';
  return 0;
}
