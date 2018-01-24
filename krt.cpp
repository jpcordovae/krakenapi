#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <ctime>

#include <chrono>
#include <thread>

#include "kraken/kclient.hpp"
#include "libjson/libjson.h"

using namespace std;
using namespace kraken;


//------------------------------------------------------------------------------

int main(int argc, char* argv[]) 
{
   try {

      // initialize kraken lib's resources:
      kraken::initialize();

      //
      // command line argument handling:
      //
      // usage:
      //     krt <pair> [interval] [since]
      // 

      string pair;
      string last = "0"; // by default: the oldest possible trade data
      int interval = 0;   // by default: krt exits after download trade data

      switch (argc) {
      case 4:
	 last = string(argv[3]);
      case 3:
	 istringstream(argv[2]) >> interval;      
      case 2:
	 pair = string(argv[1]);
	 break;
      default: 
	 throw runtime_error("wrong number of arguments");
      };
      
      chrono::seconds dura(interval);

      KClient kc;
      vector<KTrade> vt;

      while (true) {
	 // store and print trades
	 last = kc.trades(pair, last, vt);
	 for (int i = 0; i < vt.size(); ++i) 
	    cout << vt[i] << endl;
	    
	 // exit from the loop if interval is 0
	 if (interval == 0) break;
	 
	 // sleep
	 this_thread::sleep_for(dura);
      }

      // terminate kraken lib's resources
      kraken::terminate();
   }
   catch(exception& e) {
      cerr << "Error: " << e.what() << endl;
      exit(EXIT_FAILURE);
   }
   catch(...) {
      cerr << "Unknow exception." << endl;
      exit(EXIT_FAILURE);
   }

   return 0;
}
