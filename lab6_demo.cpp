// Лабораторна робота №6, третій рівень, варіант 14 «Система Платежі».
// ЛР6 п.1–п.6: демонстрація перевантажених операторів і шаблонів.

#include "lab6_demo.h"

#include <cstddef>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>

#include "credit_account.h"
#include "credit_card.h"
#include "current_account.h"
#include "financial_product.h"
#include "history.h"
#include "insurance.h"
#include "loyalty_program.h"
#include "order.h"
#include "payment_record.h"
#include "premium_account.h"
#include "product_store.h"
#include "savings_account.h"

namespace {

constexpr int kYear = 2026;
constexpr std::size_t kStoreCapacity = 5;

void PrintHeader(const std::string& title) {
  std::cout << "\n========== " << title << " ==========\n";
}

void PrintStep(const std::string& text) {
  std::cout << "\n-- " << text << '\n';
}

void PrintYears(const std::string& label, const CreditCard& card) {
  std::cout << "  " << label << ": строк дії до " << card.expiry_year_
            << '\n';
}

void DemoIncrementDecrement() {
  PrintHeader("ЛР6 п.1: унарні ++ і -- (строк дії CreditCard)");
  CreditCard card("4441 1111 2222 3001", "ПриватБанк", "Visa", 2028, 10000);
  PrintYears("До", card);

  PrintStep("x = ++card: спочатку зміна, x – новий стан");
  CreditCard x = ++card;
  PrintYears("x", x);
  PrintYears("card", card);

  PrintStep("y = card++: y – копія стану ДО зміни");
  CreditCard y = card++;
  PrintYears("y", y);
  PrintYears("card", card);

  PrintStep("++(++card): префіксний повертає посилання, тож +2 роки");
  ++(++card);
  PrintYears("card", card);

  PrintStep("z = card--: z – стан до скорочення");
  CreditCard z = card--;
  PrintYears("z", z);
  PrintYears("card", card);

  PrintStep("--card для картки до 2027 року: другий раз – відмова");
  CreditCard old_card("5168 0000 0000 3002", "Ощадбанк", "Mastercard", 2027,
                      5000);
  PrintYears("До", old_card);
  --old_card;
  --old_card;
  PrintYears("Після", old_card);
  std::cout << "  Кінець блоку п.1:\n";
}

void DemoBinaryPlusMinus() {
  PrintHeader("ЛР6 п.2: бінарні + і - (Order)");
  Order laptop(101, "Ноутбук", 32000);
  Order mouse(102, "Мишка", 800);
  laptop.MarkPaid();
  std::cout << "  До:\n";
  laptop.Print();
  mouse.Print();

  PrintStep("combined = laptop + mouse");
  const Order combined = laptop + mouse;
  combined.Print();

  PrintStep("discounted = combined - 2500 (знижка)");
  const Order discounted = combined - 2500;
  discounted.Print();

  PrintStep("mouse - 1000: сума не стає від'ємною");
  (mouse - 1000).Print();

  PrintStep("Ланцюжок (laptop + mouse + mouse) - 300");
  ((laptop + mouse + mouse) - 300).Print();

  std::cout << "\n  Після – оригінали не змінилися:\n";
  laptop.Print();
  mouse.Print();
}

void DemoCompoundOperators() {
  PrintHeader("ЛР6 п.3: оператори +=, -=, *=, *");
  CreditCard card("4441 1111 2222 3003", "ПриватБанк", "Visa", 2029, 2000);
  std::cout << "  До:\n";
  card.Print();

  PrintStep("card += 500 (поповнення)");
  card += 500;
  PrintStep("card += -10 (некоректна сума)");
  card += -10;
  PrintStep("(card -= 200) -= 100 (ланцюжок оплат)");
  (card -= 200) -= 100;
  PrintStep("card -= 100000 (перевищення ліміту – стан не змінюється)");
  card -= 100000;
  PrintStep("card *= 1.5 (запит на збільшення кредитного ліміту)");
  card *= 1.5;
  PrintStep("card *= -2 (некоректний коефіцієнт)");
  card *= -2;
  PrintStep("card.Block(); card -= 50 (заблокована картка)");
  card.Block();
  card -= 50;
  card.Unblock();
  std::cout << "  Після:\n";
  card.Print();

  PrintStep("doubled = card * 2: нова картка, оригінал без змін");
  const CreditCard doubled = card * 2;
  std::cout << "  card:\n";
  card.Print();
  std::cout << "  doubled:\n";
  doubled.Print();
  std::cout << "  Кінець блоку:\n";
}

void DemoAssignment() {
  PrintHeader("ЛР6 п.3: оператор = (CreditCard)");
  CreditCard a("1111 0000 0000 0001", "Банк А", "Visa", 2027, 1000);
  CreditCard b("2222 0000 0000 0002", "Банк Б", "Mastercard", 2028, 2000);
  CreditCard c("3333 0000 0000 0003", "Банк В", "Visa", 2031, 9000);
  c += 750;
  std::cout << "  До:\n";
  a.Print();
  b.Print();
  c.Print();
  CreditCard::PrintStatistics();

  PrintStep("a = b = c (ланцюжок присвоєнь)");
  a = b = c;
  PrintStep("a = a (самоприсвоєння)");
  CreditCard& same = a;
  a = same;

  std::cout << "  Після:\n";
  a.Print();
  b.Print();
  c.Print();
  std::cout << "  Лічильники не змінилися – нових карток не створено:\n";
  CreditCard::PrintStatistics();
  std::cout << "  Кінець блоку:\n";
}

void PrintStore(const ProductStore& store) {
  for (std::size_t i = 0; i < store.size(); ++i) {
    const FinancialProduct& product = store[static_cast<int>(i)];
    std::cout << std::fixed << std::setprecision(2) << "    store[" << i
              << "] №" << product.id() << ' ' << product.TypeName() << ", "
              << product.owner() << ", " << product.MonthlyCost()
              << " грн/міс.\n";
  }
}

void DemoIndexAndTemplateFunction() {
  std::cout << "\n  Створення продуктів для ProductStore:\n";
  CurrentAccount current("Олена Чаюк", "ПриватБанк", "грн", "UA610001", 5000,
                         kYear, "5168 0000 0000 6001", 1000, 40, 10000);
  Insurance insurance("Андрій Мельник", kYear, "POL-6002", "СК «Гарант»",
                      30000, 1.5, 1);
  ProductStore store(kStoreCapacity);
  store.AddStatic(&current);
  store.AddStatic(&insurance);
  store.AddDynamic(new SavingsAccount("Олена Чаюк", "Ощадбанк", "грн",
                                      "UA610003", 10000, kYear, 11.0, 12,
                                      1000));
  store.AddDynamic(new CreditAccount("Андрій Мельник", "Ощадбанк", "грн",
                                     "UA610004", -6000, kYear, 20000, 2.5,
                                     0));
  store.AddDynamic(new LoyaltyProgram("Олена Чаюк", "Бонус+", "Silver", 2.0,
                                      1, kYear));

  PrintHeader("ЛР6 п.3: operator[] (ProductStore, наскрізний індекс)");
  std::cout << "  Спочатку статичні (" << store.static_size()
            << "), потім динамічні (" << store.dynamic_size() << "):\n";
  PrintStore(store);
  PrintStep("store[3].Describe() через неконстантний operator[]");
  store[3].Describe();
  PrintStep("store[10] – вихід за межі");
  try {
    std::cout << store[10].TypeName() << '\n';
  } catch (const std::runtime_error& error) {
    std::cout << "  catch: " << error.what() << '\n';
  }

  PrintHeader("ЛР6 п.4: шаблонні функції-члени CountIf / FindIf");
  const double threshold = 30.0;
  const int expensive = store.CountIf([threshold](const FinancialProduct& p) {
    return p.MonthlyCost() > threshold;
  });
  std::cout << "  CountIf(вартість > " << threshold
            << " грн/міс.): " << expensive << '\n';

  const std::string owner = "Олена Чаюк";
  const int owned = store.CountIf(
      [&owner](const FinancialProduct& p) { return p.owner() == owner; });
  std::cout << "  CountIf(власник == «" << owner << "»): " << owned << '\n';

  const FinancialProduct* credit = store.FindIf([](const FinancialProduct& p) {
    return p.TypeName() == "CreditAccount";
  });
  std::cout << "  FindIf(TypeName() == «CreditAccount»): ";
  if (credit != nullptr)
    std::cout << "№" << credit->id() << ", власник " << credit->owner()
              << '\n';

  const FinancialProduct* premium = store.FindIf([](const FinancialProduct& p) {
    return p.TypeName() == "PremiumAccount";
  });
  std::cout << "  FindIf(TypeName() == «PremiumAccount»): "
            << (premium == nullptr ? "не знайдено (nullptr)" : "знайдено")
            << '\n';
  std::cout << "  Кінець блоку – знищення продуктів:\n";
}

void DemoClassTemplate() {
  PrintHeader("ЛР6 п.5: шаблон класу History<T>");
  PrintStep("History<double> – суми платежів, грн");
  History<double> amounts("Суми платежів, грн");
  std::cout << "  До: порожня, Size() = " << amounts.Size() << '\n';
  const double values[] = {1200.5, 350.0, 4800.0, 99.9, 640.0};
  for (double value : values)
    amounts.Add(value);
  std::cout << "  Після Add (5-й елемент збільшив масив з 4 до 8):\n";
  amounts.Print();
  std::cout << std::fixed << std::setprecision(2)
            << "  Total = " << amounts.Total()
            << ", Average = " << amounts.Average()
            << ", Max = " << amounts.Max() << ", Min = " << amounts.Min()
            << ", Last = " << amounts.Last() << '\n'
            << "  Contains(350) = " << std::boolalpha << amounts.Contains(350)
            << ", IndexOf(777) = " << amounts.IndexOf(777)
            << ", amounts[2] = " << amounts[2] << std::noboolalpha << '\n';

  PrintStep("History<int> – кількість операцій за день");
  History<int> operations("Операцій за день", 2);
  const int counts[] = {3, 7, 2, 5};
  for (int count : counts)
    operations.Add(count);
  operations.Print();
  std::cout << "  Max = " << operations.Max()
            << ", Min = " << operations.Min()
            << ", Average = " << operations.Average()
            << " (для int – ціле ділення)\n";

  PrintStep("Винятки: порожня історія і вихід за межі");
  History<int> empty("Порожня");
  try {
    std::cout << empty.Max() << '\n';
  } catch (const std::runtime_error& error) {
    std::cout << "  catch: " << error.what() << '\n';
  }
  try {
    std::cout << operations[9] << '\n';
  } catch (const std::runtime_error& error) {
    std::cout << "  catch: " << error.what() << '\n';
  }
}

void DemoUserTypeTemplate() {
  PrintHeader("ЛР6 п.6: користувацький тип PaymentRecord у History<T>");
  const PaymentRecord rent("2026-11-01", 9500.0, PaymentType::kAccount,
                           "Олена Чаюк", "ОСББ «Затишок»");
  const PaymentRecord phone("2026-11-03", 250.0, PaymentType::kCard,
                            "Олена Чаюк", "Київстар");
  const PaymentRecord shop("2026-11-07", 1830.4, PaymentType::kCard,
                           "Олена Чаюк", "Сільпо");
  const PaymentRecord gift("2026-11-09", 3000.0, PaymentType::kCard,
                           "Олена Чаюк", "Андрій Мельник");

  History<PaymentRecord> history("Історія платежів, листопад");
  std::cout << "  До: Size() = " << history.Size() << '\n';
  history.Add(rent);
  history.Add(phone);
  history.Add(shop);
  history.Add(gift);
  std::cout << "  Після Add:\n";
  history.Print();

  std::cout << "  Max:     " << history.Max() << '\n'
            << "  Min:     " << history.Min() << '\n'
            << "  Average: " << history.Average() << '\n'
            << "  Last:    " << history.Last() << '\n'
            << "  history[1]: " << history[1] << '\n';

  const PaymentRecord unknown("2026-11-10", 250.0, PaymentType::kCard,
                              "Олена Чаюк", "Vodafone");
  std::cout << std::boolalpha
            << "  Contains(phone) = " << history.Contains(phone)
            << ", IndexOf(shop) = " << history.IndexOf(shop)
            << ", Contains(unknown) = " << history.Contains(unknown) << '\n'
            << "  phone != unknown: " << (phone != unknown)
            << ", phone == history[1]: " << (phone == history[1])
            << std::noboolalpha << '\n';

  PrintStep("Вільний шаблон MaxOf<T> для різних типів");
  std::cout << "  MaxOf(shop, gift): " << MaxOf(shop, gift) << '\n'
            << "  MaxOf(4.5, 2.0): " << MaxOf(4.5, 2.0) << '\n'
            << "  MaxOf<std::string>(\"Visa\", \"Mastercard\"): "
            << MaxOf<std::string>("Visa", "Mastercard") << '\n';

  // ПОМИЛКА: Order не має operator> і operator<<, тому не підходить шаблону:
  // MaxOf(Order(1, "A", 10), Order(2, "B", 20));
  // History<Order> orders("Замовлення"); orders.Add(Order());
}

}  // namespace

void RunLab6Demo() {
  std::cout << "Лабораторна робота №6 – перевантаження операторів, шаблони\n";
  DemoIncrementDecrement();
  DemoBinaryPlusMinus();
  DemoCompoundOperators();
  DemoAssignment();
  DemoIndexAndTemplateFunction();
  DemoClassTemplate();
  DemoUserTypeTemplate();
  std::cout << "\nКінець демонстрації ЛР6.\n";
  CreditCard::PrintStatistics();
  std::cout << "  Живих фінансових продуктів: "
            << FinancialProduct::alive_count() << '\n';
}
