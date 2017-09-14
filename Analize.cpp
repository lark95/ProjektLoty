#include "Analize.h"
#include <fstream>
#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <algorithm>
#include <math.h>
#include <iomanip>

#include <cassert>

Analize::Analize()
{
}


Analize::~Analize()
{
}

// FUNKCJE WCZYTUJACE DANE Z PLIKOW

void Analize::LoadPlaningData(std::string nazwa_pliku)
{
	PlaningFlightData.resize(1);
	
	std::ifstream loadFile(nazwa_pliku);
	if (!loadFile.good())
		throw "File not found";
	
	std::cout << "Wczytuje plik..." << std::endl;
	getline(loadFile, nazwa_pliku);
	getline(loadFile, nazwa_pliku);
	
	std::string data;													// z. do przechowywania linii z pliku tekstowego
	while (!loadFile.eof())
	{
		getline(loadFile, data);
		
		if (data.empty() || data.size() == 1)
		{
			std::cout << "Pusta linia napotkana. Koniec pliku. Koncze wczytywac dane.\n";
			break;
		}

		//Komentarze ulatwiajace debugowanie w przypadku bledow typu std::out_of_range
		//std::cout << "Current data: " << data << "\n";
		//std::cout << "Is data empty: " << data.empty() << "\n"; //Jesli string nie jest pusty, a linijka wyzej nic nie wypisuje, to znaczy ze moze to byc tzn. "bialy znak" (whitespace).
		//std::cout << "Data size: " << data.size() << "\n"; //Warto sprawdzic jak dlugi jest ciag znakow (ktore moga byc biale, czyli niewidoczne).
		//char is_whitespace = data[0];
		//std::cout << "Is space: " << isspace(is_whitespace) << "\n"; //Jesli ta funkcja zwroci 0 oznacza to ze dany znak nie jest znakiem bialym. Jesli funkcja zwroci wartosc != 0, oznacza to ze znak jest znakiem bialym.
		
		FlightData TempFlightData = LoadPlaningFlights(data);			// w. funkcji tworzacej obiekt danego lotu
		for (;;)
		{
			getline(loadFile, data);
			if (data != ">end ") 
			{
				CheckPoint TempCheckPoint = LoadPlaningCheckPoint(data); // w. funkcji tworzacej obiekt danych lotu
				TempFlightData.data.push_back(TempCheckPoint);
			}
			else
			{
				break;
			}
		}
		PlaningFlightData.push_back(TempFlightData);
	}
	std::cout << "Plik pomyslnie wczytany" << std::endl;
	loadFile.close();
}

void Analize::LoadRealData(std::string _v)
{
	RealFlightData.resize(1);

	std::string data;													// z. do przechowywania linii z pliku tekstowego
	std::ifstream loadFile(_v);
	if (!loadFile.good())
		throw "File not found";
	std::cout << "Wczytuje plik..." << std::endl;
	getline(loadFile, _v);
	getline(loadFile, _v);

	while (!loadFile.eof())
	{
		getline(loadFile, data);
		
		if (data.empty() || data.size() == 1)
		{
			std::cout << "Pusta linia napotkana. Koniec pliku. Koncze wczytywac dane.\n";
			break;
		}

		FlightData TempFlightData = LoadRealFlights(data);				// w. funkcji tworzacej obiekt danego lotu
		for (;;)
		{
			getline(loadFile, data);
			if (data != ">end ")
			{
				CheckPoint TempCheckPoint = LoadRealCheckPoint(data);	// w. funkcji tworzacej obiekt danych lotu
				TempFlightData.data.push_back(TempCheckPoint);
			}
			else break;
		}
		if (TempFlightData.Rtstart.hour == 50)
		{
			TempFlightData.Rtstart.hour = TempFlightData.data[0].raportTime.hour;
			TempFlightData.Rtstart.minute = TempFlightData.data[0].raportTime.minute;
			TempFlightData.Rtstart.second = TempFlightData.data[0].raportTime.second;
			RealFlightData.push_back(TempFlightData);
		}
		else
			RealFlightData.push_back(TempFlightData);
	}
	std::cout << "Plik pomyslnie wczytany" << std::endl;
	loadFile.close();
}

// FUNKCJE TWORZACE OBIEKTY LOTOW

FlightData Analize::LoadRealFlights(std::string stringBufor)
{
	// TWORZENIE ZMIENNYCH

	std::string airline_;												// z. przechowujaca dane o linii lotniczej	 
	std::string uid_;													// z. przechowujaca unikalny identyfikator lotu	
	std::string date_;													// z. przechowujaca date lotu	
	std::string tdep_;													// z. przechowujaca czas startu lotu	
	std::string dep_;													// z. przechowujaca nazwe lotniska startowego	
	std::string arr_;													// z. przechowujaca nazwe lotniska docelowego	
	std::string tarr_;													// z. przechowujaca czas ladowania	
	std::string acn_;													// z. przechowujaca nazwe samolotu	
	std::string hourA_;													// z. przechowujaca godzine przylotu	 
	std::string minuteA_;												// z. przechowujaca minute przylotu	
	std::string secondA_;												// z. przechowujaca sekunde przylotu	
	std::string hourS_;													// z. przechowujaca godzine startu	
	std::string minuteS_;												// z. przechowujaca minute startu	
	std::string secondS_;												// z. przechowujaca sekunde startu	
	std::string year_;													// z. przechowujaca rok lotu	
	std::string month_;													// z. przechowujaca miesiac lotu
	std::string day_;													// z. przechowujaca dzien lotu

	unsigned int intBufor, intBufor2, RhourS, RminuteS, RsecondS;			// kontenery do konwersji
	unsigned int RhourA, RminuteA, RsecondA, year, month, day, uid;		// kontenery do konwersji

	// SZUKANIE POTRZEBNYCH INFORMACJI

	intBufor = stringBufor.find(" ");
	airline_.insert(0, stringBufor, 0, intBufor);

	intBufor = stringBufor.find("uid=");
	intBufor2 = stringBufor.find(" ", intBufor);
	uid_.insert(0, stringBufor, intBufor + 4, intBufor2 - (intBufor + 4));

	intBufor = stringBufor.find("date=");
	intBufor2 = stringBufor.find(" ", intBufor);
	date_.insert(0, stringBufor, intBufor + 5, intBufor2 - (intBufor + 5));

	year_.insert(0, date_, 0, 4);
	month_.insert(0, date_, 5, 2);
	day_.insert(0, date_, 8, 2);

	/*intBufor = stringBufor.find("tdep=");

	if (intBufor == std::string::npos)
	{
		tdep_ = "50:50:50";
	}
	else
	{
		intBufor2 = stringBufor.find(" ", intBufor);
		tdep_.insert(0, stringBufor, intBufor + 5, intBufor2 - (intBufor + 5));
	}

	if (tdep_[0] == '+' || tdep_[0] == '#' || tdep_[0] == '-')
	{
		hourS_.insert(0, tdep_, 1, 2);
		minuteS_.insert(0, tdep_, 4, 2);
		secondS_.insert(0, tdep_, 7, 2);
	}
	else
	{
		hourS_.insert(0, tdep_, 0, 2);
		minuteS_.insert(0, tdep_, 3, 2);
		secondS_.insert(0, tdep_, 6, 2);
	} */

	intBufor = stringBufor.find("tdep=");
	intBufor2 = stringBufor.find(" ", intBufor);
	tdep_.insert(0, stringBufor, intBufor + 5, intBufor2 - (intBufor + 5));
	if (tdep_[0] == '+' || tdep_[0] == '#' || tdep_[0] == '-')
	{
		hourS_.insert(0, tdep_, 1, 2);
		minuteS_.insert(0, tdep_, 4, 2);
		secondS_.insert(0, tdep_, 7, 2);
	}
	else
	{
		hourS_.insert(0, tdep_, 0, 2);
		minuteS_.insert(0, tdep_, 3, 2);
		secondS_.insert(0, tdep_, 6, 2);
	}

	intBufor = stringBufor.find("dep=");
	intBufor2 = stringBufor.find(" ", intBufor);
	dep_.insert(0, stringBufor, intBufor + 4, intBufor2 - (intBufor + 4));

	intBufor = stringBufor.find("arr=");
	intBufor2 = stringBufor.find(" ", intBufor);
	arr_.insert(0, stringBufor, intBufor + 4, intBufor2 - (intBufor + 4));

	intBufor = stringBufor.find("tarr=");

	if (intBufor == std::string::npos)
	{
		intBufor = stringBufor.find("ts=");
		intBufor2 = stringBufor.find(" ", intBufor);
		tarr_.insert(0, stringBufor, intBufor + 3, intBufor2 - (intBufor + 3));
	}
	else
	{
		intBufor2 = stringBufor.find(" ", intBufor);
		tarr_.insert(0, stringBufor, intBufor + 5, intBufor2 - (intBufor + 5));
	}

	if (tarr_[0] == '+' || tarr_[0] == '#' || tarr_[0] == '-')
	{
		hourA_.insert(0, tarr_, 1, 2);
		minuteA_.insert(0, tarr_, 4, 2);
		secondA_.insert(0, tarr_, 7, 2);
	}
	else
	{
		hourA_.insert(0, tarr_, 0, 2);
		minuteA_.insert(0, tarr_, 3, 2);
		secondA_.insert(0, tarr_, 6, 2);
	}

	intBufor = stringBufor.find("acn=");
	intBufor2 = stringBufor.find(" ", intBufor);
	acn_.insert(0, stringBufor, intBufor + 4, intBufor2 - (intBufor + 4));

	// KONWERSJA STRINGOW NA INTY

	uid = std::stoi(uid_);

	year = std::stoi(year_);
	month = std::stoi(month_);
	day = std::stoi(day_);

	RhourA = std::stoi(hourA_);
	RminuteA = std::stoi(minuteA_);
	RsecondA = std::stoi(secondA_);


	RhourS = std::stoi(hourS_);
	RminuteS = std::stoi(minuteS_);
	RsecondS = std::stoi(secondS_);


	// TWORZENIE OBIEKTOW STRUKTUR I INICJALIZACJA

	//#############
	
	Rtarr.hour = RhourA;
	Rtarr.minute = RminuteA;
	Rtarr.second = RsecondA;
	//###########
	
	Rtstart.hour = RhourS;
	Rtstart.minute = RminuteS;
	Rtstart.second = RsecondS;

	uid__.uid = uid;
	
	airline__.airline = airline_;
	//std::cout << airline__.airline << std::endl;
	//system("PAUSE");

	Date date;
	date.year = year;
	date.month = month;
	date.day = day;

	// TWORZENIE OBIEKTU LOTU I DO KONTENERA

	FlightData bufor(airline_, uid, date, dep_, tdep, arr_ , tarr, acn_ );
	return bufor;
}

CheckPoint Analize::LoadRealCheckPoint(std::string stringBufor)
{
	// TWORZENIE ZMIENNYCH

	std::string degreesNS_;												// z. przechowujaca stopnie geograficzne NS	 
	std::string minuteS_NS;												// z. przechowujaca minuty geograficzne NS	
	std::string degreesEW_;												// z. przechowujaca stopnie geograficzne EW	
	std::string minuteS_EW;												// z. przechowujaca minuty geograficzne EW	
	std::string hourT_;													// z. przechowujaca godzine samolotu nad danym punktem	
	std::string minuteT_;												// z. przechowujaca minute samolotu nad danym punktem	
	std::string secondT_;												// z. przechowujaca sekunde samolotu nad danym punktem	
	std::string actualHigh_;											// z. przechowujaca aktualna wysokosc	
	std::string nextHigh_;												// z. przechowujaca wysokosc na ktorej bedzie samolot	 
	std::string positionNS_;											// z. przechowujaca symbol polkuli NS	
	std::string positionEW_;											// z. przechowujaca symbol polkuli EW
	std::string  vr_;													// z. przechowujaca wspolczynnik wznoszenia
	
	unsigned short RdegreesNS, RminutesNS, RdegreesEW, RminutesEW;			// kontenery do konwersji
	unsigned short RhourT, RminuteT, RsecondT, RactualHigh, RnextHigh, Rvr;	// kontenery do konwersji

	int intBufor, intBufor2;											// kontenery

	// SZUKANIE POTRZEBNYCH INFORMACJI

	degreesNS_.insert(0, stringBufor, 1, 2);
	positionNS_.insert(0, stringBufor, 3, 1);
	minuteS_NS.insert(0, stringBufor, 4, 4);
	degreesEW_.insert(0, stringBufor, 9, 3);
	positionEW_.insert(0, stringBufor, 12, 1);
	minuteS_EW.insert(0, stringBufor, 13, 4);
	hourT_.insert(0, stringBufor, 19, 2);
	minuteT_.insert(0, stringBufor, 22, 2);
	secondT_.insert(0, stringBufor, 25, 2);
	actualHigh_.insert(0, stringBufor, 28, 3);
	nextHigh_.insert(0, stringBufor, 32, 3);

	intBufor = stringBufor.find("vr=");

	if (intBufor == std::string::npos)
	{
		vr_ = "0";
	}
	else
	{
		intBufor2 = stringBufor.find(" ", intBufor);
		vr_.insert(0, stringBufor, intBufor + 3, intBufor2 - (intBufor + 3));
	}

	// KONWERSJA STRINGOW NA ODPOWIEDNIE FORMATY DANYCH

	RdegreesNS = std::stoi(degreesNS_);
	RminutesNS = std::stoi(minuteS_NS);

	RdegreesEW = std::stoi(degreesEW_);
	RminutesEW = std::stoi(minuteS_EW);

	RhourT = std::stoi(hourT_);
	RminuteT = std::stoi(minuteT_);
	RsecondT = std::stoi(secondT_);
	Rvr = std::stoi(vr_);

	if (actualHigh_[2] != ' ')
	{
		RactualHigh = std::stoi(actualHigh_);
	}
	else
		RactualHigh = 0;

	if (nextHigh_[2] != ' ')
	{
		RnextHigh = std::stoi(nextHigh_);
	}
	else
		RnextHigh = 0;

	// TWORZENIE OBIEKTOW STRUKTUR I INICJALIZACJA

	RpositionPointNS.degrees = RdegreesNS;
	RpositionPointNS.position = positionNS_;
	RpositionPointNS.minutes = RminutesNS;

	Coordinates positionPointsEW;
	positionPointsEW.degrees = RdegreesEW;
	positionPointsEW.position = positionEW_;
	positionPointsEW.minutes = RminutesEW;

	
	positionTimeR.hour = RhourT;
	positionTimeR.minute = RminuteT;
	positionTimeR.second = RsecondT;

	CheckPoint checkLine;
	checkLine.NS = RpositionPointNS;
	checkLine.EW = positionPointsEW;
	checkLine.raportTime = positionTimeR;
	checkLine.actualHigh = RactualHigh;
	checkLine.nextHigh = RnextHigh;
	checkLine.vr = Rvr;

	return checkLine;
}

FlightData Analize::LoadPlaningFlights(std::string stringBufor)
{
	// TWORZENIE ZMIENNYCH

	std::string _airline;												// z. przechowujaca dane o linii lotniczej	 
	std::string _uid;													// z. przechowujaca unikalny identyfikator lotu	
	std::string _date;													// z. przechowujaca date lotu	
	std::string _ts;													// z. przechowujaca czas startu lotu
	std::string _tdep;
	std::string _dep;													// z. przechowujaca nazwe lotniska startowego	
	std::string arr_;													// z. przechowujaca nazwe lotniska docelowego	
	std::string _tarr;													// z. przechowujaca czas ladowania	
	std::string acn_;													// z. przechowujaca nazwe samolotu	
	std::string _hourA;													// z. przechowujaca godzine przylotu	 
	std::string _minuteA;												// z. przechowujaca minute przylotu	
	std::string _secondA;												// z. przechowujaca sekunde przylotu	
	std::string _hourS;													// z. przechowujaca godzine startu	
	std::string _minuteS;												// z. przechowujaca minute startu	
	std::string _secondS;												// z. przechowujaca sekunde startu	
	std::string year_;													// z. przechowujaca rok lotu	
	std::string month_;													// z. przechowujaca miesiac lotu
	std::string day_;													// z. przechowujaca dzien lotu

	unsigned int intBufor, intBufor2, PhourS, PminuteS, PsecondS;			// kontenery do konwersji
	unsigned int PhourA, PminuteA, PsecondA, year, month, day, uid;		// kontenery do konwersji

	// SZUKANIE POTRZEBNYCH INFORMACJI

	intBufor = stringBufor.find(" ");
	_airline.insert(0, stringBufor, 0, intBufor);
	
	intBufor = stringBufor.find("uid=");
	intBufor2 = stringBufor.find(" ", intBufor);
	_uid.insert(0, stringBufor, intBufor + 4, intBufor2 - (intBufor + 4));

	intBufor = stringBufor.find("date=");
	intBufor2 = stringBufor.find(" ", intBufor);
	_date.insert(0, stringBufor, intBufor + 5, intBufor2 - (intBufor + 5));

	year_.insert(0, _date, 0, 4);
	month_.insert(0, _date, 5, 2);
	day_.insert(0, _date, 8, 2);

	intBufor = stringBufor.find("ts=");
	intBufor2 = stringBufor.find(" ", intBufor);
	_ts.insert(0, stringBufor, intBufor + 3, intBufor2 - (intBufor + 3));

	/*if (_ts[0] == '+' || _ts[0] == '#' || _ts[0] == '-')
	{
		_hourS.insert(0, _ts, 1, 2);
		_minuteS.insert(0, _ts, 4, 2);
		_secondS.insert(0, _ts, 7, 2);
	}
	else
	{
		_hourS.insert(0, _ts, 0, 2);
		_minuteS.insert(0, _ts, 3, 2);
		_secondS.insert(0, _ts, 6, 2);
	}*/

	intBufor = stringBufor.find("dep=");
	intBufor2 = stringBufor.find(" ", intBufor);
	_dep.insert(0, stringBufor, intBufor + 4, intBufor2 - (intBufor + 4));
	
	intBufor = stringBufor.find("tdep=");
	intBufor2 = stringBufor.find(" ", intBufor);
	_tdep.insert(0, stringBufor, intBufor + 5, intBufor2 - (intBufor + 5));
	if (_tdep[0] == '+' || _tdep[0] == '#' || _tdep[0] == '-')
	{
		_hourS.insert(0, _tdep, 1, 2);
		_minuteS.insert(0, _tdep, 4, 2);
		_secondS.insert(0, _tdep, 7, 2);
	}
	else
	{
		_hourS.insert(0, _tdep, 0, 2);
		_minuteS.insert(0, _tdep, 3, 2);
		_secondS.insert(0, _tdep, 6, 2);
	}

	intBufor = stringBufor.find("arr=");
	intBufor2 = stringBufor.find(" ", intBufor);
	arr_.insert(0, stringBufor, intBufor + 4, intBufor2 - (intBufor + 4));

	intBufor = stringBufor.find("tarr=");
	intBufor2 = stringBufor.find(" ", intBufor);
	_tarr.insert(0, stringBufor, intBufor + 5, intBufor2 - (intBufor + 5));

	if (_tarr[0] == '+' || _tarr[0] == '#' || _tarr[0] == '-')
	{
		_hourA.insert(0, _tarr, 1, 2);
		_minuteA.insert(0, _tarr, 4, 2);
		_secondA.insert(0, _tarr, 7, 2);
	}
	else
	{
		_hourA.insert(0, _tarr, 0, 2);
		_minuteA.insert(0, _tarr, 3, 2);
		_secondA.insert(0, _tarr, 6, 2);
	}

	intBufor = stringBufor.find("acn=");
	intBufor2 = stringBufor.find(" ", intBufor);
	acn_.insert(0, stringBufor, intBufor + 4, intBufor2 - (intBufor + 4));

	// KONWERSJA STRINGOW NA INTY

	uid = std::stoi(_uid);

	year = std::stoi(year_);
	month = std::stoi(month_);
	day = std::stoi(day_);

	PhourA = std::stoi(_hourA);
	PminuteA = std::stoi(_minuteA);
	PsecondA = std::stoi(_secondA);

	PhourS = std::stoi(_hourS);
	PminuteS = std::stoi(_minuteS);
	PsecondS = std::stoi(_secondS);

	// TWORZENIE OBIEKTOW STRUKTUR I INICJALIZACJA

	
	Ptarr.hour = PhourA;
	Ptarr.minute = PminuteA;
	Ptarr.second = PsecondA;
	//###########
	
	Ptstart.hour = PhourS;
	Ptstart.minute = PminuteS;
	Ptstart.second = PsecondS;
	//###########
	uid__.uid = uid;//wpisanie uid do obiektu
	//std::cout << uid__.uid << std::endl;
	for (int i = 0; i < UId.size(); i++)
	{
		UId.push_back(uid__.uid);
	}
	
	airline__.airline = _airline;// wpisanie lini lotniczej do obiektu
	for (int i = 0; i < AIrline.size(); i++)
	{
		AIrline.push_back(airline__.airline);
	}
	

	Date date;
	date.year = year;
	date.month = month;
	date.day = day;

	// TWORZENIE OBIEKTU LOTU I DO KONTENERA

	FlightData bufor(_airline, uid, date, _dep, tdep , arr_ ,tarr , acn_);
	return bufor;
}

CheckPoint Analize::LoadPlaningCheckPoint(std::string stringBufor)
{
	// TWORZENIE ZMIENNYCH

	std::string _degreesNS;												// z. przechowujaca stopnie geograficzne NS	 
	std::string _minuteS_NS;												// z. przechowujaca minuty geograficzne NS	
	std::string _degreesEW;												// z. przechowujaca stopnie geograficzne EW	
	std::string _minuteS_EW;												// z. przechowujaca minuty geograficzne EW	
	std::string _hourT;													// z. przechowujaca godzine samolotu nad danym punktem	
	std::string _minuteT;												// z. przechowujaca minute samolotu nad danym punktem	
	std::string _secondT;												// z. przechowujaca sekunde samolotu nad danym punktem	
	std::string _actualHigh;											// z. przechowujaca aktualna wysokosc	
	std::string _nextHigh;												// z. przechowujaca wysokosc na ktorej bedzie samolot	 
	std::string positionNS_;											// z. przechowujaca symbol polkuli NS	
	std::string positionEW_;											// z. przechowujaca symbol polkuli EW	

	unsigned short PdegreesNS, PminutesNS, PdegreesEW, PminutesEW;			// kontenery do konwersji
	unsigned short PhourT, PminuteT, PsecondT, PactualHigh, PnextHigh;		// kontenery do konwersji
	unsigned short Pvr = 0;												// z. przechowujaca wspolczynnik wznoszenia

	// SZUKANIE POTRZEBNYCH INFORMACJI

	_degreesNS.insert(0, stringBufor, 1, 2);
	positionNS_.insert(0, stringBufor, 3, 1);
	_minuteS_NS.insert(0, stringBufor, 4, 4);
	_degreesEW.insert(0, stringBufor, 9, 3);
	positionEW_.insert(0, stringBufor, 12, 1);
	_minuteS_EW.insert(0, stringBufor, 13, 4);
	_hourT.insert(0, stringBufor, 19, 2);
	_minuteT.insert(0, stringBufor, 22, 2);
	_secondT.insert(0, stringBufor, 25, 2);
	_actualHigh.insert(0, stringBufor, 28, 3);
	_nextHigh.insert(0, stringBufor, 32, 3);

	// KONWERSJA STRINGOW NA ODPOWIEDNIE FORMATY DANYCH

	PdegreesNS = std::stoi(_degreesNS);
	PminutesNS = std::stoi(_minuteS_NS);

	PdegreesEW = std::stoi(_degreesEW);
	PminutesEW = std::stoi(_minuteS_EW);

	PhourT = std::stoi(_hourT);
	PminuteT = std::stoi(_minuteT);
	PsecondT = std::stoi(_secondT);

	if (_actualHigh[2] != ' ')
	{
		PactualHigh = std::stoi(_actualHigh);
	}
	else
		PactualHigh = 0;

	if (_nextHigh[2] != ' ')
	{
		PnextHigh = std::stoi(_nextHigh);
	}
	else
		PnextHigh = 0;

	// TWORZENIE OBIEKTOW STRUKTUR I INICJALIZACJA


	PpositionPointNS.degrees = PdegreesNS;
	PpositionPointNS.position = positionNS_;
	PpositionPointNS.minutes = PminutesNS;

	Coordinates positionPointsEW;
	positionPointsEW.degrees = PdegreesEW;
	positionPointsEW.position = positionEW_;
	positionPointsEW.minutes = PminutesEW;

	
	positionTimeP.hour = PhourT;
	positionTimeP.minute = PminuteT;
	positionTimeP.second = PsecondT;

	CheckPoint checkLine;
	checkLine.NS = PpositionPointNS;
	checkLine.EW = positionPointsEW;
	checkLine.raportTime = positionTimeP;
	checkLine.actualHigh = PactualHigh;
	checkLine.nextHigh = PnextHigh;
	checkLine.vr = Pvr;

	return checkLine;
}
/*void Analize::LoadLine()
{
	std::fstream plik;
	plik.open("linie.txt", std::ios::in | std::ios::out);
	std::string asd;
	while (plik.good() == true)
	{
		plik >> asd;
		Line.push_back(asd);
	}
	plik.close();
}
void Analize::LoadUid()
{
	std::fstream plik;
	plik.open("Uid.txt", std::ios::in | std::ios::out);
	int asd;
	while (plik.good() == true)
	{
		plik >> asd;
		Uid.push_back(asd);
	}
	plik.close();
}*/
// FUNKCJE SPRAWDZAJACE POPRAWNOSC DANYCH

short Analize::CheckFile(std::string _v)
{
	std::ifstream loadFile(_v);
	if (!loadFile.good())
	{
		loadFile.close();
		return 0;
	}
	else
	{
		loadFile.close();
		return 1;
	}
}

short Analize::CheckHour(short _v)
{
	if (_v >= 0 && _v < 24)
		return 1;
	else
		return 0;
}

short Analize::CheckMinute(short _v)
{
	if (_v >= 0 && _v < 60)
		return 1;
	else
		return 0;
}

// FUNKCJE WYSZUKUJACE


unsigned int Analize::FindUid(unsigned int uid_)
{
	if (uid_ == uid__.uid)
	{
		return uid_;
		std::cout << uid__.uid << std::endl;
	}

	/*for (unsigned int ip = 0; ip < RealFlightData.size(); ip++)
	{
		
	}*/	
	std::cout << uid__.uid << std::endl;
	std::cout << "Brak podanego UID" << std::endl;
	return 0;
}
//szukanie linii lotniczej , dodane - 14.09.17
std::string Analize::FindLine(std::string airline_)
{
	if (airline_ == airline__.airline)
	{
		return airline_;
		std::cout << airline__.airline << std::endl;
	}
	std::cout << airline__.airline << std::endl;
	std::cout << "Brak podanej linii lotniczej" << std::endl;
	return 0;
}
bool Analize::UidCheck(int uid_)
{
	
	for (unsigned int i = 0; i < Uid.size(); i++)
	{
		
		if (uid_ == Uid[i])
		{
			return true;
		}
		
		
	}
}
bool Analize::LineCheck(std::string a)
{
	
	for (unsigned int i = 0; i < Line.size(); i++)
	{
		
		if (a == Line[i])
		{
			return true;
		}
		
	}


}
// FUNKCJE WYKONAWCZE PROGRAMU

//2
void Analize::StartRemainderForFlight(unsigned int _dep, std::string _v)
{
	std::ofstream saveFile(_v);
	int D1 = 0, D2 = 0, D3 = 0, D8 = 0;
	double D5 = 0, D4 = 0;


	for (unsigned int ip = 0; ip < RealFlightData.size() && ip < PlaningFlightData.size(); ip++)
	{
		if (_dep == RealFlightData[ip].uid && _dep == PlaningFlightData[ip].uid)
		{

			//roznica

			D1 = (Ptstart.hour * 3600) + (Ptstart.minute * 60) + Ptstart.second;
			D2 = (Rtstart.hour * 3600) + (Rtstart.minute * 60) + Rtstart.second;

			D3 = D2 - D1;
			D4 = D3 / 60;
			if (D4 > 60) D5 = D4 / 60;
			std::string liczba_string = std::to_string(D5);
			std::string delimeter = ".";
			std::string liczba_calkowita_string = liczba_string.substr(0, liczba_string.find(delimeter));
			int D6 = std::stoi(liczba_calkowita_string);
			std::string liczba_po_przecinku_string = liczba_string.substr(liczba_string.find(delimeter) + 1);
			liczba_po_przecinku_string = liczba_po_przecinku_string.substr(0, 2);
			int D7 = std::stoi(liczba_po_przecinku_string);
			if (D7 > 60) { D8 = D7 - 60; D6++; saveFile << D6 << " : " << D8 << std::endl; }
			else
			{
				saveFile << D6 << " : " << D7 << std::endl;
			}
		}
	}

	saveFile.close();
} // 2
//3
void Analize::ArrRemainderForFlight(unsigned int _dep,std::string _v)
{
	std::ofstream saveFile(_v);
	int D1 = 0, D2 = 0, D3 = 0, D8 = 0, D6 = 0, D7 = 0;
	double D5 = 0.0, D4 = 0.0;


	for (unsigned int ip = 0; ip < RealFlightData.size() && ip < PlaningFlightData.size(); ip++)
	{
		if (_dep == RealFlightData[ip].uid && _dep == PlaningFlightData[ip].uid)
		{

			//roznica

			D1 = (Ptarr.hour * 3600) + (Ptarr.minute * 60) + Ptarr.second;
			D2 = (Rtarr.hour * 3600) + (Rtarr.minute * 60) + Rtarr.second;

			D3 = D2 - D1;
			if (D3 < 0) D3 *= -1;
			D4 = D3 / 60.0; 
			if (D4 > 60.0)
			{
				D5 = D4 / 60.0;
				std::string liczba_string = std::to_string(D5);
				std::string delimeter = ".";
				std::string liczba_calkowita_string = liczba_string.substr(0, liczba_string.find(delimeter));
				D6 = std::stoi(liczba_calkowita_string);
				std::string liczba_po_przecinku_string = liczba_string.substr(liczba_string.find(delimeter) + 1);
				liczba_po_przecinku_string = liczba_po_przecinku_string.substr(0, 2);
				D7 = std::stoi(liczba_po_przecinku_string);
			}
			else
			{
				std::string liczba_string = std::to_string(D4);
				std::string delimeter = ".";
				std::string liczba_calkowita_string = liczba_string.substr(0, liczba_string.find(delimeter));
				D6 = std::stoi(liczba_calkowita_string);
				std::string liczba_po_przecinku_string = liczba_string.substr(liczba_string.find(delimeter) + 1);
				liczba_po_przecinku_string = liczba_po_przecinku_string.substr(0, 2);
				D7 = std::stoi(liczba_po_przecinku_string);

			}

			if (D7 > 60) { D8 = D7 - 60; D6++; saveFile << D6 << " : " << D8 << std::endl; }
			else
			{
				saveFile << D6 << " : " << D7 << std::endl;
			}
		}
	}

	saveFile.close();
}
//4
void Analize::LengthComparisonForFlight(unsigned int _dep,std::string _v)
{
	std::ofstream saveFile(_v);
	int D1 = 0, D2 = 0, D3 = 0, D8 = 0, D1_ = 0, D2_ = 0, D3_ = 0, D8_ = 0, D9 = 0, D10 = 0; 
	double D5 = 0.0, D4 = 0.0, D5_ = 0.0, D4_ = 0.0;
	std::string delimeter;
	std::string liczba_calkowita_string;
	std::string liczba_po_przecinku_string;
	std::string liczba_string;



	for (unsigned int ip = 0; ip < RealFlightData.size() && ip < PlaningFlightData.size(); ip++)
	{
		if (_dep == RealFlightData[ip].uid && _dep == PlaningFlightData[ip].uid)
		{

			//roznica
			//planowane
			D1 = (Ptstart.hour * 3600) + (Ptstart.minute * 60) + Ptstart.second;
			D2 = (Ptarr.hour * 3600) + (Ptarr.minute * 60) + Ptarr.second;

			D3 = D2 - D1;
			D4 = D3 / 60.0;
			
			if (D4 > 60) D5 = D4 / 60.0;
			liczba_string = std::to_string(D5);
			delimeter = ".";
			liczba_calkowita_string = liczba_string.substr(0, liczba_string.find(delimeter));
			int D6 = std::stoi(liczba_calkowita_string);
			liczba_po_przecinku_string = liczba_string.substr(liczba_string.find(delimeter) + 1);
			liczba_po_przecinku_string = liczba_po_przecinku_string.substr(0, 2);
			int D7 = std::stoi(liczba_po_przecinku_string);

			//rzeczywiste
			D1_ = (Rtstart.hour * 3600) + (Rtstart.minute * 60) + Rtstart.second;
			D2_ = (Rtarr.hour * 3600) + (Rtarr.minute * 60) + Rtarr.second;

			D3_ = D2_ - D1_;
			D4_ = D3_ / 60.0;
			
			if (D4_ > 60) D5_ = D4_ / 60.0; 
			liczba_string = std::to_string(D5_);
			delimeter = ".";
			liczba_calkowita_string = liczba_string.substr(0, liczba_string.find(delimeter));
			int D6_ = std::stoi(liczba_calkowita_string);
			liczba_po_przecinku_string = liczba_string.substr(liczba_string.find(delimeter) + 1);
			liczba_po_przecinku_string = liczba_po_przecinku_string.substr(0, 2);
			int D7_ = std::stoi(liczba_po_przecinku_string);
		

			if (D7 > 60 || D7_ > 60)
			{
				D8 = D7 - 60; 
				D6++;
				D8_ = D7_ - 60; 
				D6_++;
				D9 = D6_ - D6;
				D10 = D8_ - D8;	//roznica dl trwania lotu planowanego i rzeczywistego
				if (D10 < 0) { D10 * -1; }
				saveFile << D9 << ":" << D10 << std::endl;
			}
			else
			{
				D9 = D6_ - D6;
				D10 = D7_ - D7;
				if (D10 < 0) { D10 *=-1;}
				saveFile << D9 << ":" << D10 << std::endl;
			}
		}
	}
	
	saveFile.close();
}
//5
void Analize::LengthComparisonForAirlines(std::string _dep,std::string _v) // jak bedzie zle( jak pobiera co nie trzeba ) to dep_
{
	//for (unsigned int i = 0; i < Line.size(); i++ )
	//{
		//if (dep_ == Line[i])
		//{
	std::ofstream saveFile(_v);
	int D1 = 0, D2 = 0, D3 = 0, D8 = 0, D1_ = 0, D2_ = 0, D3_ = 0, D8_ = 0, D9 = 0, D10 = 0;
	double D5 = 0.0, D4 = 0.0, D5_ = 0.0, D4_ = 0.0;
	std::string delimeter;
	std::string liczba_calkowita_string;
	std::string liczba_po_przecinku_string;
	std::string liczba_string;



	for (unsigned int ip = 0; ip < RealFlightData.size() && ip < PlaningFlightData.size(); ip++)
	{
		if (_dep == RealFlightData[ip].airline && _dep == PlaningFlightData[ip].airline)
		{

			//roznica
			//planowane
			D1 = (Ptstart.hour * 3600) + (Ptstart.minute * 60) + Ptstart.second;
			D2 = (Ptarr.hour * 3600) + (Ptarr.minute * 60) + Ptarr.second;

			D3 = D2 - D1;
			D4 = D3 / 60.0;

			if (D4 > 60) D5 = D4 / 60.0;
			liczba_string = std::to_string(D5);
			delimeter = ".";
			liczba_calkowita_string = liczba_string.substr(0, liczba_string.find(delimeter));
			int D6 = std::stoi(liczba_calkowita_string);
			liczba_po_przecinku_string = liczba_string.substr(liczba_string.find(delimeter) + 1);
			liczba_po_przecinku_string = liczba_po_przecinku_string.substr(0, 2);
			int D7 = std::stoi(liczba_po_przecinku_string);

			//rzeczywiste
			D1_ = (Rtstart.hour * 3600) + (Rtstart.minute * 60) + Rtstart.second;
			D2_ = (Rtarr.hour * 3600) + (Rtarr.minute * 60) + Rtarr.second;

			D3_ = D2_ - D1_;
			D4_ = D3_ / 60.0;

			if (D4_ > 60) D5_ = D4_ / 60.0;
			liczba_string = std::to_string(D5_);
			delimeter = ".";
			liczba_calkowita_string = liczba_string.substr(0, liczba_string.find(delimeter));
			int D6_ = std::stoi(liczba_calkowita_string);
			liczba_po_przecinku_string = liczba_string.substr(liczba_string.find(delimeter) + 1);
			liczba_po_przecinku_string = liczba_po_przecinku_string.substr(0, 2);
			int D7_ = std::stoi(liczba_po_przecinku_string);


			if (D7 > 60 || D7_ > 60)
			{
				D8 = D7 - 60;
				D6++;
				D8_ = D7_ - 60;
				D6_++;
				D9 = D6_ - D6;
				D10 = D8_ - D8;	//roznica dl trwania lotu planowanego i rzeczywistego
				if (D10 < 0) { D10 * -1; }
				saveFile << D9 << ":" << D10 << std::endl;
			}
			else
			{
				D9 = D6_ - D6;
				D10 = D7_ - D7;
				if (D10 < 0) { D10 *= -1; }
				saveFile << D9 << ":" << D10 << std::endl;
			}
		}
	}

	saveFile.close();
}
//6
void Analize::StartRemainderForAirlines(std::string _dep,std::string _v)
{
	std::ofstream saveFile(_v);
	int D1 = 0, D2 = 0, D3 = 0, D4 = 0, D8 = 0;
	double D5 = 0;
	for (unsigned int ip = 0; ip < RealFlightData.size() && ip < PlaningFlightData.size(); ip++)
	{
		if (_dep == RealFlightData[ip].airline && _dep == PlaningFlightData[ip].airline)
		{
			D1 = (Rtstart.hour * 3600) + (Rtstart.minute * 60) + Rtstart.second;
			D2 = (Ptstart.hour * 3600) + (Ptstart.minute * 60) + Ptstart.second;
			D3 = D2 - D1;
			D4 = D3 / 60;
			if (D4 > 60) D5 = D4 / 60;
			std::string liczba_string = std::to_string(D5);
			std::string delimeter = ".";
			std::string liczba_calkowita_string = liczba_string.substr(0, liczba_string.find(delimeter));
			int D6 = std::stoi(liczba_calkowita_string);
			std::string liczba_po_przecinku_string = liczba_string.substr(liczba_string.find(delimeter));
			int D7 = std::stoi(liczba_calkowita_string);
			if (D7 > 60) D8 = D7 - 60;

			saveFile << D6 << " : " << D8 << std::endl;
		}
	}
	//std::cout << Dhour << ":" << Dminute << ":" << Dsecond << std::endl;
	
	saveFile.close();
}
//7
void Analize::ArrRemainderForAirlines(std::string _dep,std::string _v)
{
	std::ofstream saveFile(_v);
	int D1 = 0, D2 = 0, D3 = 0, D8 = 0, D6 = 0, D7 = 0;
	double D5 = 0.0, D4 = 0.0;

	for (unsigned int ip = 0; ip < RealFlightData.size() && ip < PlaningFlightData.size(); ip++)
	{
		if (_dep == RealFlightData[ip].airline && _dep == PlaningFlightData[ip].airline)
		{
			//roznica

			D1 = (Ptarr.hour * 3600) + (Ptarr.minute * 60) + Ptarr.second;
			D2 = (Rtarr.hour * 3600) + (Rtarr.minute * 60) + Rtarr.second;

			D3 = D2 - D1;
			if (D3 < 0) D3 *= -1;
			D4 = D3 / 60.0;
			if (D4 > 60.0)
			{
				D5 = D4 / 60.0;
				std::string liczba_string = std::to_string(D5);
				std::string delimeter = ".";
				std::string liczba_calkowita_string = liczba_string.substr(0, liczba_string.find(delimeter));
				D6 = std::stoi(liczba_calkowita_string);
				std::string liczba_po_przecinku_string = liczba_string.substr(liczba_string.find(delimeter) + 1);
				liczba_po_przecinku_string = liczba_po_przecinku_string.substr(0, 2);
				D7 = std::stoi(liczba_po_przecinku_string);
			}
			else
			{
				std::string liczba_string = std::to_string(D4);
				std::string delimeter = ".";
				std::string liczba_calkowita_string = liczba_string.substr(0, liczba_string.find(delimeter));
				D6 = std::stoi(liczba_calkowita_string);
				std::string liczba_po_przecinku_string = liczba_string.substr(liczba_string.find(delimeter) + 1);
				liczba_po_przecinku_string = liczba_po_przecinku_string.substr(0, 2);
				D7 = std::stoi(liczba_po_przecinku_string);

			}

			if (D7 > 60) { D8 = D7 - 60; D6++; saveFile << D6 << " : " << D8 << std::endl; }
			else
			{
				saveFile << D6 << " : " << D7 << std::endl;
			}
		}
	}

	saveFile.close();
}
//8
void Analize::GreatestDistanceDifference(unsigned int _dep,std::string _v)
{
	std::ofstream saveFile(_v);
	double a, b,c;

	for (unsigned int ip = 0; ip < RealFlightData.size() && ip < PlaningFlightData.size(); ip++)
	{
		if (_dep == RealFlightData[ip].uid && _dep == PlaningFlightData[ip].uid)
		{
			//Punkty znajduja sie na tej samej polkuli N (poniewaz plik z danymi obejmuje USA) co oznacza ze wartosci sie odejmuje
			//odejmowanie stopni
			a = RpositionPointNS.degrees - PpositionPointNS.degrees;
			if (a < 0)
			{
				a *= -1;
			}

			//odejmowanie minut, tak naprawde wartosc to sekundy
			b = RpositionPointNS.minutes - PpositionPointNS.minutes;
			if (b < 0)
			{
				b *= -1;
			}
			//dzielenie sekund przez 60, tak by otrzymac wartosc w minutach, nastepnie mnozenie razy 1.85 co daje wynik w kilometrach
			c = (b / 60)*1.85;
		}
	}
	//std::cout << "Samolot znajduje sie " << std::setprecision(4)<< c << " kilometrow od miejsca w ktorym powinien byc" << std::endl;
	saveFile << "Samolot znajduje sie " << std::setprecision(4)<< c << " kilometrow od miejsca w ktorym powinien byc" << std::endl;
	saveFile.close();
}


