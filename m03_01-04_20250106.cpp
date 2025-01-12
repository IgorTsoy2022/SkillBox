#include <iostream>
#include <string>
#include <vector>

std::vector<std::string> buildRoute() {
  int stops = 0;
  std::cout << "‚¢¥¤¨â¥ ª®«¨ç¥áâ¢® ®áâ ­®¢®ª: ";
  std::cin >> stops;
  std::vector<std::string> route(stops, "");
  
  std::cout << "‚¢¥¤¨â¥ ­ ¨¬¥­®¢ ­¨ï ®áâ ­®¢®ª:\n";
  for(int i = 0; i < stops; ++i) {
    std::cout << i + 1 << "-ï ®áâ ­®¢ª : ";
    std::cin >> route[i];
  } 
  std::cout << "Œ àèàãâ áä®à¬¨à®¢ ­.\n";
  return route; 
}

double routeRevenue(const std::vector<std::string>& route, const double price) {
  auto stops = route.size();
  int currentPassengers = 0;
  double revenue = 0;
  for(const std::string& stop : route) {
    int passengers = 0;
    --stops;
    
    std::cout << "à¨¡ë¢ ¥¬ ­  ®áâ ­®¢ªã " << stop << ".\n";
    std::cout << "‚ á «®­¥ ¯ áá ¦¨à®¢: " << currentPassengers << "\n";

    if(stops > 0) {
      std::cout << "‘ª®«ìª® ¯ áá ¦¨à®¢ ¢ëè«® ­  ®áâ ­®¢ª¥? ";
      std::cin >> passengers;
      currentPassengers = currentPassengers > passengers ? currentPassengers - passengers : 0;
      std::cout << "‘ª®«ìª® ¯ áá ¦¨à®¢ ¢®è«® ­  ®áâ ­®¢ª¥? ";
      std::cin >> passengers;
      currentPassengers += passengers;
      revenue += passengers * price;

      std::cout << "Žâ¯à ¢«ï¥¬áï á ®áâ ­®¢ª¨ " << stop << ".\n";
      std::cout << "‚ á «®­¥ ¯ áá ¦¨à®¢: " << currentPassengers << "\n";

      std::cout << "-------…¤¥¬-------\n";
    } else {
      std::cout << "-----Š®­¥ç­ ï-----\n";
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
  std::cout << "‡ ¤ ­¨¥ 1.  áç¥â ¯à¨¡ë«¨ ¬ àèàãâ .\n";
  std::vector<std::string> route = buildRoute();

  double revenue = routeRevenue(route, 20);
  double driverSalary = revenue / 4;
  double fuelCost = revenue / 5;
  double taxes = fuelCost;
  double repairCar = fuelCost;
  double net = revenue - driverSalary - fuelCost - taxes - repairCar;

  std::cout << "‚ëàãçª : " << revenue << " àã¡.\n";
  std::cout << "‡ à¯« â  ¢®¤¨â¥«ï: " << driverSalary << " àã¡.\n";
  std::cout << " áå®¤ë ­  â®¯«¨¢®: " << fuelCost << " àã¡.\n";
  std::cout << " «®£¨: " << taxes << " àã¡.\n";
  std::cout << " áå®¤ë ­  à¥¬®­â ¬ è¨­ë: " << repairCar << " àã¡.\n";
  std::cout << "ˆâ®£® ç¨áâ ï ¯à¨¡ë«ì: " << net << " àã¡.\n";

  std::cout << "\n‡ ¤ ­¨¥ 2. ¥ «¨§ æ¨ï äã­ªæ¨¨ ®¡¬¥­  ¤ ­­ë¬¨ (swap())\n";
  int a = 43;
  int b = 155;
  std::cout << "a = " << a << ", b = " << b << "\n";
  swap(a, b);
  std::cout << "®¬¥­ï«¨ ¬¥áâ ¬¨:\n";
  std::cout << "a = " << a << ", b = " << b << "\n";

  std::cout << "\n‡ ¤ ­¨¥ 3. ‚ëá®â  ¡ ¬¡ãª .\n";
  std::cout << "‘ãâª¨ ­ ç¨­ îâáï á ¤­ï ¨ áª« ¤ë¢ îâáï\n"
    << "¨§ ¤­ï (12 ç) ¨ ­®ç¨ (12 ç).\n"
    << "‘«¥¤®¢ â¥«ì­® ­  2 áãâ®ª ¨ ¯®«®¢¨­  ¤­ï á®®â¢¥âáâ¢ã¥â 2.25 áãâ®ª.\n";
  double height = bambooHeight(2.25, 50, 20, 100);
  std::cout << "‚ëá®â  ¡ ¬¡ãª  ç¥à¥§ " << 2.25 << " áãâ®ª : " << height << " á¬.\n";

  std::cout << "\n‡ ¤ ­¨¥ 4. ¥ «¨§ æ¨ï ®¡¬¥­  ¤ ­­ë¬¨ ¡¥§ ¤®¯®«­¨â¥«ì­®© ¯ ¬ïâ¨.\n";
  std::cout << "a = " << a << ", b = " << b << "\n";
  swapNumbers(a, b);
  std::cout << "®¬¥­ï«¨ ¬¥áâ ¬¨:\n";
  std::cout << "a = " << a << ", b = " << b << "\n";
  return 0;
}
