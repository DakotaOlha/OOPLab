// Лабораторна робота №5, третій рівень, варіант 14 «Система Платежі».
// ЛР5 п.16: заголовний файл класу Administrator.

#ifndef ADMINISTRATOR_H_
#define ADMINISTRATOR_H_

#include <string>
#include <vector>

class Client;

// Агрегація — адміністратор зберігає покажчики на клієнтів, але
// не володіє ними: клієнти існують до і після адміністратора.
class Administrator {
 public:
  explicit Administrator(const std::string& name);
  ~Administrator();

  void AddClient(Client* client);
  void CheckAllCards();
  bool ReviewLimitRequest(Client* client, double new_limit);
  void PrintClients() const;

 private:
  std::string name_;
  std::vector<Client*> clients_;
};

#endif  // ADMINISTRATOR_H_
