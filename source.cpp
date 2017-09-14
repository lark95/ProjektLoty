#include <iostream>
#include "Analize.h"
#include <string>
#include <fstream>

using namespace std;

void WypiszOpcje()
{
	cout <<
		"1)Wczytaj dane\n" <<
		"2)Roznica czasu startow dla wybranego samolotu\n" <<
		"3)Roznica czasu przylotow dla wybranego samolotu\n" <<
		"4)Roznica dlugosci trwania lotu dla wybranego samolotu\n" <<
		"5)Roznica dlugosci trwania lotu dla wybranej linii lotniczej\n" <<
		"6)Roznica czasu startow dla wybranej linii lotniczej \n" <<
		"7)Roznica czasu przylotow dla wybranej linii lotniczej \n" <<
		"8)Najwieksza z odleglosci od punktu, w ktorym powinien znajdowac sie samolot \n" <<
		"0)Wyjscie z programu.\n\n";
}

void main()
{
	Analize Temp;
	string nazwa_pliku, string_2;
	int b;
	
	cout.width(50);
	cout << "Witamy !!!\n\n" <<
		" Program PR porownujacy rzeczywiste i planowane trasy samolotow\n\n" <<
		"1. Wczytaj dane\n\n" <<endl;

	try //Lapanie wyjatkow. Jesli ktorakolwiek funkcja rzuci blad, dzieki bloku try - catch mozemy dowiedziec sie o typie bledu.
	{
		int x = -1, s;
		bool bufor;
		while (x != 0)
		{
			s = 0;
			bufor = false;
			cout << "Wybierz opcje:\n";
			cin >> x;
			switch (x)
			{
			case 1:
				cout << "Podaj nazwe pliku z danymi planowanymi z rozszerzeniem" << endl;
				cin >> nazwa_pliku;
				while (!Temp.CheckFile(nazwa_pliku))
				{
					cout << "Nie ma pliku o takiej nazwie" << endl;
					cout << "Podaj nazwe pliku z danymi planowanymi z rozszerzeniem" << endl;
					cin >> nazwa_pliku;
				}
				Temp.LoadPlaningData(nazwa_pliku);
				cout << "Podaj nazwe pliku z danymi rzeczywistymi z rozszerzeniem" << endl;
				cin >> nazwa_pliku;
				while (!Temp.CheckFile(nazwa_pliku))
				{
					cout << "Nie ma pliku o takiej nazwie" << endl;
					cout << "Podaj nazwe pliku z danymi rzeczywistymi z rozszerzeniem" << endl;
					cin >> nazwa_pliku;
				}
				Temp.LoadRealData(nazwa_pliku);
				
				system("CLS");

				WypiszOpcje();
				break;
			case 2:
				cout << "Podaj unikalny identyfikator lotu  (uid)" << endl;
				cin >> b;
				
				
				Temp.FindUid(b);
				
				cout << "Podaj nazwe pliku wraz z rozszerzeniem do ktorego zostana zapisane informacje" << endl;
				cin >> nazwa_pliku;
				Temp.StartRemainderForFlight(b, nazwa_pliku);

				system("CLS");

				WypiszOpcje();
				break;
			case 3:
				cout << "Podaj unikalny identyfikator lotu  (uid)" << endl;
				cin >> b;
				
				//while (Temp.UidCheck(b) != true)
				//{
				//	std::cout << "Brak UID w pliku" << std::endl;
				//	std::cout << "Podaj nowe UID" << std::endl;
				//	std::cin >> b;
				//}
				Temp.FindUid(b);

				cout << "Podaj nazwe pliku wraz z rozszerzeniem do ktorego zostana zapisane informacje" << endl;
				cin >> nazwa_pliku;
				Temp.ArrRemainderForFlight(b, nazwa_pliku);

				system("CLS");

				WypiszOpcje();
				break;
			case 4:
				cout << "Podaj unikalny identyfikator lotu  (uid)" << endl;
				cin >> b;
				
				
				Temp.FindUid(b);

				cout << "Podaj nazwe pliku wraz z rozszerzeniem do ktorego zostana zapisane informacje" << endl;
				cin >> nazwa_pliku;
				Temp.LengthComparisonForFlight(b, nazwa_pliku);

				system("CLS");

				WypiszOpcje();
				break;
			case 5:
				cout << "Podaj nazwe lini lotniczej" << endl;
				cin >> string_2;
				
				//while (Temp.LineCheck(string_2) != true)
				//{
				//	cout << "Brak lini lotniczej" << std::endl;
				//	cout << "Podaj nowa linie lotnicza" << std::endl;
				//	cin >> string_2;
				//}
				Temp.FindLine(string_2);
				cout << "Podaj nazwe pliku wraz z rozszerzeniem do ktorego zostana zapisane informacje" << endl;
				cin >> nazwa_pliku;
				Temp.LengthComparisonForAirlines(string_2, nazwa_pliku);

				system("CLS");

				WypiszOpcje();
				break;
			case 6:
				cout << "Podaj nazwe lini lotniczej" << endl;
				cin >> string_2;
				
				//while (Temp.LineCheck(string_2) != true)
				//{
				//	cout << "Brak lini lotniczej" << std::endl;
				//	cout << "Podaj nowa linie lotnicza" << std::endl;
				//	cin >> string_2;
				//}
				Temp.FindLine(string_2);
				cout << "Podaj nazwe pliku wraz z rozszerzeniem do ktorego zostana zapisane informacje" << endl;
				cin >> nazwa_pliku;
				Temp.StartRemainderForAirlines(string_2, nazwa_pliku);

				system("CLS");

				WypiszOpcje();
				break;
			case 7:
				cout << "Podaj nazwe lini lotniczej" << endl;
				cin >> string_2;
				
				//while (Temp.LineCheck(string_2) != true)
				//{
				//	cout << "Brak lini lotniczej" << std::endl;
				//	cout << "Podaj nowa linie lotnicza" << std::endl;
				//	cin >> string_2;
				//}
				Temp.FindLine(string_2);

				cout << "Podaj nazwe pliku wraz z rozszerzeniem do ktorego zostana zapisane informacje" << endl;
				cin >> nazwa_pliku;
				Temp.ArrRemainderForAirlines(string_2, nazwa_pliku);

				system("CLS");

				WypiszOpcje();
				break;
			case 8:
				cout << "Podaj unikalny identyfikator lotu  (uid)" << endl;
				cin >> b;
				
				//while (Temp.UidCheck(b) != true)
				//{
				//	std::cout << "Brak UID w pliku" << std::endl;
				//	std::cout << "Podaj nowe UID" << std::endl;
				//	std::cin >> b;
				//}
				Temp.FindUid(b);


				cout << "Podaj nazwe pliku wraz z rozszerzeniem do ktorego zostana zapisane informacje" << endl;
				cin >> nazwa_pliku;
				Temp.GreatestDistanceDifference(b, nazwa_pliku);

				system("CLS");

				WypiszOpcje();
				break;

			case 0:
				break;
			default:
				cout << "Wybrales opcje spoza zakresu.";
				break;

			}
		}
	}
	catch (std::out_of_range &ex)
	{
		std::cout << ex.what() << std::endl;
	}
	catch (std::exception &ex)
	{
		std::cout << ex.what() << std::endl;
	}
	catch (std::ios_base::failure &fail)
	{
		std::cout << "1" << std::endl;
	}
	catch (...)
	{
		std::cout << "BUG HAPPENED!" << std::endl;
	}
	system("PAUSE");
	return;
}
