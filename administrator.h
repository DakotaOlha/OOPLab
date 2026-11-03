// Лабораторна робота №3, варіант 14 «Система Платежі».
// п.17: заголовний файл класу Administrator.
//
// п.12: агрегація — адміністратор зберігає покажчики на клієнтів, але НЕ
// володіє ними: клієнти існують до і після адміністратора.

#ifndef ADMINISTRATOR_H_
#define ADMINISTRATOR_H_

#include <string>
#include <vector>

class Client;

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

#endif
