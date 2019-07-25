#include <iostream>
#include <stdexcept>
#include <chrono>
#include <ctime>
#include <thread>
#include <mutex>
#include <tuple>
#include <algorithm>

#include "kutils.hpp"
#include "kraken/kplot.hpp"
#include "kraken/kclient.hpp"
#include "kraken/kohlc.hpp"
#include "kraken/korderbook.hpp"

using namespace std;
using namespace kraken;
//------------------------------------------------------------------------------

typedef struct client_data {
  KInput in;
  KClient client;
} client_data;

client_data CD;
mutex mtxCD;
KAssetsMap kam;

void ohlcs_thread_function()
{
  cout << "initializing OHLC thread..." << endl;
  KOHLCStorage kohlcs;
  std::string last = "0";
  SafePrint{} << "kohlcs thread starting..." << std::endl;
  std::string data_directory = "data/ohlc_";
  while(1) {
    try{
      //BEGIN Critical Section
      mtxCD.lock();
      last = CD.client.OHLC("ETHUSD",last.c_str(),"1",kohlcs);
      mtxCD.unlock(); 
      //END   Critical Section
      if(kohlcs.size()>=360) {
		ofstream ffile(data_directory+last,ios::out | ios::binary | ios_base::ate);
		ffile.write((char*)kohlcs.data(),kohlcs.size());
		ffile.close();
		//cout << kohlcs  << endl;
		kohlcs.clear();
      }
      SafePrint{} << "buffer size: " << kohlcs.size() << std::endl;
    }
    catch(exception &e) {
      SafePrint{} << "Error :" << e.what() << endl;
    }
    catch(...) {
      SafePrint{} << "Unknow exception." << endl;
    }
    this_thread::sleep_for(chrono::milliseconds(5000));
  }
}

void order_book_thread_function( string _asset )
{
  SafePrint{} << "initializing order book thread..." << endl;
  std::string filename = "data/kraken_order_book.bin";
  KOrderBook kob;
  KOBStorage kobs;
  
  try {

    while(1) {
      clear_korderbook(kob);
      //BEGIN Critical Section
      mtxCD.lock();
      CD.client.orderbook(_asset,kob);
      mtxCD.unlock();
      //END   Critical Section
	  kobs.push_back(kob);
      if(kobs.size()>=10) {
		ofstream ffile(filename,ios::out | ios::binary | ios_base::ate);
		ffile.write((char*)kobs.data(),kobs.size());
		ffile.close();
		kobs.clear();
      }
      SafePrint{} << "order book buffer size: " << kobs.size() << std::endl;
      std::vector<double> prices_asks  = get_prices(kob.lAsks);
      std::vector<double> volumes_asks = get_volumes(kob.lAsks);
      std::vector<double> prices_bids  = get_prices(kob.lBids);
      std::vector<double> volumes_bids = get_volumes(kob.lBids);
      double mmin = *(min_element(prices_bids.begin(),prices_bids.end()));
      double mmax = *(max_element(prices_asks.begin(),prices_asks.end()));
      //gp << "set yrange [0:1500]\n";
      gp << "set xrange [" << mmin << ":" << mmax << "]\n";
      gp << "set grid ytics lt 0 lw 1\n";
      gp << "set grid xtics lt 0 lw 1\n";
      gp << "plot '-' with points title 'asks', '-' with points title 'bids'\n";
      gp.send1d(make_tuple(prices_asks,volumes_asks));
      gp.send1d(make_tuple(prices_bids,volumes_bids));
      //gp.send1d(make_tuple(prices_asks,volumes_asks,prices_bids,volumes_bids));
      this_thread::sleep_for(chrono::milliseconds(2500));// 60 seconds
	  
    }
  } catch( const exception& e ) {
    cerr << "korderbook Error: " << e.what() << endl;
  } catch(...) {
    cerr << "korderbook Unknow exception." << endl;
  }
}

void spread_thread_function()
{
  SafePrint{} << "initializing spread thread..." << endl;
  string last = "0";
  string data_directory = "data/spread_";
  KSpreadStorage kss;
  try {
    while(1) {
      //BEGIN Critical Section
      mtxCD.lock();
      last = CD.client.spread("ETHUSD",last,kss);
      mtxCD.unlock();
      //END   Critical Section

	  /*for_each(kss.begin(),kss.end(),[&](const KSpread &ks){
									   cout << to_string(ks.time)
											<< " , " << ks.bid
											<< " , " << ks.ask << endl;});*/
      //gp << "set yrange [0:1500]\n";
      //gp << "set xrange [750:850]\n";
      //gp << "plot '-' with points title 'asks', '-' with points title 'bids'\n";
      //gp.send1d(make_tuple(prices_asks,volumes_asks));
      //gp.send1d(make_tuple(prices_bids,volumes_bids));
      //gp.send1d(make_tuple(prices_asks,volumes_asks,prices_bids,volumes_bids));
	  
	  // save to a file
	  if(kss.size()>=10*1024) {
		ofstream ffile(data_directory+last,ios::out | ios::binary | ios_base::ate);
		ffile.write((char*)kss.data(),kss.size());
		ffile.close();
		//cout << kohlcs  << endl;
		kss.clear();
      }

      this_thread::sleep_for(chrono::milliseconds( 2500));// 60 seconds
    }
  } catch( exception& e ) {
    SafePrint{} << "Error: " << e.what() << endl;
  } catch(...) {
    SafePrint{} << "Unknow exception." << endl;
  } 
}

void trades_thread_function(string pair, string &last)
{
  string path = "data/trades_";
  KTradeStorage kts;
  try{
    //BEGIN Critical Section
	mtxCD.lock();
	last = CD.client.trades("ETHUSD",last,kts);
	mtxCD.unlock();
	//END   Critical Section
	//ktstorage.
	if(kts.size()>=1000){
	  // save trades to a file
	}
  }
  catch(std::exception &e){
	string what = "trade_thread exception: ";
	what += e.what();
	cerr << what << endl;
  }
}


int main(int argc, char **argv) 
{
  curl_global_init(CURL_GLOBAL_ALL);
  cout << "getting assets..." << endl;
  string last_timestamp_spread;
  
  
  //CD.client.print_assets(kam);
  //cout << kss << endl;
	//BEGIN Critical Section
	mtxCD.lock();
	try{
	  //CD.client.update_assets(kam);
	}catch(std::exception &e){
	  SafePrint() << "asset exception: " << e.what() << std::endl;
	}
	mtxCD.unlock();
	//END Critical Section
	// threads
	SafePrint{} << "starting threads..." << endl;
	thread order_book_thread(order_book_thread_function,"ETHUSD");
	thread ohlcs_thread(ohlcs_thread_function);
	thread spread_thread(spread_thread_function);
  
  
  try {  
	
    //**************************************************
    /*cout << endl;
      cout << "TRADES" << endl;
      std::vector<KTrade> ktrades;
      cout << client.trades("ETHUSD","0",ktrades) << endl;
    */
    //**************************************************
    /*cout << endl;
      cout << "SPREAD" << endl;
      KSpreadStorage kss;
      cout << client.spread("ETHUSD","0", kss) << endl;
    */
    //**************************************************
    /*cout << endl;
      cout << "ASSETS" << endl;
      KAssetsMap kam;
      cout << client.update_assets(kam) << endl;
    */
    //**************************************************
    /*cout << endl;
      cout << "OHLC" << endl;
      KOHLCStorage kohlcs;
      mtxCD.lock();
      CD.client.OHLC("ETHUSD","0","1",kohlcs);
      mtxCD.unlock(); 
      cout << kohlcs  << endl;*/
    //**************************************************
    /*cout << endl;
      cout << "SERVER TIME" << endl;
      std::time_t stime;
      cout << client.server_time(stime) << endl;
    */
  }
  catch(exception& e) {
    SafePrint{} << "Error: " << e.what() << endl;
  }
  catch(...) {
    SafePrint{} << "Unknow exception." << endl;
  }
  
  cin.get();
  curl_global_cleanup();
  
  return 0;
}

