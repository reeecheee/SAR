/*! \mainpage
 *  Object-oriented Programming in C++, Professor Doug Ferguson \n
 *  JHU EP, Summer 2014 \n
 *  20140620, Mike Ricci \n\n
 *  HW3: Parabolic SAR \n
 *	 main.cc
 * 
 *  This parabolic stop-and-reverse (SAR) program parses historical stock price
 *  data from CSV files and computes the SAR values by date.
 * 
 *  Input: CSV files containing historical price data for various stocks are
 *  parsed using Boost::Tokenizer library.
 * 
 *  Output: Displays the SAR values by date for the stock
 *
 *  Limitations: Rounding errors may possibly be encountered from the use of
 *  floats to represent currency values.  Doxygen style comments are not entirely
 *  implemented.
 */

#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <array>
#include "SARdataPoint.h"
#include "SAR.h"

using namespace std;

int main()
{
	array<string, 10> files = {"/home/reechee/Documents/Cpp/HW3/ASPS.csv",
						           "/home/reechee/Documents/Cpp/HW3/AZO.csv",
						           "/home/reechee/Documents/Cpp/HW3/CNP.csv",
		                       "/home/reechee/Documents/Cpp/HW3/COST.csv",
						           "/home/reechee/Documents/Cpp/HW3/CVS.csv",
						           "/home/reechee/Documents/Cpp/HW3/DBP.csv",
		                       "/home/reechee/Documents/Cpp/HW3/DGL.csv",
						           "/home/reechee/Documents/Cpp/HW3/JJP.csv",
						           "/home/reechee/Documents/Cpp/HW3/ORLY.csv",
	                          "/home/reechee/Documents/Cpp/HW3/PHYS.csv"};

	SAR SAR1 = SAR(); // initialize SAR object

	ofstream output; // initialize output file
	output.open("/home/reechee/Documents/Cpp/HW3/output.txt"); // open output file

	for(auto& f : files)
	{
		cout << "Now reading file: " << f << '\n';
		SAR1.parse(f);
		SAR1.initComp();
		SAR1.calcSAR();
		SAR1.printSARfile(output); // throws seg fault
	}

	output.close(); // close output file
	
	return 0;
} // end main()

