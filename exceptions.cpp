// Лабораторна робота №5, третій рівень, варіант 14 «Система Платежі».
// ЛР5 п.16: реалізація власних класів винятків.

#include "exceptions.h"

MemoryError::MemoryError(const std::string& message)
    : std::runtime_error("Помилка пам'яті: " + message) {}

FileError::FileError(const std::string& message)
    : std::runtime_error("Помилка файлу: " + message) {}

NotFoundError::NotFoundError(const std::string& message)
    : std::runtime_error("Не знайдено: " + message) {}
