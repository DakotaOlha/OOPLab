// Лабораторна робота №4, третій рівень, варіант 14 «Система Платежі».
// ЛР4 п.13: main.cpp — демонстрації ЛР3 і ЛР4 та меню вибору.

#include <iomanip>
#include <iostream>
#include <string>

#ifdef _WIN32
#include <windows.h>
#endif

#include "administrator.h"
#include "bank_account.h"
#include "client.h"
#include "credit_account.h"
#include "credit_card.h"
#include "current_account.h"
#include "installment_plan.h"
#include "order.h"
#include "payment.h"
#include "premium_account.h"
#include "savings_account.h"

namespace {

constexpr int kCurrentYear = 2026;

void PrintHeader(const std::string& title) {
  std::cout << "\n========== " << title << " ==========\n";
}

}  // namespace

// ЛР3 п.5: параметр — об'єкт (передається копія, тому спрацьовують
// конструктор копіювання і деструктор).
void PrintFinancialReport(Client client) {
  const CreditCard& card = client.card();
  const double debt = card.balance() < 0.0 ? -card.balance() : 0.0;
  const double own_on_card = card.balance() > 0.0 ? card.balance() : 0.0;
  double available_credit = card.credit_limit() - debt;

  if (available_credit < 0.0)
    available_credit = 0.0;

  const double usage_percent =
      card.credit_limit() > 0.0 ? debt / card.credit_limit() * 100.0 : 0.0;

  const double total_available =
      client.account_balance() + own_on_card + available_credit;

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

// ЛР3 п.6: перевантаження з параметром-адресою — копія не створюється.
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

// ЛР3 п.7: функція типу класу Client — повертає об'єкт класу.
Client MakePremiumClient(const Client& base, double premium_limit) {
  std::cout << "  Оформлення преміум-обслуговування для " << base.last_name_
            << ":\n";

  Client premium = base.ReissueCard(base.card().expiry_year_ + 4);
  premium.mutable_card().set_credit_limit(premium_limit);
  premium.mutable_card().TopUp(500.0);
  return premium;
}

void RunLab3Demo() {
  std::cout << "Лабораторна робота №3, третій рівень, варіант 14\n";

  // ЛР3 п.4: статичні елементи існують ще до створення жодного об'єкта.
  PrintHeader("п.4: Статичні поля до створення об'єктів");
  CreditCard::PrintStatistics();
  std::cout << "  Комісія за переказ для всіх клієнтів: "
            << Client::commission_percent() << "%\n";

  // ЛР3 п.8, п.10: три способи створення об'єктів і всі конструктори.
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

  // ЛР3 п.3: деструктор відслідковує вихід об'єкта з блоку програми.
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

  // ЛР3 п.4: спільне використання статичного поля та методу.
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

  // ЛР3 п.5, п.6: функції поза класом з параметром-об'єктом та адресою.
  PrintHeader("п.5: Функція з параметром-об'єктом (копія)");
  kovalenko.PayOrder(6500);
  PrintFinancialReport(kovalenko);

  PrintHeader("п.6: Перевантажена функція з параметром-адресою");
  PrintFinancialReport(&kovalenko);

  // ЛР3 п.7: функція, що повертає об'єкт класу.
  PrintHeader("п.7: Функція типу класу Client");
  Client premium = MakePremiumClient(melnyk, 50000);
  premium.Print();

  // ЛР3 п.9: покажчики, статична і динамічна пам'ять, масиви об'єктів.
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

  // ЛР3 п.11, п.2: клас асоціації Payment з const-полями і посиланнями.
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

  // ЛР3 п.12: агрегація — адміністратор знищується, клієнти залишаються.
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

  // ЛР3 п.13: композиція — картка створюється і знищується разом з клієнтом.
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

  // ЛР3 п.3: локальні об'єкти знищуються у зворотному порядку.
  PrintHeader("Кінець RunLab3Demo(): знищення локальних об'єктів");
}

void RunLab4Demo() {
  std::cout << "Лабораторна робота №4, третій рівень, варіант 14\n";

  // -------------------------------------------------------------------------
  // ЛР4 п.2, п.5: public-наслідування, об'єкт створено конструктором.
  // -------------------------------------------------------------------------
  PrintHeader("ЛР4 п.2, п.5: public-наслідування (CurrentAccount)");
  CurrentAccount current("Тарас Бойко", "ПриватБанк", "грн", "UA200001",
                         5000, 2019, "5168 0000 0000 1111", 5000, 50, 25000);
  std::cout << "Ззовні доступні public-поля бази: " << current.owner_name_
            << ", " << current.bank_name_ << ", " << current.currency_
            << '\n'
            << "і public-методи бази Deposit(), Withdraw(), PrintInfo():\n";
  current.Deposit(1000);
  current.Withdraw(500);
  current.BankAccount::PrintInfo();
  std::cout << "Protected/private-поля — лише через геттери: balance() = "
            << current.balance()
            << ", opening_year() = " << current.opening_year()
            << ", вік рахунку: " << current.AccountAgeYears(kCurrentYear)
            << " р.\n";
  // ПОМИЛКА: current.balance_ = 0; — 'balance_' is protected.
  // ПОМИЛКА: current.opening_year_ = 2000; — 'opening_year_' is private.

  PrintHeader("ЛР4 п.2, п.5: protected-наслідування (SavingsAccount)");
  SavingsAccount savings("Тарас Бойко", "Ощадбанк", "грн", "UA200002", 20000,
                         2021, 12.0, 12, 5000);
  std::cout << "Через using доступні: owner_name_ = " << savings.owner_name_
            << ", balance() = " << savings.balance() << ", PrintInfo():\n";
  savings.PrintInfo();
  savings.TopUp(3000);
  savings.AddMonthlyInterest();
  savings.EarlyWithdraw(2000);
  savings.PrintSavings();
  // ПОМИЛКА: savings.bank_name_; — public-поле бази стало protected.
  // ПОМИЛКА: savings.Deposit(100); — 'Deposit' is protected у SavingsAccount.
  // ПОМИЛКА: BankAccount& base = savings; — 'BankAccount' is inaccessible
  // base of 'SavingsAccount'.

  PrintHeader("ЛР4 п.2, п.5: private-наслідування (CreditAccount)");
  CreditAccount credit("Тарас Бойко", "Ощадбанк", "грн", "UA200003", 0, 2024,
                       30000, 2.0, 0);
  std::cout << "Ззовні доступні лише власні методи CreditAccount:\n";
  credit.Borrow(10000);
  credit.Repay(4000);
  credit.Print();
  // ПОМИЛКА: credit.owner_name_; — член бази став private.
  // ПОМИЛКА: credit.PrintInfo(); — 'PrintInfo' is private у CreditAccount.
  // ПОМИЛКА: credit.Deposit(100); — 'Deposit' is private у CreditAccount.

  // -------------------------------------------------------------------------
  // ЛР4 п.3: InstallmentPlan використовує методи BankAccount і CurrentAccount.
  // -------------------------------------------------------------------------
  PrintHeader("ЛР4 п.3: private-наслідування (InstallmentPlan)");
  InstallmentPlan plan("Олена Шевчук", "Райффайзен", "UA200004", 5000, 2022,
                       3000, "Ноутбук", 24000, 6, 11);
  plan.PrintSchedule();
  plan.PayNextPart();
  plan.TopUp(6000);
  plan.PayNextPart();
  std::cout << "Через using: " << plan.owner_name_
            << ", balance() = " << plan.balance() << '\n';
  plan.PrintSchedule();
  // ПОМИЛКА: plan.Withdraw(100); — CurrentAccount є private-базою.
  // ПОМИЛКА: plan.PrintInfo(); — 'PrintInfo' is private у InstallmentPlan.

  // -------------------------------------------------------------------------
  // ЛР4 п.4: множинне наслідування (PremiumAccount).
  // -------------------------------------------------------------------------
  PrintHeader("ЛР4 п.4: множинне наслідування (PremiumAccount)");
  PremiumAccount premium("Ірина Коваленко", "UA200005", 15000, 2018,
                         "4441 0000 0000 2222", 10000, "POL-0077", 50000,
                         "Кешбек+", 3.0, 2023);
  premium.PayWithCashback(2000);
  premium.ReportDamage(1200);
  std::cout << std::fixed << std::setprecision(2)
            << "Методи різних баз: AnnualPremium() = "
            << premium.AnnualPremium() << " грн, points() = "
            << premium.points() << ", card_number() = "
            << premium.card_number() << '\n';
  premium.PrintInfo();

  // -------------------------------------------------------------------------
  // ЛР4 п.6: доступ до членів бази через :: та using.
  // -------------------------------------------------------------------------
  PrintHeader("ЛР4 п.6: оператори :: та using");
  std::cout << "current.BankAccount::PrintInfo() — версія бази:\n";
  current.BankAccount::PrintInfo();
  std::cout << "current.PrintInfo() — версія CurrentAccount:\n";
  current.PrintInfo();
  std::cout << "Обидві версії Withdraw завдяки using BankAccount::Withdraw:\n";
  current.Withdraw(300);
  current.Withdraw(7000, true);
  std::cout << "premium.Insurance::Print() і premium.LoyaltyProgram::Print():"
            << '\n';
  premium.Insurance::Print();
  premium.LoyaltyProgram::Print();
  // ПОМИЛКА: premium.Print(); — request for member 'Print' is ambiguous:
  // Print() є і в Insurance, і в LoyaltyProgram; потрібне уточнення ::.
  std::cout << "premium.CurrentAccount::PrintInfo() — версія бази:\n";
  premium.CurrentAccount::PrintInfo();

  // -------------------------------------------------------------------------
  // ЛР4 п.7, п.8: порядок конструкторів/деструкторів і вихід з блоків.
  // -------------------------------------------------------------------------
  PrintHeader("ЛР4 п.7, п.8: порядок конструкторів і деструкторів");
  std::cout << "-> Вхід у блок 1 (просте public-наслідування)\n";
  {
    CurrentAccount block_current("Блок 1", "ПриватБанк", "грн", "UA900001",
                                 100, 2026, "0000 0000 0000 0001", 0, 0,
                                 1000);
    std::cout << "<- Вихід з блоку 1\n";
  }

  std::cout << "-> Вхід у блок 2 (protected-наслідування)\n";
  {
    SavingsAccount block_savings("Блок 2", "Ощадбанк", "грн", "UA900002",
                                 1000, 2026, 10.0, 6, 100);
    std::cout << "<- Вихід з блоку 2\n";
  }

  std::cout << "-> Вхід у блок 3 (private-наслідування)\n";
  {
    CreditAccount block_credit("Блок 3", "Ощадбанк", "грн", "UA900003", 0,
                               2026, 5000, 2.0, 0);
    std::cout << "<- Вихід з блоку 3\n";
  }

  std::cout << "-> Вхід у блок 4 (багаторівневе наслідування)\n";
  {
    InstallmentPlan block_plan("Блок 4", "Райффайзен", "UA900004", 0, 2026,
                               0, "Пилосос", 6000, 3, 1);
    std::cout << "  -> Вхід у вкладений блок 5 (множинне наслідування)\n";
    {
      PremiumAccount block_premium("Блок 5", "UA900005", 0, 2026,
                                   "0000 0000 0000 0005", 0, "POL-0005",
                                   1000, "Бонус", 1.0, 2026);
      std::cout << "  <- Вихід з вкладеного блоку 5\n";
    }
    std::cout << "<- Вихід з блоку 4\n";
  }

  // -------------------------------------------------------------------------
  // ЛР4 п.9: сценарій роботи системи з класами ЛР3 (Client, Order).
  // -------------------------------------------------------------------------
  PrintHeader("ЛР4 п.9: сценарій роботи системи платежів");
  std::cout << "-> Вхід у блок сценарію\n";
  {
    std::cout << "Клієнти та замовлення:\n";
    Client kovalenko(11, "Ірина", "Коваленко", "+380671112233", "UA100011",
                     3000,
                     CreditCard("4441 1144 0000 7777", "ПриватБанк", "Visa",
                                2029, 10000));
    Client melnyk(12, "Андрій", "Мельник", "+380502223344", "UA100012", 1500,
                  CreditCard("5375 4141 0000 8888", "Ощадбанк", "Mastercard",
                             2028, 5000));
    Client shevchuk(13, "Олена", "Шевчук", "+380933334455", "UA100013", 2000,
                    CreditCard("4731 2190 0000 9999", "Райффайзен", "Visa",
                               2030, 8000));
    Order smartphone(201, "Смартфон", 18000);
    Order delivery(202, "Доставка смартфона", 250);
    Order tv(203, "Телевізор", 22000);
    Order laptop(204, "Ноутбук (розстрочка)", 24000);

    std::cout << "Відкриття рахунків:\n";
    const std::string kovalenko_name =
        kovalenko.first_name_ + " " + kovalenko.last_name_;
    const std::string melnyk_name =
        melnyk.first_name_ + " " + melnyk.last_name_;
    const std::string shevchuk_name =
        shevchuk.first_name_ + " " + shevchuk.last_name_;
    PremiumAccount kovalenko_premium(
        kovalenko_name, "UA300001", 15000, 2018, kovalenko.card().number(),
        10000, "POL-1001", 50000, "Кешбек+", 3.0, 2023);
    SavingsAccount kovalenko_savings(kovalenko_name, "ПриватБанк", "грн",
                                     "UA300002", 20000, 2020, 12.0, 12, 5000);
    CreditAccount melnyk_credit(melnyk_name, "Ощадбанк", "грн", "UA300003",
                                0, 2025, 30000, 2.0, 0);
    CurrentAccount melnyk_current(melnyk_name, "Ощадбанк", "грн", "UA300004",
                                  5000, 2020, melnyk.card().number(), 5000,
                                  50, 25000);
    InstallmentPlan shevchuk_plan(shevchuk_name, "Райффайзен", "UA300005",
                                  5000, 2022, 3000, laptop.description(),
                                  laptop.amount(), 6, 11);

    std::cout << "\nКоваленко купує смартфон з преміального рахунку:\n";
    if (kovalenko_premium.PayWithCashback(smartphone.amount()))
      smartphone.MarkPaid();
    std::cout << "Коваленко оплачує доставку зі свого рахунку (ЛР3):\n";
    if (kovalenko.PayOrder(delivery.amount(), true))
      delivery.MarkPaid();

    std::cout << "\nМельник бере кредит і переказує гроші на поточний "
                 "рахунок:\n";
    melnyk_credit.Borrow(20000);
    melnyk_credit.TransferTo(melnyk_current, 15000);
    std::cout << "Мельник купує телевізор з овердрафтом:\n";
    if (melnyk_current.Withdraw(tv.amount(), true))
      tv.MarkPaid();

    std::cout << "\nШевчук сплачує першу частину плану виплат:\n";
    shevchuk_plan.PayNextPart();
    shevchuk_plan.TopUp(3000);

    std::cout << "\nМинув місяць — щомісячні операції:\n";
    kovalenko_premium.StartNewDay();
    kovalenko_premium.ChargeMonthlyFee();
    kovalenko_premium.ReportDamage(1500);
    kovalenko_savings.AddMonthlyInterest();
    std::cout << "Коваленко знімає з депозиту, щоб закрити овердрафт:\n";
    if (kovalenko_savings.EarlyWithdraw(3000))
      kovalenko_premium.Deposit(3000);
    melnyk_current.StartNewDay();
    melnyk_current.ChargeMonthlyFee();
    melnyk_credit.AccrueMonthlyInterest();
    melnyk_credit.Repay(5000);
    shevchuk_plan.PayNextPart();
    if (shevchuk_plan.is_completed())
      laptop.MarkPaid();

    std::cout << "\nПідсумок — замовлення:\n";
    smartphone.Print();
    delivery.Print();
    tv.Print();
    laptop.Print();
    std::cout << "Підсумок — клієнти і рахунки:\n";
    kovalenko.Print();
    kovalenko_premium.PrintInfo();
    kovalenko_savings.PrintSavings();
    melnyk_current.PrintInfo();
    melnyk_credit.Print();
    shevchuk_plan.PrintSchedule();
    std::cout << "<- Вихід з блоку сценарію\n";
  }

  PrintHeader("Кінець RunLab4Demo(): знищення локальних об'єктів");
}

int main() {
#ifdef _WIN32
  SetConsoleOutputCP(CP_UTF8);
  SetConsoleCP(CP_UTF8);
#endif

  std::cout << "Оберіть демонстрацію: 3 — ЛР3, 4 — ЛР4: ";
  int choice = 0;
  std::cin >> choice;

  if (choice == 3) {
    RunLab3Demo();
  } else if (choice == 4) {
    RunLab4Demo();
  } else {
    std::cout << "Невідомий вибір.\n";
  }
  return 0;
}
