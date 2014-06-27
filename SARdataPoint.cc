/*! 
 *	 SARdataPoint.cc
 * 
 *  Source file for the SARdataPoint class: This class represents a single data 
 *  point of historical stock price and contains fields for computing the SAR 
 *  values.
 */

#include <string>
#include "SARdataPoint.h"

//Constructor
SARdataPoint::SARdataPoint()
{

}

//Virtual Destructor
SARdataPoint::~SARdataPoint()
{

}

//Member Functions (all are accessors and mutators)

//The getAlpha() function returns the acceleration factor, alpha, as a float.
double SARdataPoint::getAlpha() const
{
	return alpha;
}

//The getDate() function returns the date as an int in ISO format (YYYYMMDD).
int SARdataPoint::getDate() const
{
	return date;
}

//The getEP() function returns the extreme point, EP, as a float.
double SARdataPoint::getEP() const
{
	return EP;
}

//The getHigh() function returns the high price, high, as a float.
double SARdataPoint::getHigh() const
{
	return high;
}

//The getLot() function returns the low price, low, as a float.
double SARdataPoint::getLow() const
{
	return low;
}

//The getSAR() functions returns the SAR as a float.
double SARdataPoint::getSAR() const
{
	return SAR;
}

//The getTrend() function returns the trend direction as an enum direction type.
SARdataPoint::direction SARdataPoint::getTrend() const
{
	return trend;
}

//The setAlpha() function sets the acceleration factor, alpha, to the float alphaIn 
void SARdataPoint::setAlpha(double alphaIn)
{
	this->alpha = alphaIn;
}

//The setDate() function sets the date to be the ISO formatted int dateIn.
void SARdataPoint::setDate(int dateIn)
{
	this->date = dateIn;
}

//The setEP() function sets the extreme point, EP, to be the float EPin.
void SARdataPoint::setEP(double EPin)
{
	this->EP = EPin;
}

//The setHigh() functions sets the high price to be the float highIn.
void SARdataPoint::setHigh(double highIn)
{
	this->high = highIn;
}

//The setLow() function sets the low price to be the float lowIn.
void SARdataPoint::setLow(double lowIn)
{
	this->low = lowIn;
}

//The setSAR() function sets the SAR to be the float SARin.
void SARdataPoint::setSAR(double SARin)
{
	this->SAR = SARin;
}

//The setTrend() function sets the trend direction to trendIn.
void SARdataPoint::setTrend(SARdataPoint::direction trendIn)
{
	this->trend = trendIn;
}