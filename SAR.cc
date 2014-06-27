/*! 
 *	 SAR.cc
 * 
 *  Source file for the SAR class: This class represents a group of SARdataPoint
 *  objects and contains functions which compute SAR values for each object.
 */

#include <map>
#include <string>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <iterator>
#include <boost/tokenizer.hpp>
#include "SARdataPoint.h"
#include "SAR.h"

//Constructor
SAR::SAR()
{

}

//Virtual destructor
SAR::~SAR()
{

}

//Member functions

//The function SAR initializes an SARdataPoint object with the map key value
//of dateIn
void SAR::addDataPoint(int dateIn)
{
	this->data[dateIn] = SARdataPoint();
	this->data[dateIn].setDate(dateIn);
}

//The function calcEP() calculates and sets the SAR values for the SARdataPoint 
//objects contained in the SAR object, starting at the fifth, see pseudocode in
//design explanation for further info
void SAR::calcSAR()
{
	std::map<int, SARdataPoint>::iterator it = this->data.begin();
	std::advance(it, 3); // move iterator it to fourth element
	
	//for loop goes from element 5 through the end
	// it points to day n
	//((++it)--) points to day n+1 and resets the iterator back to day n
	//((--it)++) points to day n-1 and resets the iterator back to day n
	for(it; it != this->data.end(); ++it)
	{

		//SAR
		double recentLow; // find and store recent low price
		if(it->second.getLow() < ((--it)++)->second.getLow())
		{
			recentLow = it->second.getLow();
		}
		else
		{
			recentLow = ((--it)++)->second.getLow();
		}
		double recentHigh; // find and store recent high price
		if(it->second.getHigh() > ((--it)++)->second.getHigh())
		{
			recentHigh = it->second.getHigh();
		}
		else
		{
			recentHigh = ((--it)++)->second.getHigh();
		}
		
		//calculate these tempSARs to make following if's more readable
		double tempSARyesterday = it->second.getSAR() + it->second.getAlpha()
				 				        * (it->second.getEP() - it->second.getSAR());
		double tempSARdayBefore = it->second.getSAR() + ((--it)++)->second.getAlpha()
				 				        * (it->second.getEP() - it->second.getSAR());
		
		if(it->second.getTrend() == ((--it)++)->second.getTrend()) // compute SAR
		{
			if(it->second.getTrend() == SARdataPoint::UP)
			{
				if(tempSARdayBefore < recentLow)
				{
					((++it)--)->second.setSAR(tempSARyesterday);
				}
				else
				{
					((++it)--)->second.setSAR(recentLow);
				}
			}
			else
			{
				if(tempSARyesterday > recentHigh)
				{
					((++it)--)->second.setSAR(tempSARyesterday);
				}
				else
				{
					((++it)--)->second.setSAR(recentHigh);
				}
			}
		}
		else
		{
			((++it)--)->second.setSAR(it->second.getEP());
		} // end SAR computation

		//EP
		if(it->second.getTrend() == SARdataPoint::UP)
		{
			if(((++it)--)->second.getHigh() > it->second.getEP())
			{
				((++it)--)->second.setEP(((++it)--)->second.getHigh());
			}
			else
			{
				((++it)--)->second.setEP(it->second.getEP());
			}
		}
		else
		{
			if(((++it)--)->second.getLow() < it->second.getEP())
			{
				((++it)--)->second.setEP(((++it)--)->second.getLow());
			}
			else
			{
				((++it)--)->second.setEP(it->second.getEP());
			}
		} // end EP computation

		//TREND
		if(it->second.getTrend() == SARdataPoint::UP)
		{
			if(((++it)--)->second.getLow() > ((++it)--)->second.getSAR())
			{
				((++it)--)->second.setTrend(SARdataPoint::UP); // remain UP
			}
			else
			{
				((++it)--)->second.setTrend(SARdataPoint::DOWN); // change to down
			}
		}
		else
		{
			if(it->second.getTrend() == SARdataPoint::DOWN)
			{
				if(((++it)--)->second.getHigh() < ((++it)--)->second.getSAR())
				{
					((++it)--)->second.setTrend(SARdataPoint::DOWN);
				}
				else
				{
					((++it)--)->second.setTrend(SARdataPoint::UP);
				}
			}
		} // end trend computation

		//ALPHA
		if(((++it)--)->second.getTrend() == it->second.getTrend())
		{
			if(((++it)--)->second.getTrend() == SARdataPoint::UP)
			{
				if(((++it)--)->second.getEP() > it->second.getEP())
				{
					if(it->second.getAlpha() == ALPHA_MAX)
					{
						((++it)--)->second.setAlpha(it->second.getAlpha());
					}
					else // alpha not maxed out yet
					{
						((++it)--)->second.setAlpha(it->second.getAlpha() + ALPHA_BASE);
					}
				}
				else // the EP decreased
				{
					((++it)--)->second.setAlpha(it->second.getAlpha());
				}
			}
			else
			{
				if(((++it)--)->second.getEP() < it->second.getEP())
				{
					if(it->second.getAlpha() == ALPHA_MAX)
					{
						((++it)--)->second.setAlpha(it->second.getAlpha());
					}
					else // alpha not maxed out yet
					{
						((++it)--)->second.setAlpha(it->second.getAlpha() + ALPHA_BASE);
					}
				}
				else // the EP increased
				{
					((++it)--)->second.setAlpha(it->second.getAlpha());
				}
			}
		}
		else // the trend has changed, reset alpha to ALPHA_BASE
		{
			((++it)--)->second.setAlpha(ALPHA_BASE);
		} // end alpha computation
		
	} // end for loop
} // end calcSAR() function


//The function initComp() initializes the computations by computing the fifth
//SARdataPoint object's SAR and related values based on the first five highs and
//lows.  It also calculates the first direction, EP, and acceleration factor.
void SAR::initComp()
{
	std::map<int, SARdataPoint>::iterator iter = this->data.begin();

	//find min of first 5 lows and max of first 5 highs
	double minLow = iter->second.getLow();
	double maxHigh = iter->second.getHigh();
	for(iter = this->data.begin(); iter != this->data.end(); ++iter)
	{
		if(std::distance(this->data.begin(), iter) <= 5)
		{
			if(iter->second.getLow() < minLow)
			{
				minLow = iter->second.getLow();
			}
			if(iter->second.getHigh() > maxHigh)
			{
				maxHigh = iter->second.getHigh();
			}
		}
	}

	//apply initial "starter" values to fourth and fifth elements
	for(iter = this->data.begin(); iter != this->data.end(); ++iter)
	{
		if(std::distance(this->data.begin(), iter) == 3) //fourth element
		{
			iter->second.setTrend(SARdataPoint::UP);
		}
		
		if(std::distance(this->data.begin(), iter) == 4) //fifth element
		{
			iter->second.setSAR(minLow); //set SAR to minimum of first five lows
			iter->second.setEP(maxHigh);//set EP to maximum of first five highs
		}
	}
}

//The function parse() uses the Boost Tokenizer library to parse the date, high,
//and low values into SARdataPoint objects.
void SAR::parse(std::string filepath)
{
	typedef boost::tokenizer<boost::escaped_list_separator<char> > Tokenizer;
	std::string line;
	
	std::ifstream file(filepath);

	if (file.is_open())
	{
		while (getline(file, line))
		{
		   Tokenizer tok(line);
			
			for(Tokenizer::iterator iter = tok.begin(); iter != tok.end(); ++iter)
			{
				int currentIndexInt;
				std::string currentIndexStr;

				//something in here was duplicating the final digit of the date,
				//second call to erase is a truncation work-around
				if(std::distance(tok.begin(), iter) == 0 && *iter != "Date")
				{
					currentIndexStr = *iter;
					currentIndexStr.erase(std::remove(currentIndexStr.begin(), 
					                                  currentIndexStr.end(), '-'));
					currentIndexStr.erase(currentIndexStr.size() - 1);
					currentIndexInt = std::stoi(currentIndexStr);
					this->addDataPoint(currentIndexInt);
				}
				
				if(std::distance(tok.begin(), iter) == 2  && *iter != "High")
				{
					this->data[currentIndexInt].setHigh(std::stod(*iter));
				}
				
				if(std::distance(tok.begin(), iter) == 3 && *iter != "Low")
				{
					this->data[currentIndexInt].setLow(std::stod(*iter));
				}
			}
		}
		file.close();
	}
	else 
	{
		std::cout << "Unable to open file"; 
	}
}

//The function printSAR() outputs all dates and calculated SAR values to console
void SAR::printSAR() const
{
	//column headings
	std::cout << std::setw(8) << "Date" << " ";
	std::cout << std::setw(8) << "High" << " ";
	std::cout << std::setw(8) << "Low" << " ";
	std::cout << std::setw(8) << "EP" << " ";
	std::cout << std::setw(8) << "Alpha" << " ";
	std::cout << std::setw(8) << "Trend" << " ";
	std::cout << std::setw(8) << "SAR"  << " " << '\n';

	//column contents
	for(auto const& d : this->data)
	{
		std::cout <<  std::setw(8) << d.second.getDate() << "  ";
		std::cout <<  std::setw(8) << d.second.getHigh() << "  ";
		std::cout <<  std::setw(8) << d.second.getLow() << "  ";
		std::cout <<  std::setw(8) << d.second.getEP() << "  ";
		std::cout <<  std::setw(8) << d.second.getAlpha() << "  ";
		std::cout <<  std::setw(8) << d.second.getTrend() << "  ";
		std::cout <<  std::setw(8) << d.second.getSAR() << '\n';
	}
}

//The function printSARfile() outputs all dates and calculated SAR values to file
void SAR::printSARfile(std::ofstream& file)
{
	//column headings
	file << std::setw(8) << "Date" << " ";
	file << std::setw(8) << "High" << " ";
	file << std::setw(8) << "Low" << " ";
	file << std::setw(8) << "EP" << " ";
	file << std::setw(8) << "Alpha" << " ";
	file << std::setw(8) << "Trend" << " ";
	file << std::setw(8) << "SAR"  << " " << '\n';

	//column contents
	for(auto const& d : this->data)
	{
		file <<  std::setw(8) << d.second.getDate() << "  ";
		file <<  std::setw(8) << d.second.getHigh() << "  ";
		file <<  std::setw(8) << d.second.getLow() << "  ";
		file <<  std::setw(8) << d.second.getEP() << "  ";
		file <<  std::setw(8) << d.second.getAlpha() << "  ";
		file <<  std::setw(8) << d.second.getTrend() << "  ";
		file <<  std::setw(8) << d.second.getSAR() << '\n';
	}
}