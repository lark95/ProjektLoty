#pragma once
#include "FlightData.h"
#include <string>
#include <math.h>


class Analize: public Time, public Date, public Coordinates, public FlightData
{ 
public:
	Analize();
    ~Analize();


	// FUNKCJE WCZYTUJACE DANE Z PLIKOW

	void LoadPlaningData(std::string);						// f. wczytujaca planowane loty
	void LoadRealData(std::string);							// f. wczytujaca rzeczywiste loty	
	//void LoadLine();
	//void LoadUid();
	// FUNKCJE TWORZACE OBIEKTY LOTOW

	FlightData LoadRealFlights(std::string);				// f. tworzaca obiekt danego lotu
	CheckPoint LoadRealCheckPoint(std::string);				// f. tworzaca obiekt danych lotu
	FlightData LoadPlaningFlights(std::string);				// f. tworzaca obiekt danego lotu
	CheckPoint LoadPlaningCheckPoint(std::string);			// f. tworzaca obiekt danych lotu

	// FUNKCJE SPRAWDZAJACE POPRAWNOSC DANYCH

	short CheckFile(std::string);							// f. sprawdzajaca istnienie pliku
	short CheckHour(short);									// f. sprawdzajaca poprawnosc godzinowa
	short CheckMinute(short);								// f. sprawdzajaca poprawnosc minutowa

	// FUNKCJE WYSZUKUJACE

	
	unsigned int FindRealUid(unsigned int);					// f. wyszukujaca UID lotu rzeczywistego
	unsigned int FindUid(unsigned int);
	std::string FindLine(std::string); // wyszukiwanie lini lotniczej - dodane 14.09.17
	bool UidCheck(int);
	bool LineCheck(std::string);

	//FUNKCJE WYKONAWCZE PROGRAMU
	

	void StartRemainderForFlight(unsigned int,std::string);			// f. licząca roznice czasu startu P i R dla wybranego lotu
	void ArrRemainderForFlight(unsigned int,std::string);						// f. liczaca roznice czasu przylotu P i R dla wybranego lotu
	void LengthComparisonForFlight(unsigned int,std::string);					// f. porownujaca czas trwania lotu P i R dla wybranego lotu
	void LengthComparisonForAirlines(std::string,std::string);					// f. porownujaca czas trwania lotu P i R dla wybranej linii lotniczej
	void StartRemainderForAirlines(std::string,std::string);					// f. liczaca roznice czasu startow P i R dla wybranej linii lotniczej
	void ArrRemainderForAirlines(std::string,std::string);						// f. liczaca roznice czaqsu przylotow P i R dla wybranej linii lotniczej
	void GreatestDistanceDifference(unsigned int,std::string);					// f. liczaca najwieksza z odleglosci od pkt w ktorym powinien byc samolot
	
	// ZMIENNE

	protected:

	std::vector <FlightData> PlaningFlightData;				// k. przechowujacy loty planowane
	std::vector <FlightData> RealFlightData;				// k. przechowujacy loty rzeczywiste
	std::vector <std::string> Line;
	std::vector <int> Uid;
	Time positionTimeP;
	Time positionTimeR;
	Time Ptstart;
	Time Rtstart;
	Time Ptarr;
	Time Rtarr;

	Coordinates PpositionPointNS;
	Coordinates RpositionPointNS;
	
};



