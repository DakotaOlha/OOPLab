// Лабораторна робота №7, третій рівень, варіант 14 «Система Платежі».
// ЛР7 п.1–п.4: демонстрація контейнерів, ітераторів, алгоритмів, функторів.

#include "lab7_demo.h"

#include <algorithm>
#include <cstddef>
#include <deque>
#include <forward_list>
#include <functional>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <memory>
#include <numeric>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include "credit_card.h"
#include "financial_product.h"
#include "functors.h"
#include "insurance.h"
#include "loyalty_program.h"
#include "order.h"
#include "payment_record.h"
#include "savings_account.h"

namespace {

constexpr int kYear = 2026;

void PrintHeader(const std::string& title) {
  std::cout << "\n========== " << title << " ==========\n";
}

void PrintStep(const std::string& text) {
  std::cout << "-- " << text << '\n';
}

std::vector<PaymentRecord> SamplePayments() {
  return {
      PaymentRecord("2026-11-07", 1830.40, PaymentType::kCard, "Олена Чаюк",
                    "Сільпо"),
      PaymentRecord("2026-11-01", 9500.00, PaymentType::kAccount,
                    "Олена Чаюк", "ОСББ «Затишок»"),
      PaymentRecord("2026-11-09", 3000.00, PaymentType::kCard,
                    "Андрій Мельник", "Олена Чаюк"),
      PaymentRecord("2026-11-03", 250.00, PaymentType::kCard, "Олена Чаюк",
                    "Київстар"),
      PaymentRecord("2026-11-05", 250.00, PaymentType::kCard,
                    "Андрій Мельник", "Київстар"),
  };
}

template <typename Container>
void PrintRecords(const std::string& label, const Container& records) {
  std::cout << "  " << label << " (" << records.size() << "):\n";
  for (const PaymentRecord& record : records)
    std::cout << "    " << record << '\n';
}

template <typename Container>
void PrintAmounts(const std::string& label, const Container& amounts) {
  std::cout << "  " << label << ':' << std::fixed << std::setprecision(2);
  for (double amount : amounts)
    std::cout << ' ' << amount;
  std::cout << '\n';
}

template <typename Container>
void PrintStrings(const std::string& label, const Container& items) {
  std::cout << "  " << label << ':';
  for (const std::string& item : items)
    std::cout << " [" << item << ']';
  std::cout << '\n';
}

void PrintOrders(const std::string& label, const std::list<Order>& orders) {
  std::cout << "  " << label << " (" << orders.size() << "):\n";
  for (const Order& order : orders) {
    std::cout << "  ";
    order.Print();
  }
}

// ЛР7 п.2: категорію визначає iterator_traits<It>::iterator_category –
// тег, від якого залежить, які операції (++, --, it + n) дозволені.
template <typename It>
std::string CategoryName() {
  using Category = typename std::iterator_traits<It>::iterator_category;
  if (std::is_same<Category, std::random_access_iterator_tag>::value)
    return "random access (довільного доступу)";
  if (std::is_same<Category, std::bidirectional_iterator_tag>::value)
    return "bidirectional (двонаправлений)";
  if (std::is_same<Category, std::forward_iterator_tag>::value)
    return "forward (односпрямований)";
  if (std::is_same<Category, std::input_iterator_tag>::value)
    return "input (вхідний)";
  if (std::is_same<Category, std::output_iterator_tag>::value)
    return "output (вихідний)";
  return "невідома";
}

template <typename It, typename Tag>
constexpr bool kIsCategory = std::is_same<
    typename std::iterator_traits<It>::iterator_category, Tag>::value;

// ЛР7 п.1.1: vector – історія платежів (push_back, at/[], erase).
void DemoVector() {
  PrintHeader("ЛР7 п.1.1: vector<PaymentRecord> – історія платежів");
  std::vector<PaymentRecord> history;
  std::cout << "  До: size() = " << history.size() << '\n';
  for (const PaymentRecord& record : SamplePayments())
    history.push_back(record);
  PrintRecords("Після push_back", history);

  PrintStep("at(1) і [3] – доступ за індексом");
  std::cout << "    at(1): " << history.at(1) << '\n'
            << "    [3]:   " << history[3] << '\n';
  try {
    std::cout << history.at(10) << '\n';
  } catch (const std::out_of_range&) {
    std::cout << "    at(10): виняток std::out_of_range\n";
  }

  PrintStep("erase(begin() + 3) – видалення 4-го платежу");
  // Після erase ітератори на видалений і наступні елементи недійсні,
  // тому використовуємо лише повернений erase ітератор.
  const auto next = history.erase(history.begin() + 3);
  std::cout << "    erase повернув ітератор на: " << *next << '\n';
  PrintRecords("Після erase", history);
}

// ЛР7 п.1.2: list – замовлення клієнта (push_front/back, insert, sort,
// remove_if).
void DemoList() {
  PrintHeader("ЛР7 п.1.2: list<Order> – замовлення клієнта");
  std::list<Order> orders;
  std::cout << "  До: size() = " << orders.size() << '\n';
  orders.push_back(Order(201, "Ноутбук", 32000));
  orders.push_back(Order(202, "Мишка", 800));
  orders.push_front(Order(200, "Терміновий ремонт", 1500));
  PrintOrders("Після push_back і push_front", orders);

  PrintStep("insert перед 3-м елементом (вставка в середину за O(1))");
  orders.insert(std::next(orders.begin(), 2), Order(203, "Чохол", 450));
  PrintOrders("Після insert", orders);

  PrintStep("sort за сумою і remove_if оплачених замовлень");
  orders.front().MarkPaid();
  orders.sort([](const Order& a, const Order& b) {
    return a.amount() < b.amount();
  });
  PrintOrders("Після MarkPaid() і sort", orders);
  orders.remove_if([](const Order& order) { return order.is_paid(); });
  PrintOrders("Після remove_if", orders);
}

void PrintProductQueue(
    const std::deque<std::unique_ptr<FinancialProduct>>& queue) {
  std::cout << "  Черга (" << queue.size() << "):";
  for (const auto& product : queue)
    std::cout << " [№" << product->id() << ' ' << product->TypeName() << ']';
  std::cout << '\n';
}

// ЛР7 п.1.3: deque – черга обслуговування (push_back, push_front,
// pop_front).
void DemoDeque() {
  PrintHeader("ЛР7 п.1.3: deque<unique_ptr<FinancialProduct>> – черга");
  // Продукти ієрархії зберігаються через unique_ptr: за значенням об'єкт
  // похідного класу «обрізався» б до базового, а копія FinancialProduct
  // створює новий продукт. unique_ptr робить deque єдиним власником.
  std::deque<std::unique_ptr<FinancialProduct>> queue;
  std::cout << "  До: size() = " << queue.size() << '\n';
  PrintStep("push_back – звичайні звернення");
  queue.push_back(std::make_unique<SavingsAccount>(
      "Олена Чаюк", "Ощадбанк", "грн", "UA710001", 10000, kYear, 11.0, 12,
      1000));
  queue.push_back(std::make_unique<LoyaltyProgram>(
      "Андрій Мельник", "Бонус+", "Silver", 2.0, 1, kYear));
  PrintStep("push_front – термінове звернення стає першим");
  queue.push_front(std::make_unique<Insurance>(
      "Ірина Коваленко", kYear, "POL-7003", "СК «Гарант»", 30000, 1.5, 1));
  PrintProductQueue(queue);

  PrintStep("pop_front – обслуговано першого; unique_ptr знищує продукт");
  queue.pop_front();
  std::cout << "  Після pop_front:\n";
  PrintProductQueue(queue);
  std::cout << "  Кінець блоку – deque знищує решту продуктів:\n";
}

// ЛР7 п.1.4: set – унікальні номери карток (insert, find/count, erase).
void DemoSet() {
  PrintHeader("ЛР7 п.1.4: set<string> – унікальні номери карток");
  std::set<std::string> cards;
  std::cout << "  До: size() = " << cards.size() << '\n';
  cards.insert("5168 0000 0000 7002");
  cards.insert("4441 0000 0000 7001");
  cards.insert("5375 0000 0000 7003");
  const bool added = cards.insert("4441 0000 0000 7001").second;
  PrintStrings("Після insert (відсортовано)", cards);
  std::cout << "  insert дубліката «4441 … 7001»: "
            << (added ? "додано" : "не додано, ключ уже є") << '\n';

  PrintStep("find і count");
  const auto it = cards.find("5168 0000 0000 7002");
  std::cout << "    find(«5168 … 7002»): "
            << (it != cards.end() ? "знайдено" : "немає")
            << ", count(«4441 … 7001») = "
            << cards.count("4441 0000 0000 7001") << '\n';

  PrintStep("erase(«5375 0000 0000 7003») – картку закрито");
  const std::size_t removed = cards.erase("5375 0000 0000 7003");
  std::cout << "    видалено елементів: " << removed << '\n';
  PrintStrings("Після erase", cards);
}

// ЛР7 п.1.5: multiset – суми платежів з повторами (insert, count,
// equal_range/lower_bound).
void DemoMultiset() {
  PrintHeader("ЛР7 п.1.5: multiset<double> – суми платежів з повторами");
  std::multiset<double> amounts;
  std::cout << "  До: size() = " << amounts.size() << '\n';
  for (const PaymentRecord& record : SamplePayments())
    amounts.insert(record.amount());
  amounts.insert(250.0);
  PrintAmounts("Після insert (з повторами)", amounts);

  PrintStep("count(250) і equal_range(250)");
  const auto range = amounts.equal_range(250.0);
  std::cout << "    count(250) = " << amounts.count(250.0)
            << ", у equal_range елементів: "
            << std::distance(range.first, range.second) << '\n';

  PrintStep("lower_bound(1000) – перша сума не менша за 1000");
  std::cout << "    *lower_bound(1000) = " << *amounts.lower_bound(1000.0)
            << '\n';
}

// ЛР7 п.1.6: map – баланс за номером рахунку ([]/insert, find, erase).
void DemoMap() {
  PrintHeader("ЛР7 п.1.6: map<string, double> – баланс за рахунком");
  std::map<std::string, double> balances;
  std::cout << "  До: size() = " << balances.size() << '\n';
  balances["UA710002"] = 12000.0;
  balances["UA710001"] = 5000.0;
  balances.insert({"UA710003", 800.0});
  const bool inserted = balances.insert({"UA710001", 1.0}).second;
  std::cout << "  Після [] і insert:\n";
  for (const auto& entry : balances)
    std::cout << "    " << entry.first << " → " << entry.second << " грн\n";
  std::cout << "  insert існуючого ключа UA710001: "
            << (inserted ? "перезаписано" : "не перезаписує значення")
            << '\n';

  PrintStep("find(«UA710002») і поповнення на 500");
  const auto it = balances.find("UA710002");
  if (it != balances.end())
    it->second += 500.0;
  PrintStep("erase(«UA710003») – рахунок закрито");
  balances.erase("UA710003");
  std::cout << "  Після find і erase:\n";
  for (const auto& entry : balances)
    std::cout << "    " << entry.first << " → " << entry.second << " грн\n";
}

// ЛР7 п.1.7: multimap – платежі за відправником (insert, equal_range,
// count).
void DemoMultimap() {
  PrintHeader("ЛР7 п.1.7: multimap<string, PaymentRecord> – за відправником");
  std::multimap<std::string, PaymentRecord> by_sender;
  std::cout << "  До: size() = " << by_sender.size() << '\n';
  for (const PaymentRecord& record : SamplePayments())
    by_sender.insert({record.sender(), record});
  std::cout << "  Після insert (" << by_sender.size() << "):\n";
  for (const auto& entry : by_sender)
    std::cout << "    " << entry.first << ": " << entry.second << '\n';

  const std::string sender = "Андрій Мельник";
  PrintStep("count і equal_range для «" + sender + "»");
  std::cout << "    count = " << by_sender.count(sender) << '\n';
  const auto range = by_sender.equal_range(sender);
  for (auto it = range.first; it != range.second; ++it)
    std::cout << "    " << it->second << '\n';
}

// ЛР7 п.1.8: stack – скасування останніх операцій (push, top, pop).
void DemoStack() {
  PrintHeader("ЛР7 п.1.8: stack<PaymentRecord> – скасування операцій");
  std::stack<PaymentRecord> undo;
  std::cout << "  До: size() = " << undo.size() << '\n';
  for (const PaymentRecord& record : SamplePayments())
    undo.push(record);
  std::cout << "  Після push: size() = " << undo.size() << '\n'
            << "  top() – остання операція: " << undo.top() << '\n';
  PrintStep("pop() двічі – скасовано дві останні операції");
  undo.pop();
  undo.pop();
  std::cout << "  Після pop: size() = " << undo.size()
            << ", top() = " << undo.top() << '\n';
}

// ЛР7 п.1.9: queue – замовлення в порядку надходження (push, front, pop).
void DemoQueue() {
  PrintHeader("ЛР7 п.1.9: queue<Order> – замовлення на оплату");
  std::queue<Order> to_pay;
  std::cout << "  До: size() = " << to_pay.size() << '\n';
  to_pay.push(Order(301, "Комуналка", 2100));
  to_pay.push(Order(302, "Інтернет", 300));
  to_pay.push(Order(303, "Підписка", 199));
  std::cout << "  Після push: size() = " << to_pay.size() << ", front():\n";
  to_pay.front().Print();
  PrintStep("pop() – оплачено перше замовлення (FIFO)");
  to_pay.pop();
  std::cout << "  Після pop: size() = " << to_pay.size() << ", front():\n";
  to_pay.front().Print();
}

// ЛР7 п.1.10: priority_queue – платежі за пріоритетом (push, top, pop).
void DemoPriorityQueue() {
  PrintHeader("ЛР7 п.1.10: priority_queue<PaymentRecord> – за сумою");
  std::priority_queue<PaymentRecord> largest_first;
  std::cout << "  До: size() = " << largest_first.size() << '\n';
  for (const PaymentRecord& record : SamplePayments())
    largest_first.push(record);
  PrintStep("operator< з ЛР6: top() – найбільший платіж");
  std::cout << "  size() = " << largest_first.size() << '\n';
  while (!largest_first.empty()) {
    std::cout << "    top: " << largest_first.top() << '\n';
    largest_first.pop();
  }
  std::cout << "  Після pop усіх: size() = " << largest_first.size() << '\n';

  PrintStep("компаратор std::greater<PaymentRecord>: найменший першим");
  std::priority_queue<PaymentRecord, std::vector<PaymentRecord>,
                      std::greater<PaymentRecord>>
      smallest_first;
  for (const PaymentRecord& record : SamplePayments())
    smallest_first.push(record);
  std::cout << "    top: " << smallest_first.top() << '\n';
  smallest_first.pop();
  std::cout << "    після pop top: " << smallest_first.top() << '\n';
}

// ЛР7 п.2.1: вхідний ітератор – лише читання і ++, один прохід.
void DemoInputIterator() {
  PrintHeader("ЛР7 п.2.1: вхідний ітератор istream_iterator<double>");
  using InputIt = std::istream_iterator<double>;
  static_assert(kIsCategory<InputIt, std::input_iterator_tag>,
                "istream_iterator має бути вхідним");
  std::istringstream input("1830.40 9500 3000 250 250");
  std::cout << "  Потік: «" << input.str() << "»\n";
  const std::vector<double> amounts{InputIt(input), InputIt()};
  std::cout << "  Категорія: " << CategoryName<InputIt>() << '\n';
  PrintAmounts("Прочитано у vector<double>", amounts);
}

// ЛР7 п.2.2: вихідні ітератори – лише запис і ++.
void DemoOutputIterator() {
  PrintHeader("ЛР7 п.2.2: вихідні ітератори ostream_iterator, back_inserter");
  using OstreamIt = std::ostream_iterator<PaymentRecord>;
  using BackIt = std::back_insert_iterator<std::vector<PaymentRecord>>;
  static_assert(kIsCategory<OstreamIt, std::output_iterator_tag>,
                "ostream_iterator має бути вихідним");
  static_assert(kIsCategory<BackIt, std::output_iterator_tag>,
                "back_insert_iterator має бути вихідним");
  const std::vector<PaymentRecord> payments = SamplePayments();
  std::cout << "  Категорії: ostream_iterator – " << CategoryName<OstreamIt>()
            << ", back_inserter – " << CategoryName<BackIt>() << '\n';

  PrintStep("copy у ostream_iterator (виводить через operator<< з ЛР6)");
  std::copy(payments.begin(), payments.begin() + 2,
            OstreamIt(std::cout, "\n"));

  PrintStep("copy_if через back_inserter – лише платежі карткою");
  std::vector<PaymentRecord> by_card;
  std::copy_if(payments.begin(), payments.end(), std::back_inserter(by_card),
               [](const PaymentRecord& r) {
                 return r.type() == PaymentType::kCard;
               });
  PrintRecords("Після copy_if", by_card);
}

// ЛР7 п.2.3: односпрямований ітератор – лише ++, багатопрохідний.
void DemoForwardIterator() {
  PrintHeader("ЛР7 п.2.3: односпрямований ітератор forward_list");
  using ForwardIt = std::forward_list<std::string>::iterator;
  static_assert(kIsCategory<ForwardIt, std::forward_iterator_tag>,
                "forward_list має односпрямований ітератор");
  std::forward_list<std::string> recipients{"Київстар", "Сільпо"};
  PrintStrings("До", recipients);
  recipients.push_front("ОСББ «Затишок»");
  recipients.insert_after(recipients.begin(), "Укрпошта");
  std::cout << "  Категорія: " << CategoryName<ForwardIt>() << '\n'
            << "  Обхід лише вперед (++):";
  int position = 1;
  for (ForwardIt it = recipients.begin(); it != recipients.end(); ++it)
    std::cout << ' ' << position++ << '.' << *it;
  std::cout << '\n';
}

// ЛР7 п.2.4: двонаправлені ітератори – ++ і --, reverse_iterator.
void DemoBidirectionalIterator() {
  PrintHeader("ЛР7 п.2.4: двонаправлені ітератори list, set, map");
  using ListIt = std::list<Order>::iterator;
  using SetIt = std::set<std::string>::iterator;
  using MapIt = std::map<std::string, double>::reverse_iterator;
  static_assert(kIsCategory<ListIt, std::bidirectional_iterator_tag>,
                "list має двонаправлений ітератор");
  static_assert(kIsCategory<SetIt, std::bidirectional_iterator_tag>,
                "set має двонаправлений ітератор");
  static_assert(kIsCategory<MapIt, std::bidirectional_iterator_tag>,
                "reverse_iterator map – двонаправлений");
  std::cout << "  Категорія list/set/map: " << CategoryName<ListIt>() << '\n';

  std::list<Order> orders{Order(401, "Квитки", 1200),
                          Order(402, "Готель", 5400),
                          Order(403, "Таксі", 350)};
  PrintStep("list<Order>: вперед (++), потім назад (--)");
  ListIt it = orders.begin();
  ++it;
  std::cout << "    після ++: #" << it->id();
  ++it;
  std::cout << ", після ++: #" << it->id();
  --it;
  std::cout << ", після --: #" << it->id() << '\n';

  PrintStep("set<string>: обхід назад через rbegin()/rend()");
  const std::set<std::string> senders{"Андрій", "Борис", "Олена"};
  std::cout << "   ";
  for (auto r = senders.rbegin(); r != senders.rend(); ++r)
    std::cout << ' ' << *r;
  std::cout << '\n';

  PrintStep("map<string, double>: від найбільшого номера рахунку");
  std::map<std::string, double> balances{{"UA01", 100.0}, {"UA02", 250.0},
                                         {"UA03", 75.5}};
  for (MapIt r = balances.rbegin(); r != balances.rend(); ++r)
    std::cout << "    " << r->first << " → " << r->second << '\n';
}

// ЛР7 п.2.5: ітератор довільного доступу – it + n, it[k], різниця.
void DemoRandomAccessIterator() {
  PrintHeader("ЛР7 п.2.5: ітератор довільного доступу vector, deque");
  using VectorIt = std::vector<PaymentRecord>::iterator;
  using DequeIt = std::deque<double>::iterator;
  static_assert(kIsCategory<VectorIt, std::random_access_iterator_tag>,
                "vector має ітератор довільного доступу");
  static_assert(kIsCategory<DequeIt, std::random_access_iterator_tag>,
                "deque має ітератор довільного доступу");
  std::vector<PaymentRecord> payments = SamplePayments();
  std::cout << "  Категорія vector/deque: " << CategoryName<VectorIt>()
            << '\n';
  const VectorIt first = payments.begin();
  std::cout << "    *(begin + 2): " << *(first + 2) << '\n'
            << "    begin[4]:     " << first[4] << '\n'
            << "    end - begin = " << (payments.end() - first) << '\n';
  PrintStep("sort вимагає довільного доступу – хронологія платежів");
  std::sort(payments.begin(), payments.end(), EarlierDate());
  PrintRecords("Після sort за датою", payments);
}

// ЛР7 п.3: алгоритми sort, find_if, count_if, accumulate, transform
// (+ max_element, remove_if/erase, binary_search, for_each).
void DemoAlgorithms() {
  PrintHeader("ЛР7 п.3: алгоритми STL");
  std::vector<PaymentRecord> payments = SamplePayments();
  PrintRecords("До", payments);

  PrintStep("1) sort з функтором EarlierDate");
  std::sort(payments.begin(), payments.end(), EarlierDate());
  PrintRecords("Після sort", payments);

  PrintStep("2) find_if – перший платіж до «Київстар»");
  const auto found =
      std::find_if(payments.begin(), payments.end(),
                   [](const PaymentRecord& r) {
                     return r.receiver() == "Київстар";
                   });
  if (found != payments.end())
    std::cout << "    " << *found << '\n';

  PrintStep("3) count_if з функтором AmountInRange(200, 2000)");
  std::cout << "    платежів у діапазоні: "
            << std::count_if(payments.begin(), payments.end(),
                             AmountInRange(200.0, 2000.0))
            << '\n';

  PrintStep("4) accumulate – загальна сума платежів");
  const double total = std::accumulate(
      payments.begin(), payments.end(), 0.0,
      [](double sum, const PaymentRecord& r) { return sum + r.amount(); });
  std::cout << std::fixed << std::setprecision(2) << "    сума = " << total
            << " грн\n";

  PrintStep("5) transform з функтором CommissionCalculator(1.5)");
  std::vector<double> fees(payments.size());
  std::transform(payments.begin(), payments.end(), fees.begin(),
                 CommissionCalculator(1.5));
  PrintAmounts("Комісія 1.5%", fees);

  PrintStep("Додатково: max_element, remove_if + erase, binary_search");
  const auto largest = std::max_element(payments.begin(), payments.end());
  std::cout << "    max_element: " << *largest << '\n';
  // remove_if лише зсуває елементи, що лишаються, на початок і повертає
  // новий логічний кінець; фізично видаляє «хвіст» erase.
  payments.erase(std::remove_if(payments.begin(), payments.end(),
                                AmountInRange(0.0, 300.0)),
                 payments.end());
  PrintRecords("Після remove_if(сума ≤ 300) + erase", payments);
  std::vector<double> sorted_fees = fees;
  std::sort(sorted_fees.begin(), sorted_fees.end());
  std::cout << "    binary_search(142.50) у відсортованих комісіях: "
            << std::boolalpha
            << std::binary_search(sorted_fees.begin(), sorted_fees.end(),
                                  142.5)
            << std::noboolalpha << '\n';
  std::cout << "    for_each – отримувачі:";
  std::for_each(payments.begin(), payments.end(), [](const PaymentRecord& r) {
    std::cout << ' ' << r.receiver();
  });
  std::cout << '\n';
}

// ЛР7 п.4: власні функтори, які використовують інші компоненти.
void DemoOwnFunctors() {
  PrintHeader("ЛР7 п.4: власні функтори");
  const CommissionCalculator commission(1.0);
  const AmountInRange small(0.0, 500.0);
  const EarlierDate earlier;
  const std::vector<PaymentRecord> payments = SamplePayments();
  std::cout << std::fixed << std::setprecision(2)
            << "  CommissionCalculator(1.0)(9500) = " << commission(9500.0)
            << " грн\n"
            << "  AmountInRange(0, 500)(250) = " << std::boolalpha
            << small(250.0) << ", (1830.40) = " << small(1830.40) << '\n'
            << "  EarlierDate(" << payments[3].date() << ", "
            << payments[0].date()
            << ") = " << earlier(payments[3], payments[0])
            << std::noboolalpha << '\n';
}

// ЛР7 п.4.1: арифметичні функтори.
void DemoArithmeticFunctors() {
  PrintHeader("ЛР7 п.4.1: арифметичні функтори");
  std::cout << std::fixed << std::setprecision(2);
  const std::vector<double> balances{5000.0, 12000.0, 800.0};
  PrintAmounts("Баланси рахунків", balances);

  std::vector<double> result(balances.size());
  const std::vector<double> top_ups{500.0, 1000.0, 200.0};
  std::transform(balances.begin(), balances.end(), top_ups.begin(),
                 result.begin(), std::plus<double>());
  PrintAmounts("std::plus – поповнення (+500, +1000, +200)", result);

  const std::vector<double> charges{250.0, 3000.0, 100.0};
  std::transform(balances.begin(), balances.end(), charges.begin(),
                 result.begin(), std::minus<double>());
  PrintAmounts("std::minus – списання (-250, -3000, -100)", result);

  const std::multiplies<double> multiply;
  std::transform(balances.begin(), balances.end(), result.begin(),
                 [&multiply](double b) { return multiply(b, 0.01); });
  PrintAmounts("std::multiplies – комісія 1% (баланс × 0.01)", result);

  const std::vector<PaymentRecord> payments = SamplePayments();
  const double total = std::accumulate(
      payments.begin(), payments.end(), 0.0,
      [](double sum, const PaymentRecord& r) { return sum + r.amount(); });
  const double count = static_cast<double>(payments.size());
  std::cout << "  std::divides – середній платіж: " << total << " / "
            << count << " = " << std::divides<double>()(total, count)
            << " грн\n";

  const std::vector<long long> kopecks{183040, 9990, 64025};
  std::vector<long long> remainder(kopecks.size());
  const std::modulus<long long> mod;
  std::transform(kopecks.begin(), kopecks.end(), remainder.begin(),
                 [&mod](long long k) { return mod(k, 100LL); });
  std::cout << "  std::modulus – копійки (сума в коп. % 100):";
  for (std::size_t i = 0; i < kopecks.size(); ++i)
    std::cout << ' ' << kopecks[i] << " → " << remainder[i];
  std::cout << '\n';

  const std::vector<double> refunds{250.0, 1830.40};
  std::vector<double> negated(refunds.size());
  std::transform(refunds.begin(), refunds.end(), negated.begin(),
                 std::negate<double>());
  PrintAmounts("std::negate – повернення коштів", negated);
}

// ЛР7 п.4.2: функтори порівняння.
void DemoComparisonFunctors() {
  PrintHeader("ЛР7 п.4.2: функтори порівняння");
  const std::vector<PaymentRecord> payments = SamplePayments();
  std::vector<double> amounts;
  for (const PaymentRecord& record : payments)
    amounts.push_back(record.amount());
  PrintAmounts("Суми", amounts);

  const std::string target = "Київстар";
  const std::equal_to<std::string> equal;
  const std::not_equal_to<std::string> not_equal;
  std::cout << "  std::equal_to – платежів до «" << target << "»: "
            << std::count_if(payments.begin(), payments.end(),
                             [&](const PaymentRecord& r) {
                               return equal(r.receiver(), target);
                             })
            << '\n'
            << "  std::not_equal_to – платежів іншим отримувачам: "
            << std::count_if(payments.begin(), payments.end(),
                             [&](const PaymentRecord& r) {
                               return not_equal(r.receiver(), target);
                             })
            << '\n';

  std::sort(amounts.begin(), amounts.end(), std::greater<double>());
  PrintAmounts("std::greater – sort за спаданням", amounts);
  std::sort(amounts.begin(), amounts.end(), std::less<double>());
  PrintAmounts("std::less – sort за зростанням", amounts);

  const std::greater_equal<double> at_least;
  const std::less_equal<double> at_most;
  std::cout << "  std::greater_equal – платежів ≥ 1000: "
            << std::count_if(amounts.begin(), amounts.end(),
                             [&](double a) { return at_least(a, 1000.0); })
            << '\n'
            << "  std::less_equal – платежів ≤ 500: "
            << std::count_if(amounts.begin(), amounts.end(),
                             [&](double a) { return at_most(a, 500.0); })
            << '\n';
}

// ЛР7 п.4.3: логічні функтори – комбінування умов для карток.
void DemoLogicalFunctors() {
  PrintHeader("ЛР7 п.4.3: логічні функтори (картки CreditCard)");
  std::vector<CreditCard> cards;
  cards.reserve(3);
  cards.emplace_back("4441 0000 0000 7101", "ПриватБанк", "Visa", 2029,
                     5000);
  cards.emplace_back("5168 0000 0000 7102", "Ощадбанк", "Mastercard", 2028,
                     3000);
  cards.emplace_back("5375 0000 0000 7103", "Монобанк", "Mastercard", 2030,
                     2000);
  cards[0] += 2500;
  cards[1] -= 400;
  cards[2] += 1500;
  cards[2].Block();
  std::cout << "  Картки:\n";
  for (const CreditCard& card : cards) {
    std::cout << std::fixed << std::setprecision(2) << "    "
              << card.short_number() << ": баланс " << card.balance()
              << ", " << (card.is_blocked() ? "заблокована" : "активна")
              << '\n';
  }

  const double kMinBalance = 1000.0;
  const std::logical_and<bool> both;
  const std::logical_or<bool> either;
  const std::logical_not<bool> negation;
  std::cout << "  std::logical_and – активна І баланс ≥ 1000: "
            << std::count_if(cards.begin(), cards.end(),
                             [&](const CreditCard& c) {
                               return both(!c.is_blocked(),
                                           c.balance() >= kMinBalance);
                             })
            << '\n'
            << "  std::logical_or – заблокована АБО борг: "
            << std::count_if(cards.begin(), cards.end(),
                             [&](const CreditCard& c) {
                               return either(c.is_blocked(),
                                             c.balance() < 0.0);
                             })
            << '\n'
            << "  std::logical_not – НЕ заблоковані: "
            << std::count_if(cards.begin(), cards.end(),
                             [&](const CreditCard& c) {
                               return negation(c.is_blocked());
                             })
            << '\n';
  std::cout << "  Кінець блоку – vector знищує картки:\n";
}

}  // namespace

void RunLab7Demo() {
  std::cout << "Лабораторна робота №7 – бібліотека STL\n";
  DemoVector();
  DemoList();
  DemoDeque();
  DemoSet();
  DemoMultiset();
  DemoMap();
  DemoMultimap();
  DemoStack();
  DemoQueue();
  DemoPriorityQueue();
  DemoInputIterator();
  DemoOutputIterator();
  DemoForwardIterator();
  DemoBidirectionalIterator();
  DemoRandomAccessIterator();
  DemoAlgorithms();
  DemoOwnFunctors();
  DemoArithmeticFunctors();
  DemoComparisonFunctors();
  DemoLogicalFunctors();
  std::cout << "\nКінець демонстрації ЛР7.\n";
  CreditCard::PrintStatistics();
  std::cout << "  Живих фінансових продуктів: "
            << FinancialProduct::alive_count() << '\n';
}
