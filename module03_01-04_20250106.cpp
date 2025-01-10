#include <iostream>
#include <string>
#include <vector>

std::vector<std::string> buildRoute() {
  int stops = 0;
  std::cout << "������ ������⢮ ��⠭����: ";
  std::cin >> stops;
  std::vector<std::string> route(stops, "");
  
  std::cout << "������ ������������ ��⠭����:\n";
  for(int i = 0; i < stops; ++i) {
    std::cout << i + 1 << "-� ��⠭����: ";
    std::cin >> route[i];
  } 
  std::cout << "������� ��ନ஢��.\n";
  return route; 
}

double routeRevenue(const std::vector<std::string>& route, const double price) {
  auto stops = route.size();
  int currentPassengers = 0;
  double revenue = 0;
  for(const std::string& stop : route) {
    int passengers = 0;
    --stops;
    
    std::cout << "�ਡ뢠�� �� ��⠭���� " << stop << ".\n";
    std::cout << "� ᠫ��� ���ᠦ�஢: " << currentPassengers << "\n";

    if(stops > 0) {
      std::cout << "����쪮 ���ᠦ�஢ ��諮 �� ��⠭����? ";
      std::cin >> passengers;
      currentPassengers = currentPassengers > passengers ? currentPassengers - passengers : 0;
      std::cout << "����쪮 ���ᠦ�஢ ��諮 �� ��⠭����? ";
      std::cin >> passengers;
      currentPassengers += passengers;
      revenue += passengers * price;

      std::cout << "��ࠢ�塞�� � ��⠭���� " << stop << ".\n";
      std::cout << "� ᠫ��� ���ᠦ�஢: " << currentPassengers << "\n";

      std::cout << "-------����-------\n";
    } else {
      std::cout << "-----����筠�-----\n";
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
  std::cout << "������� 1. ����� �ਡ뫨 �������.\n";
  std::vector<std::string> route = buildRoute();

  double revenue = routeRevenue(route, 20);
  double driverSalary = revenue / 4;
  double fuelCost = revenue / 5;
  double taxes = fuelCost;
  double repairCar = fuelCost;
  double net = revenue - driverSalary - fuelCost - taxes - repairCar;

  std::cout << "����窠: " << revenue << " ��.\n";
  std::cout << "��௫�� ����⥫�: " << driverSalary << " ��.\n";
  std::cout << "���室� �� ⮯����: " << fuelCost << " ��.\n";
  std::cout << "������: " << taxes << " ��.\n";
  std::cout << "���室� �� ६��� ��設�: " << repairCar << " ��.\n";
  std::cout << "�⮣� ���� �ਡ��: " << net << " ��.\n";

  std::cout << "\n������� 2. ��������� �㭪樨 ������ ����묨 (swap())\n";
  int a = 43;
  int b = 155;
  std::cout << "a = " << a << ", b = " << b << "\n";
  swap(a, b);
  std::cout << "�����﫨 ���⠬�:\n";
  std::cout << "a = " << a << ", b = " << b << "\n";

  std::cout << "\n������� 3. ���� ����㪠.\n";
  std::cout << "��⪨ ��稭����� � ��� � ᪫��뢠����\n"
    << "�� ��� (12 �) � ��� (12 �).\n"
    << "�������⥫쭮 �� 2 ��⮪ � �������� ��� ᮮ⢥����� 2.25 ��⮪.\n";
  double height = bambooHeight(2.25, 50, 20, 100);
  std::cout << "���� ����㪠 �१ " << 2.25 << " ��⮪ : " << height << " �.\n";

  std::cout << "\n������� 4. ��������� ������ ����묨 ��� �������⥫쭮� �����.\n";
  std::cout << "a = " << a << ", b = " << b << "\n";
  swapNumbers(a, b);
  std::cout << "�����﫨 ���⠬�:\n";
  std::cout << "a = " << a << ", b = " << b << "\n";
  return 0;
}
