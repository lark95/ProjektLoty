#include "FlightData.h"

FlightData::FlightData()
:
airline()
, uid(999999)
, date()
,dep("Default")
,tdep()
,arr("Default")
,tarr()
,acn("Default")
//, Ptstart()
//, Ptarr()
//, Rtarr()
//, Rtstart()
{
}

FlightData::FlightData(std::string airline_, unsigned int uid_, Date date_ , std::string _dep, Time _tdep, std::string _arr, Time _tarr, std::string _acn)
	
	
	//, Ptstart(Ptstart_)
	//, Ptarr(Ptarr_) 
	//, Rtarr(Rtarr_)
	//, Rtstart(Rtstart_)

{
	airline=airline_;
	uid=uid_;
	date=date_;
	dep = _dep;
	tdep = _tdep;
	arr = _arr;
	tarr = _tarr;
	acn = _acn;
}

FlightData::~FlightData()
{
}
