/*! 
 *	 SARdataPoint.h
 * 
 *  SARdataPoint class header file: This class represents a single data point
 *  of historical stock price and contains fields for computing the SAR values.
 * 
 *  Author: Mike Ricci
 *  Date: 20140620
 */

#ifndef SARDATAPOINT_H
#define SARDATAPOINT_H

class SARdataPoint
{
public:
	SARdataPoint();
	virtual ~SARdataPoint();
	
	enum direction {DOWN = -1, UP = 1, null = 0};
	
	double getAlpha() const;
	int getDate() const;
	double getEP() const;
	double getHigh() const;
	double getLow() const;
	double getSAR() const;
	direction getTrend() const;

	void setAlpha(double alphaIn);
	void setDate(int dateIn);
	void setEP(double EPin);
	void setHigh(double highIn);
	void setLow(double lowIn);
	void setSAR(double SARin);
	void setTrend(direction trendIn);
	
protected:

private:

	double alpha = 0;
	int date = 0; 
	double EP = 0;
	double high = 0;
	double low = 0;
	double SAR = 0;
	direction trend = null;
	
}; // end of class SARdataPoint

#endif // SARDATAPOINT_H

