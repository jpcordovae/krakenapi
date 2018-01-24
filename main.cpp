#include <iostream>
#include <stdexcept>
#include <chrono>

//#include "kapi.hpp"
#include "kraken/kclient.hpp"

using namespace std;
using namespace kraken;

//------------------------------------------------------------------------------

int main() 
{ 
   curl_global_init(CURL_GLOBAL_ALL);

   try {
     KInput in;
     KClient client;
     //KClient client( "psfPdijFav1ueTKaIIE1eQ307Sf3RDwfjDiT19BwGJz7n0rU8ZAyZADU","NCbhpXG7QWWtQivHI3f4p04jSE6Df+x89aLFTzOFFHIsuGjERbA4mDFuhaYnF+zLxFFEYL7SvQPb/C9gz9/MdA==");
     
     //**************************************************
     cout << endl;
     cout << "ORDERBOOK" << endl;
     KOrderBook kob;
     cout << client.orderbook("ETHUSD","0",kob) << endl;

     //**************************************************
     cout << endl;
     cout << "TRADES" << endl;
     std::vector<KTrade> ktrades;
     cout << client.trades("ETHUSD","0",ktrades) << endl;

     //*************************************************
     cout << endl;
     cout << "SPREAD" << endl;
     KSpreadStorage kss;
     cout << client.spread("ETHUSD","0", kss) << endl;

     //************************************************
     cout << endl;
     cout << "ASSETS" << endl;
     KAssetsMap kam;
     cout << client.update_assets(kam) << endl;

     //************************************************
     cout << endl;
     cout << "OHLC" << endl;
     KOHLCStorage kohlcs;
     cout << client.OHLC("ETHUSD","0","1",kohlcs) << endl;

     //************************************************
     cout << endl;
     cout << "SERVER TIME" << endl;
     cout << client.update_server_time() << endl;
     
   }
   catch(exception& e) {
      cerr << "Error: " << e.what() << endl;
   }
   catch(...) {
      cerr << "Unknow exception." << endl;
   }

   curl_global_cleanup();
   return 0;
}
 
