/*! 
 *	 SAR.h
 * 
 *  SAR class header file: This class represents a group of SARdataPoint objects
 *  and contains functions which compute SAR values for each object.
 * 
 *  Author: Mike Ricci
 *  Date: 20140620
 */

#ifndef SAR_H
#define SAR_H

class SAR
{
public:
	SAR();
	virtual ~SAR();

	void addDataPoint(int dateIn);
	void calcSAR();
	void initComp();
	void parse(std::string filepath);
	void printSAR() const;
	void printSARfile(std::ofstream& file);

	const double ALPHA_BASE = 0.02;
	const double ALPHA_MAX = 0.20;
	
	std::map<int, SARdataPoint> data;

protected:

private:


}; // end of class SAR

#endif // SAR_H

