#pragma once
#include <string>
#include <vector>

struct Time
{
public:

	unsigned short hour;									// z. przechowujaca godzine 
	unsigned short minute;									// z. przechowujaca minute 
	unsigned short second;									// z. przechowujaca sekunde 								
};


struct Date
{
public:
	unsigned short year;									// z. przechowujaca rok 
	unsigned short month;									// z. przechowujaca miesiac 
	unsigned short day;										// z. przechowujaca dzien 
};

struct Coordinates
{
public:
	unsigned short degrees;									// z. przechowujaca stopnie geograficzne 
	std::string position;									// z. przechowujaca symbol polkuli 
	unsigned short minutes;									// z. przechowujaca minuty geograficzne 
};

struct CheckPoint
{
public:
	Coordinates NS;											// z. przechowuaca s. przechowujaca szerokosc geograficzna samolotu
	Coordinates EW;											// z. przechowuaca s. przechowujaca dlugosc geograficzna samolotu
	Time raportTime;										// z. przechowuaca s. przechowujaca czas wyslania raportu 
	unsigned short actualHigh;								// z. przechowujaca aktualna wysokosc
	unsigned short nextHigh;								// z. przechowujaca wysokosc na ktorej bedzie samolot
	short vr;												// z. przechowujaca wspolczynnik wznoszenia
};
struct Uid
{
	unsigned int uid;
};

struct Airline
{
	std::string airline;
};

class FlightData
{
public:
	FlightData();
	FlightData(std::string, unsigned int , Date, std::string , Time , std::string , Time , std::string);
	virtual ~FlightData();
	std::string airline;									// z. przechowujaca dane o linii lotniczej
	unsigned int uid;										// z. przechowujaca unikalny identyfikator lotu
	Date date;												// s. przechowujaca date lotu
	std::vector<CheckPoint> data;							// k. przechowujacy obiekty CheckPoint
	Time Rtstart;											// s. przechowujaca rzeczywisty czas startu
	Time Ptstart;											// s. przechowujaca planowany czas startu
	Uid uid__;
	Airline airline__;
	std::string acn;
	std::string dep;
	Time tdep;
	std::string arr;
	Time tarr;
	std::vector<std::string>AIrline;
	std::vector<unsigned int>UId;
};

