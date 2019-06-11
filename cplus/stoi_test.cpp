//#include <stdexcept>
#include <iostream>
#include <string>
using namespace std;
 
int main()
{
	std::string y = "253647586946334221002101219955219971002";
	int x;
 
	try {
		x = std::stoi("");
	}
	catch (std::invalid_argument&){
		// if no conversion could be performed
		cout << "Invalid_argument" << endl;
	}
	catch (std::out_of_range&){
		// if the converted value would fall out of the range of the result type 
		// or if the underlying function (std::strtol or std::strtoull) sets errno 
		// to ERANGE.
		cout << "Out of range" << endl;
	}
	catch (...) {
		// everything else
		cout << "Something else" << endl;
	}
	return 0;
}
