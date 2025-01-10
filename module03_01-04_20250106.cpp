#include <iostream>
#include <string>
#include <vector>

std::vector<std::string> buildRoute() {
  int stops = 0;
  std::cout << "Введите количество остановок: ";
  std::cin >> stops;
  std::vector<std::string> route(stops, "");
  
  std::cout << "Введите наименования остановок:\n";
  for(int i = 0; i < stops; ++i) {
    std::cout << i + 1 << "-я остановка: ";
    std::cin >> route[i];
  } 
  std::cout << "Маршрут сформирован.\n";
  return route; 
}

double routeRevenue(const std::vector<std::string>& route, const double price) {
  auto stops = route.size();
  int currentPassengers = 0;
  double revenue = 0;
  for(const std::string& stop : route) {
    int passengers = 0;
    --stops;
    
    std::cout << "Прибываем на остановку " << stop << ".\n";
    std::cout << "В салоне пассажиров: " << currentPassengers << "\n";

    if(stops > 0) {
      std::cout << "Сколько пассажиров вышло на остановке? ";
      std::cin >> passengers;
      currentPassengers = currentPassengers > passengers ? currentPassengers - passengers : 0;
      std::cout << "Сколько пассажиров вошло на остановке? ";
      std::cin >> passengers;
      currentPassengers += passengers;
      revenue += passengers * price;

      std::cout << "Отправляемся с остановки " << stop << ".\n";
      std::cout << "В салоне пассажиров: " << currentPassengers << "\n";

      std::cout << "-------Едем-------\n";
    } else {
      std::cout << "-----Конечная-----\n";
    }
  }
  return revenue;
}

template<typename T>
void swap(T& a, T& b) {
  T buffer = a;
  a = b;
  b = buffer;
}

template<typename T>
void swapNumbers(T& a, T& b) {
  a += b;
  b = a - b;
  a -= b;
}

double bambooHeight(double days, double dayGrowth, double nightFade, double beginHeight) {
  // 1 day = 24h
  // night = 12h = 0.5 day
  // daytime = 12h = 0.5 day
  // day begins with daytime. 
  // Therefore 2 days and 0.5 of daytime is 2.25 days.
  
  double height = beginHeight;
  int intDays = days;
  double decDay = days - intDays;

  height += intDays * (dayGrowth - nightFade);

  if (decDay > 0.5) {
    height += dayGrowth;
    height -= (decDay - 0.5) * nightFade * 2;
  } else {
    height += decDay * dayGrowth * 2;
  }
  
  return height;
}

int main() {
  std::cout << "Задание 1. Расчет прибыли маршрута.\n";
  std::vector<std::string> route = buildRoute();

  double revenue = routeRevenue(route, 20);
  double driverSalary = revenue / 4;
  double fuelCost = revenue / 5;
  double taxes = fuelCost;
  double repairCar = fuelCost;
  double net = revenue - driverSalary - fuelCost - taxes - repairCar;

  std::cout << "Выручка: " << revenue << " руб.\n";
  std::cout << "Зарплата водителя: " << driverSalary << " руб.\n";
  std::cout << "Расходы на топливо: " << fuelCost << " руб.\n";
  std::cout << "Налоги: " << taxes << " руб.\n";
  std::cout << "Расходы на ремонт машины: " << repairCar << " руб.\n";
  std::cout << "Итого чистая прибыль: " << net << " руб.\n";

  std::cout << "\nЗадание 2. Реализация функции обмена данными (swap())\n";
  int a = 43;
  int b = 155;
  std::cout << "a = " << a << ", b = " << b << "\n";
  swap(a, b);
  std::cout << "Поменяли местами:\n";
  std::cout << "a = " << a << ", b = " << b << "\n";

  std::cout << "\nЗадание 3. Высота бамбука.\n";
  std::cout << "Сутки начинаются с дня и складываются\n"
    << "из дня (12 ч) и ночи (12 ч).\n"
    << "Следовательно на 2 суток и половина дня соответствует 2.25 суток.\n";
  double height = bambooHeight(2.25, 50, 20, 100);
  std::cout << "Высота бамбука через " << 2.25 << " суток : " << height << " см.\n";

  std::cout << "\nЗадание 4. Реализация обмена данными без дополнительной памяти.\n";
  std::cout << "a = " << a << ", b = " << b << "\n";
  swapNumbers(a, b);
  std::cout << "Поменяли местами:\n";
  std::cout << "a = " << a << ", b = " << b << "\n";
  return 0;
}
