// Лабораторна робота №3, варіант 14 «Система Платежі».
// п.17: cpp-файл демонстраційної програми.
//
// Файли проєкту:
//   credit_card.h / credit_card.cpp     — клас CreditCard
//   client.h / client.cpp               — клас Client (містить CreditCard)
//   order.h / order.cpp                 — клас Order
//   payment.h / payment.cpp             — клас асоціації Payment
//   administrator.h / administrator.cpp — клас Administrator (агрегація)
//   main.cpp                            — демонстраційна програма

#include <iomanip>
#include <iostream>
#include <string>

#ifdef _WIN32
#include <windows.h>
#endif

#include "administrator.h"
#include "client.h"
#include "credit_card.h"
#include "order.h"
#include "payment.h"

namespace {

void PrintHeader(const std::string& title) {
  std::cout << "\n========== " << title << " ==========\n";
}

}

// ---------------------------------------------------------------------------
// п.5: функція поза класом, параметр — ОБ'ЄКТ класу (передається копія,
// тому спрацьовують конструктор копіювання і деструктор).
// Обчислює фінансовий стан клієнта.
// ---------------------------------------------------------------------------
void PrintFinancialReport(Client client) {
  const CreditCard& card = client.card();
  const double debt = card.balance() < 0.0 ? -card.balance() : 0.0;
  const double own_on_card = card.balance() > 0.0 ? card.balance() : 0.0;
  double available_credit = card.credit_limit() - debt;

  if (available_credit < 0.0)
    available_credit = 0.0;

  const double usage_percent = card.credit_limit() > 0.0 ? debt / card.credit_limit() * 100.0 : 0.0;

  const double total_available = client.account_balance() + own_on_card + available_credit;

  std::cout << std::fixed << std::setprecision(2)
            << "  Фінансовий звіт (копія об'єкта) — " << client.last_name_
            << ":\n"
            << "    На рахунку:             " << client.account_balance()
            << " грн\n"
            << "    Кредитний борг:         " << debt << " грн\n"
            << "    Доступний кредит:       " << available_credit << " грн\n"
            << "    Використано ліміту:     " << usage_percent << " %\n"
            << "    Всього можна витратити: " << total_available << " грн\n";
}

// ---------------------------------------------------------------------------
// п.6: перевантажена функція з п.5, параметр — АДРЕСА об'єкта (покажчик).
// Копія не створюється. Обчислює план виплат кредитного боргу на 3 місяці.
// ---------------------------------------------------------------------------
void PrintFinancialReport(const Client* client) {
  if (client == nullptr)
    return;

  constexpr int kMonths = 3;
  constexpr double kMonthlyRatePercent = 2.0;

  const double balance = client->card().balance();
  const double debt = balance < 0.0 ? -balance : 0.0;

  std::cout << std::fixed << std::setprecision(2)
            << "  План виплат (через адресу об'єкта) — " << client->last_name_
            << ", борг " << debt << " грн:\n";

  if (debt == 0.0) {
    std::cout << "    Боргу немає, план виплат не потрібен.\n";
    return;
  }
  const double total = debt * (1.0 + kMonthlyRatePercent / 100.0 * kMonths);

  for (int month = 1; month <= kMonths; ++month) {
    std::cout << "    Місяць " << month << ": " << total / kMonths << " грн\n";
  }
  std::cout << "    Разом з відсотками (" << kMonthlyRatePercent
            << "%/міс.): " << total << " грн\n";
}

// ---------------------------------------------------------------------------
// п.7: функція типу класу Client — повертає об'єкт класу. Використовує
// методи класу: ReissueCard(), mutable_card().set_credit_limit(), TopUp().
// ---------------------------------------------------------------------------
Client MakePremiumClient(const Client& base, double premium_limit) {
  std::cout << "  Оформлення преміум-обслуговування для " << base.last_name_
            << ":\n";

  Client premium = base.ReissueCard(base.card().expiry_year_ + 4);
  premium.mutable_card().set_credit_limit(premium_limit);
  premium.mutable_card().TopUp(500.0);
  return premium;
}

int main() {
#ifdef _WIN32
  SetConsoleOutputCP(CP_UTF8);
  SetConsoleCP(CP_UTF8);
#endif

  std::cout << "Лабораторна робота №3, третій рівень, варіант 14\n";

  // -------------------------------------------------------------------------
  // п.4: статичні елементи існують ще до створення жодного об'єкта.
  // -------------------------------------------------------------------------
  PrintHeader("п.4: Статичні поля до створення об'єктів");
  CreditCard::PrintStatistics();
  std::cout << "  Комісія за переказ для всіх клієнтів: "
            << Client::commission_percent() << "%\n";

  // -------------------------------------------------------------------------
  // п.8: три способи створення об'єктів за допомогою конструкторів.
  // п.10: демонстрація всіх конструкторів.
  // -------------------------------------------------------------------------
  PrintHeader("п.8: Три способи створення об'єктів");
  std::cout << "1) Простий — конструктор без параметрів: CreditCard a;\n";
  CreditCard simple_card;

  std::cout << "2) Явний — явний виклик конструктора: "
               "CreditCard b = CreditCard(...);\n";
  CreditCard explicit_card =
      CreditCard("4441 1144 0000 1111", "ПриватБанк", "Visa", 2028, 20000);

  std::cout << "3) Скорочений — скорочений запис: CreditCard c(...);\n";
  CreditCard short_card("5375 4141 0000 2222", "Монобанк", "Mastercard", 2029,
                        15000);

  PrintHeader("п.10: Конструктори копіювання та класу Client");
  std::cout << "Копіювання картки: CreditCard d(short_card);\n";
  CreditCard copied_card(short_card);

  std::cout << "Клієнт без параметрів: Client unknown;\n";
  Client unknown_client;

  std::cout << "Клієнт з параметрами (картка копіюється всередину):\n";
  Client kovalenko(1, "Ірина", "Коваленко", "+380671112233", "UA100001",
                   12000, explicit_card);
  Client melnyk(2, "Андрій", "Мельник", "+380502223344", "UA100002", 3500,
                short_card);

  std::cout << "Копіювання клієнта: Client copy(kovalenko);\n";
  Client kovalenko_copy(kovalenko);
  kovalenko_copy.first_name_ = "Ірина (копія)";
  kovalenko_copy.Print();
  CreditCard::PrintStatistics();

  // -------------------------------------------------------------------------
  // п.3: деструктор відслідковує вихід об'єкта з блоку програми.
  // -------------------------------------------------------------------------
  PrintHeader("п.3: Деструктор і блоки програми");
  std::cout << "-> Вхід у зовнішній блок\n";
  {
    CreditCard outer_card("4149 6090 0000 3333", "Ощадбанк", "Visa", 2027,
                          10000);
    std::cout << "  -> Вхід у внутрішній блок\n";
    {
      CreditCard inner_card("5168 7450 0000 4444", "ПриватБанк", "Mastercard",
                            2030, 5000);
      CreditCard::PrintStatistics();
      std::cout << "  <- Вихід з внутрішнього блоку\n";
    }
    CreditCard::PrintStatistics();
    std::cout << "<- Вихід із зовнішнього блоку\n";
  }
  CreditCard::PrintStatistics();

  // -------------------------------------------------------------------------
  // п.4: спільне використання статичного поля та методу.
  // -------------------------------------------------------------------------
  PrintHeader("п.4: Статичне поле — спільна комісія");
  kovalenko.TransferToClient(melnyk, 1000);
  std::cout << "Банк змінює комісію статичним методом "
               "Client::set_commission_percent(2.5):\n";
  Client::set_commission_percent(2.5);
  std::cout << "  Нова комісія однакова для всіх: kovalenko бачить "
            << kovalenko.commission_percent() << "%, melnyk бачить "
            << melnyk.commission_percent() << "%\n";
  melnyk.TransferToClient(kovalenko, 1000);
  std::cout << "  Живих клієнтів (статичне поле): " << Client::alive_count()
            << '\n';

  // -------------------------------------------------------------------------
  // п.5, п.6: функції поза класом з параметром-об'єктом та адресою.
  // -------------------------------------------------------------------------
  PrintHeader("п.5: Функція з параметром-об'єктом (копія)");
  kovalenko.PayOrder(6500);
  PrintFinancialReport(kovalenko);

  PrintHeader("п.6: Перевантажена функція з параметром-адресою");
  PrintFinancialReport(&kovalenko);

  // -------------------------------------------------------------------------
  // п.7: функція, що повертає об'єкт класу.
  // -------------------------------------------------------------------------
  PrintHeader("п.7: Функція типу класу Client");
  Client premium = MakePremiumClient(melnyk, 50000);
  premium.Print();

  // -------------------------------------------------------------------------
  // п.9: покажчик на екземпляр класу; статична і динамічна пам'ять;
  // масиви об'єктів.
  // -------------------------------------------------------------------------
  PrintHeader("п.9: Покажчики, пам'ять, масиви об'єктів");
  std::cout << "Масив із 2 карток у статичній пам'яті (стек):\n";
  CreditCard card_array[2];

  std::cout << "Масив із 2 клієнтів у динамічній пам'яті (new[]):\n";
  Client* client_array = new Client[2];
  client_array[0].last_name_ = "Динамічний-1";
  client_array[1].last_name_ = "Динамічний-2";

  std::cout << "Один об'єкт у динамічній пам'яті (new):\n";
  Client* dynamic_client =
      new Client(3, "Олена", "Шевчук", "+380933334455", "UA100003", 27000,
                 CreditCard("4731 2190 0000 5555", "Райффайзен", "Visa", 2028,
                            25000));

  std::cout << "Робота через покажчик на екземпляр класу:\n";
  CreditCard* card_ptr = &card_array[0];
  card_ptr->set_credit_limit(1000);
  card_ptr->TopUp(250);
  dynamic_client->PayOrder(1200);
  dynamic_client->Print();
  CreditCard::PrintStatistics();

  std::cout << "Звільнення динамічної пам'яті (delete та delete[]):\n";
  delete dynamic_client;
  dynamic_client = nullptr;
  delete[] client_array;
  client_array = nullptr;
  CreditCard::PrintStatistics();

  // -------------------------------------------------------------------------
  // п.11: взаємодія трьох об'єктів через клас асоціації Payment
  // (Client <-> Order, Payment — сам зв'язок з атрибутами).
  // п.2: Payment містить поля-константи та поля-посилання.
  // -------------------------------------------------------------------------
  PrintHeader("п.11: Клас асоціації Payment (Client + Order + Payment)");
  Order laptop_order(101, "Ноутбук", 8000);
  Order phone_order(102, "Смартфон", 4000);
  laptop_order.Print();

  Payment card_payment(1, "23.09.2026", PaymentType::kCard, melnyk,
                       laptop_order);
  card_payment.Execute();
  card_payment.Print();
  laptop_order.Print();

  std::cout << "Повторна спроба оплатити те саме замовлення:\n";
  Payment duplicate_payment(2, "23.09.2026", PaymentType::kAccount, kovalenko,
                            laptop_order);
  duplicate_payment.Execute();

  std::cout << "Оплата іншого замовлення з рахунку:\n";
  Payment account_payment(3, "24.09.2026", PaymentType::kAccount, kovalenko,
                          phone_order);
  account_payment.Execute();

  std::cout << "Копія платежу (конструктор копіювання Payment):\n";
  Payment payment_copy(account_payment);
  std::cout << "Історія платежів:\n";
  card_payment.Print();
  duplicate_payment.Print();
  payment_copy.Print();

  // -------------------------------------------------------------------------
  // п.12: агрегація — Administrator зберігає покажчики на клієнтів.
  // Адміністратор знищується в кінці блоку, а клієнти залишаються.
  // -------------------------------------------------------------------------
  PrintHeader("п.12: Агрегація (Administrator -> Client*)");
  std::cout << "-> Вхід у блок\n";
  {
    Administrator admin("Петренко");
    admin.AddClient(&kovalenko);
    admin.AddClient(&melnyk);
    admin.ReviewLimitRequest(&kovalenko, 35000);  // затверджено
    admin.ReviewLimitRequest(&melnyk, 100000);    // відхилено
    melnyk.mutable_card().set_credit_limit(3000);
    admin.CheckAllCards();  // картку Мельника буде заблоковано
    std::cout << "<- Вихід з блоку\n";
  }
  std::cout << "Клієнти існують і після знищення адміністратора:\n";
  kovalenko.Print();
  melnyk.Print();

  // -------------------------------------------------------------------------
  // п.13: композиція — CreditCard є частиною Client. Картка створюється
  // разом із клієнтом і знищується разом із ним.
  // -------------------------------------------------------------------------
  PrintHeader("п.13: Композиція (Client містить CreditCard)");
  std::cout << "-> Вхід у блок\n";
  {
    Client temp_client(
        4, "Максим", "Бондар", "+380974445566", "UA100004", 800,
        CreditCard("4000 1234 0000 6666", "Монобанк", "Visa", 2031, 7000));
    temp_client.PayOrder(300);
    std::cout << "<- Вихід з блоку: спочатку ~Client(), потім ~CreditCard() "
                 "вкладеної картки\n";
  }
  CreditCard::PrintStatistics();

  // -------------------------------------------------------------------------
  // п.3: при виході з main знищуються всі локальні об'єкти у порядку,
  // зворотному до їх створення.
  // -------------------------------------------------------------------------
  PrintHeader("Кінець main(): знищення локальних об'єктів");
  return 0;
}
