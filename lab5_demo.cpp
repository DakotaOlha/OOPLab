// Лабораторна робота №5, третій рівень, варіант 14 «Система Платежі».
// ЛР5 п.16: lab5_demo.cpp — демонстрація ЛР5; разом з main.cpp це одна
// демонстраційна програма.

#include "lab5_demo.h"

#include <cstddef>
#include <cstdio>
#include <fstream>
#include <iomanip>
#include <iostream>

#include "bank_account.h"
#include "credit_account.h"
#include "current_account.h"
#include "exceptions.h"
#include "financial_product.h"
#include "installment_plan.h"
#include "insurance.h"
#include "legacy_transaction.h"
#include "loyalty_program.h"
#include "premium_account.h"
#include "product_store.h"
#include "savings_account.h"
#include "service.h"
#include "transfer_transaction.h"

namespace {

constexpr std::size_t kStoreCapacity = 5;
constexpr std::size_t kHugeCapacity = static_cast<std::size_t>(-1) / 16;
constexpr double kCostThreshold = 100.0;
constexpr char kMissingFile[] = "lab5_missing.tmp";
constexpr char kEmptyFile[] = "lab5_empty.tmp";
constexpr char kBrokenFile[] = "lab5_broken.tmp";
constexpr char kPartialFile[] = "lab5_partial.tmp";
constexpr char kOtherTypeFile[] = "lab5_other_type.tmp";
constexpr char kNoDirFile[] = "lab5_no_such_dir/data.txt";

void PrintSection(const std::string& title) {
  std::cout << "\n========== " << title << " ==========\n";
}

void PrintError(const std::exception& error) {
  std::cout << "  catch: " << error.what() << '\n';
}

// ЛР5 п.9: спосіб 2 — поліморфний виклик через посилання на базу.
void DescribeByReference(const FinancialProduct& product) {
  product.Describe();
}

// ЛР5 п.10: зміна форми (б) — параметр за значенням. PremiumAccount
// зрізається до CurrentAccount, і виклик через об'єкт зв'язується рано.
void DescribeByValue(CurrentAccount account) {
  std::cout << "  Усередині функції об'єкт уже " << account.TypeName()
            << ":\n";
  account.Describe();
}

void WriteTextFile(const std::string& file_name, const std::string& text) {
  std::ofstream out(file_name);
  out << text;
}

// ЛР5 п.2: ромб — один підоб'єкт FinancialProduct; відкритий шлях до нього
// в SavingsAccount при protected-базі BankAccount.
void ShowDiamond() {
  PrintSection("ЛР5 п.2: віртуальне наслідування (ромб)");
  std::cout << "-> Вхід у блок. Конструктор FinancialProduct викликає "
               "найпохідніший клас PremiumAccount — першим і один раз:\n";
  {
    const int alive_before = FinancialProduct::alive_count();
    PremiumAccount premium("Ірина Коваленко", "UA500010", 15000, 2018,
                           "4441 0000 0000 5010", 10000, "POL-5010", 50000,
                           "Кешбек+", 3.0, 2023);
    const FinancialProduct* via_current =
        static_cast<const CurrentAccount*>(&premium);
    const FinancialProduct* via_insurance =
        static_cast<const Insurance*>(&premium);
    const FinancialProduct* via_loyalty =
        static_cast<const LoyaltyProgram*>(&premium);
    const bool same = via_current == via_insurance &&
                      via_insurance == via_loyalty;
    std::cout << "Підоб'єкт FinancialProduct через CurrentAccount, Insurance "
                 "і LoyaltyProgram: "
              << (same ? "одна й та сама адреса" : "різні адреси") << '\n'
              << "Додалось живих продуктів: "
              << FinancialProduct::alive_count() - alive_before
              << " (а не 3); premium.id() = " << premium.id() << '\n';
    // Без virtual: request for member 'id' is ambiguous.
    premium.Describe();

    std::cout << "\nSavingsAccount: BankAccount — protected-база, а "
                 "FinancialProduct — відкритий другий шлях:\n";
    SavingsAccount savings("Тарас Бойко", "Ощадбанк", "грн", "UA500011",
                           20000, 2021, 12.0, 12, 5000);
    const FinancialProduct& product = savings;
    std::cout << "FinancialProduct& -> " << product.TypeName() << " №"
              << product.id() << ", вартість/міс.: " << std::fixed
              << std::setprecision(2) << product.MonthlyCost() << '\n';
    // ПОМИЛКА: savings.Deposit(100); — 'Deposit' is protected: члени
    // BankAccount і далі закриті, відкритий лише інтерфейс FinancialProduct
    // (той самий єдиний підоб'єкт завдяки virtual).
    // ПОМИЛКА: BankAccount& base = savings; — inaccessible base.
    std::cout << "<- Вихід з блоку: ~FinancialProduct() — останнім:\n";
  }
}

// ЛР5 п.3: delete через вказівник на базу — без virtual і з virtual.
void ShowDestructors() {
  PrintSection("ЛР5 п.3: невіртуальний і віртуальний деструктори");
  std::cout << "а) LegacyTransaction* -> new TransferTransaction "
               "(деструктор НЕ віртуальний):\n";
  LegacyTransaction* legacy =
      new TransferTransaction(1500, "UA500001", "UA500002");
  legacy->Print();
  // Видалення похідного через вказівник на базу без virtual-деструктора —
  // undefined behavior; показано лише для навчання.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdelete-non-virtual-dtor"
  delete legacy;
#pragma GCC diagnostic pop
  std::cout << "  ~TransferTransaction() НЕ викликано — знищено лише "
               "частину бази.\n";

  std::cout << "б) FinancialProduct* -> new PremiumAccount "
               "(деструктор віртуальний):\n";
  FinancialProduct* product =
      new PremiumAccount("Андрій Мельник", "UA500012", 1000, 2024,
                         "5375 0000 0000 5012", 2000, "POL-5012", 20000,
                         "Бонус", 1.0, 2024);
  delete product;
  std::cout << "  Спрацювали деструктори всіх рівнів ієрархії.\n";
}

// ЛР5 п.7: Write/Read через файл і винятки роботи з файлами.
void ShowFileErrors(const std::string& file_name, CurrentAccount& current,
                    const Insurance& insurance) {
  PrintSection("ЛР5 п.7: Write/Read і винятки роботи з файлами");
  std::cout << "Write() у файл з командного рядка, зміна, Read() назад:\n";
  try {
    WriteProduct(current, file_name);
    current.Deposit(1000);
    std::cout << "Після зміни:\n";
    current.Describe();
    ReadProduct(current, file_name);
    std::cout << "Після Read() — стан відновлено:\n";
    current.Describe();
  } catch (const FileError& error) {
    PrintError(error);
  }

  // Read() заповнює поля по черзі, тому пошкоджені файли читаємо
  // в окремий тимчасовий об'єкт.
  CurrentAccount scratch("Тимчасовий", "ПриватБанк", "грн", "UA500099", 0,
                         2026, "0000 0000 0000 5099", 0, 0, 1000);

  std::cout << "1) Ім'я файлу не задано:\n";
  try {
    WriteProduct(current, "");
  } catch (const FileError& error) {
    PrintError(error);
  }

  std::cout << "2) Файл не відкривається для запису (немає теки):\n";
  try {
    WriteProduct(current, kNoDirFile);
  } catch (const FileError& error) {
    PrintError(error);
  }

  std::cout << "3) Файл не відкривається для читання (не існує):\n";
  std::remove(kMissingFile);
  try {
    ReadProduct(scratch, kMissingFile);
  } catch (const FileError& error) {
    PrintError(error);
  }

  std::cout << "4) Порожній файл:\n";
  WriteTextFile(kEmptyFile, "");
  try {
    ReadProduct(scratch, kEmptyFile);
  } catch (const FileError& error) {
    PrintError(error);
  }

  std::cout << "5) Пошкоджені дані (замість числа — текст):\n";
  WriteTextFile(kBrokenFile, "CurrentAccount\nсім\n");
  try {
    ReadProduct(scratch, kBrokenFile);
  } catch (const FileError& error) {
    PrintError(error);
  }

  std::cout << "6) Неповні дані (файл обірвано):\n";
  WriteTextFile(kPartialFile, "CurrentAccount\n7\nТарас Бойко\n");
  try {
    ReadProduct(scratch, kPartialFile);
  } catch (const FileError& error) {
    PrintError(error);
  }

  std::cout << "7) У файлі продукт іншого типу:\n";
  try {
    WriteProduct(insurance, kOtherTypeFile);
    ReadProduct(scratch, kOtherTypeFile);
  } catch (const FileError& error) {
    PrintError(error);
  }

  std::cout << "8) Помилка запису (потік без буфера імітує збій диска):\n";
  try {
    std::ostream broken(nullptr);
    WriteToStream(current, broken);
  } catch (const FileError& error) {
    PrintError(error);
  }

  std::cout << "9) Service() передає виняток далі (throw;):\n";
  try {
    Service(scratch, kNoDirFile);
  } catch (const FileError& error) {
    PrintError(error);
  }

  for (const char* temp : {kEmptyFile, kBrokenFile, kPartialFile,
                           kOtherTypeFile}) {
    std::remove(temp);
  }
}

}  // namespace

void RunLab5Demo(const std::string& file_name) {
  std::cout << "Лабораторна робота №5, третій рівень, варіант 14\n";

  // ЛР5 п.1: абстрактний клас; статичний метод викликається через клас.
  PrintSection("ЛР5 п.1: абстрактний клас FinancialProduct");
  std::cout << "FinancialProduct::ShowAll() до створення об'єктів ЛР5:\n";
  FinancialProduct::ShowAll();
  // ПОМИЛКА: FinancialProduct product("Х", 2026); — cannot declare
  // variable 'product' to be of abstract type 'FinancialProduct'.
  // ПОМИЛКА: BankAccount account(...); — BankAccount теж абстрактний.

  ShowDiamond();
  ShowDestructors();

  std::cout << "\n-> Вхід в основний блок ЛР5\n";
  {
    // ЛР5 п.4: у кожного класу своя віртуальна функція.
    PrintSection("ЛР5 п.4: віртуальні функції кожного класу");
    std::cout << "5 статичних об'єктів (стек):\n";
    CurrentAccount current("Тарас Бойко", "ПриватБанк", "грн", "UA500001",
                           5000, 2019, "5168 0000 0000 5001", 5000, 50,
                           25000);
    SavingsAccount savings("Тарас Бойко", "Ощадбанк", "грн", "UA500002",
                           20000, 2021, 12.0, 12, 5000);
    CreditAccount credit("Андрій Мельник", "Ощадбанк", "грн", "UA500003", 0,
                         2024, 30000, 2.0, 0);
    Insurance insurance("Олена Шевчук", 2025, "POL-5004",
                        "СК «Надійний захист»", 80000, 1.2, 2);
    LoyaltyProgram loyalty("Олена Шевчук", "Бонус+", "Silver", 2.0, 1, 2024);
    credit.Borrow(12000);
    insurance.RegisterClaim(5000);
    loyalty.AddPurchase(3000);

    std::cout << std::fixed << std::setprecision(2)
              << "\nCurrentAccount: MonthlyCost() = плата "
              << current.MonthlyCost() << ", AvailableFunds() = "
              << current.AvailableFunds() << '\n'
              << "SavingsAccount: MonthlyCost() = -відсотки "
              << savings.MonthlyCost() << ", CanBeClosed() = "
              << (savings.CanBeClosed() ? "так" : "ні (строк не минув)")
              << '\n'
              << "CreditAccount: MonthlyCost() = відсотки на борг "
              << credit.MonthlyCost() << ", CanBeClosed() = "
              << (credit.CanBeClosed() ? "так" : "ні (є борг)") << '\n'
              << "Insurance: MonthlyCost() = внесок/12 "
              << insurance.MonthlyCost() << ", CoverageLeft() = "
              << insurance.CoverageLeft() << '\n'
              << "LoyaltyProgram: MonthlyCost() = -кешбек "
              << loyalty.MonthlyCost() << ", BonusValue() = "
              << loyalty.BonusValue() << '\n';

    // ЛР5 п.5: масиви статичних і динамічних об'єктів, винятки пам'яті.
    PrintSection("ЛР5 п.5: статичні та динамічні об'єкти");
    ProductStore store(kStoreCapacity);
    store.AddStatic(&current);
    store.AddStatic(&savings);
    store.AddStatic(&credit);
    store.AddStatic(&insurance);
    store.AddStatic(&loyalty);
    std::cout << "5 динамічних об'єктів (new):\n";
    store.AddDynamic(new PremiumAccount(
        "Ірина Коваленко", "UA500005", 15000, 2018, "4441 0000 0000 5005",
        10000, "POL-5005", 50000, "Кешбек+", 3.0, 2023));
    store.AddDynamic(new InstallmentPlan("Олена Шевчук", "Райффайзен",
                                         "UA500006", 5000, 2022, 3000,
                                         "Ноутбук", 24000, 6, 11));
    store.AddDynamic(new CurrentAccount(
        "Андрій Мельник", "Ощадбанк", "грн", "UA500007", 3000, 2020,
        "5375 0000 0000 5007", 2000, 30, 10000));
    store.AddDynamic(new SavingsAccount("Ірина Коваленко", "ПриватБанк",
                                        "грн", "UA500008", 50000, 2022, 10.0,
                                        6, 1000));
    store.AddDynamic(new Insurance("Андрій Мельник", 2026, "POL-5009",
                                   "СК «Гарант»", 20000, 2.0, 1));
    store.Print();

    std::cout << "Виняток 1 — величезний масив (std::bad_alloc):\n";
    try {
      ProductStore huge(kHugeCapacity);
    } catch (const MemoryError& error) {
      PrintError(error);
    }
    std::cout << "Виняток 2 — nullptr:\n";
    try {
      store.AddDynamic(nullptr);
    } catch (const MemoryError& error) {
      PrintError(error);
    }
    std::cout << "Виняток 3 — переповнення масиву (6-й об'єкт):\n";
    try {
      store.AddDynamic(new LoyaltyProgram("Зайвий", "Бонус", "Basic", 1.0, 1,
                                          2026));
    } catch (const MemoryError& error) {
      PrintError(error);
    }
    std::cout << "Виняток 4 — вихід за межі масиву:\n";
    try {
      store.StaticAt(7)->Describe();
    } catch (const MemoryError& error) {
      PrintError(error);
    }

    // ЛР5 п.6: видалення з масивів за критерієм.
    PrintSection("ЛР5 п.6: Service_Static і Service_Dinamic");
    std::cout << std::fixed << std::setprecision(2)
              << "Критерій: вартість обслуговування > " << kCostThreshold
              << " грн/міс.\n";
    const int static_removed = store.Service_Static(kCostThreshold);
    std::cout << "Service_Static: вилучено " << static_removed << '\n';
    const int dynamic_removed = store.Service_Dinamic(kCostThreshold);
    std::cout << "Service_Dinamic: видалено " << dynamic_removed << '\n';
    store.Print();

    std::cout << "Виняток 5 — повторне видалення того самого об'єкта:\n";
    const int removed_id = store.DynamicAt(0)->id();
    try {
      store.RemoveDynamicById(removed_id);
      std::cout << "  Перше видалення №" << removed_id << " — успішне.\n";
      store.RemoveDynamicById(removed_id);
    } catch (const NotFoundError& error) {
      PrintError(error);
    }
    std::cout << "Виняток 6 — видалення з порожнього масиву:\n";
    try {
      ProductStore empty_store(kStoreCapacity);
      empty_store.Service_Dinamic(kCostThreshold);
    } catch (const MemoryError& error) {
      PrintError(error);
    }

    ShowFileErrors(file_name, current, insurance);

    // ЛР5 п.8: саме меню — RunInteractiveMenu() у main.cpp.
    PrintSection("ЛР5 п.8: меню створення і знищення об'єктів");
    std::cout << "Інтерактивне меню — пункт 6 головного меню.\n";

    // ЛР5 п.9: три способи поліморфного виклику.
    PrintSection("ЛР5 п.9: поліморфні виклики трьома способами");
    std::cout << "1) Через вказівник на базу FinancialProduct*:\n";
    FinancialProduct* pointer = &insurance;
    pointer->Describe();
    std::cout << "2) Через посилання на базу FinancialProduct&:\n";
    DescribeByReference(loyalty);
    std::cout << "3) Через масив вказівників у циклі:\n";
    for (std::size_t i = 0; i < store.static_size(); ++i)
      store.StaticAt(static_cast<int>(i))->Describe();

    // ЛР5 п.9: зміна форми (а) — один покажчик по черзі вказує на різні
    // об'єкти: той самий виклик, різна поведінка.
    std::cout << "Зміна форми (а): один FinancialProduct* shape — різні "
                 "об'єкти:\n";
    FinancialProduct* shape = nullptr;
    FinancialProduct* const objects[] = {&current, &savings, &credit,
                                         &insurance, &loyalty};
    for (FinancialProduct* object : objects) {
      shape = object;
      std::cout << std::fixed << std::setprecision(2) << "  shape -> "
                << shape->TypeName() << ": shape->MonthlyCost() = "
                << shape->MonthlyCost() << '\n';
    }

    // ЛР5 п.10: пізнє і раннє зв'язування.
    PrintSection("ЛР5 п.10: пізнє і раннє зв'язування");
    BankAccount* base = &current;
    std::cout << "Пізнє: base->Describe() (virtual) — версія "
                 "CurrentAccount:\n";
    base->Describe();
    std::cout << "Раннє: base->PrintInfo() (не virtual) — версія "
                 "BankAccount, без картки:\n";
    base->PrintInfo();
    std::cout << "Раннє: current.PrintInfo() через об'єкт — версія "
                 "CurrentAccount:\n";
    current.PrintInfo();

    std::cout << "\nЗміна форми (б): PremiumAccount передається за "
                 "значенням як CurrentAccount (зрізання):\n";
    PremiumAccount vip("Ірина Коваленко", "UA500013", 30000, 2017,
                       "4441 0000 0000 5013", 20000, "POL-5013", 100000,
                       "Кешбек+", 5.0, 2020);
    vip.PayWithCashback(4000);
    DescribeByValue(vip);
    std::cout << "Для порівняння — через посилання, без зрізання:\n";
    DescribeByReference(vip);

    // ЛР5 п.11: обхід механізму віртуальних функцій.
    PrintSection("ЛР5 п.11: обхід механізму віртуальних функцій");
    FinancialProduct* product = &vip;
    std::cout << "product->Describe() — віртуальний виклик:\n";
    product->Describe();
    std::cout << "product->FinancialProduct::Describe() — тіло чисто "
                 "віртуальної функції:\n";
    product->FinancialProduct::Describe();
    std::cout << std::fixed << std::setprecision(2)
              << "vip.MonthlyCost() = " << vip.MonthlyCost()
              << ", vip.CurrentAccount::MonthlyCost() = "
              << vip.CurrentAccount::MonthlyCost()
              << ", vip.Insurance::MonthlyCost() = "
              << vip.Insurance::MonthlyCost() << '\n';

    PrintSection("Кінець основного блоку ЛР5");
    FinancialProduct::ShowAll();
    std::cout << "<- Вихід з блоку (зворотний порядок): vip, потім "
                 "ProductStore видаляє динамічні, потім статичні об'єкти\n";
  }

  PrintSection("Підсумок ЛР5");
  FinancialProduct::ShowAll();
}
