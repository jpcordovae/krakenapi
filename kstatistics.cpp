#include <iostream>
#include <stdexcept>
#include <chrono>
#include <ctime>
#include <thread>
#include <mutex>
#include <tuple>
#include <algorithm>

#include "kraken/kutils.hpp"
#include "kraken/kplot.hpp"
#include "kraken/kclient.hpp"
#include "kraken/kohlc.hpp"
#include "kraken/korderbook.hpp"
#include "kraken/ktrade.hpp"

// deamonization
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>

#define LOG_FILE "example.log"
#define TMP_FILENAME "/tmp/tmpkstatistics.tmp"

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
KOHLCStorage kohlcs;
KOBStorage kobs;
KTradeStorage kts;

void ohlcs_thread_function(const string kpair)
{
  cout << "initializing OHLC thread..." << endl;
  std::string last = "0";
  SafePrint{} << "kohlcs thread starting..." << std::endl;
  std::string data_directory = "data/ohlc_";
  while(1) {
    try{
      //BEGIN Critical Section
      mtxCD.lock();
      last = CD.client.OHLC(kpair,last.c_str(),"1",kohlcs);
      mtxCD.unlock();
      //END   Critical Section
      if(kohlcs.size()>=360) { // 10 mins aprox
		ofstream ffile(data_directory+last,ios::out | ios::binary | ios_base::ate);
		ffile << kohlcs;
		ffile.close();
		kohlcs.clear();
      }
      SafePrint{} << "ohlc size: " << kohlcs.size() << "bytes" << std::endl;
    }
    catch(exception &e) {
      SafePrint{} << "OHLC thread error :" << e.what() << endl;
    }
    catch(...) {
      SafePrint{} << "Unknow exception." << endl;
    }
    this_thread::sleep_for(chrono::milliseconds(5000));
  }
}

void order_book_thread_function( const string kpair )
{
  SafePrint{} << "initializing order book thread..." << endl;
  std::string filename = "data/order_book.bin";
  KOrderBook kob;

  try {
    while(1) {
      clear_korderbook(kob);
	  
      //BEGIN Critical Section
      mtxCD.lock();
      CD.client.orderbook(kpair,kob);
      kobs.push_back(kob);
      mtxCD.unlock();
      //END   Critical Section
	  
	  //SafePrint{} << "OrderBook size: " << endl  << kob.pair << ", asks: " << kob.lAsks << ", bids" << kob.lBids << std::endl;
      SafePrint{} << "order book storage size : " << kobs.size() << std::endl;

      if(kobs.size()>=10) {
        ofstream ffile(filename,ios::out | ios::binary | ios_base::ate);
        ffile << kobs;
        ffile.close();
        kobs.clear();
      }
	  
      std::vector<double> prices_asks  = get_prices(kob.lAsks);
      std::vector<double> volumes_asks = get_volumes(kob.lAsks);
      std::vector<double> prices_bids  = get_prices(kob.lBids);
      std::vector<double> volumes_bids = get_volumes(kob.lBids);
      double mmin = *(min_element(prices_bids.begin(),prices_bids.end()));
      double mmax = *(max_element(prices_asks.begin(),prices_asks.end()));
      //gp << "set yrange [0:1500]\n";
      //gp << "set xrange [" << mmin << ":" << mmax << "]\n";
      //gp << "set grid ytics lt 0 lw 1\n";
      //gp << "set grid xtics lt 0 lw 1\n";
      //gp << "plot '-' with points title 'asks', '-' with points title 'bids'\n";
      //gp.send1d(make_tuple(prices_asks,volumes_asks));
      //gp.send1d(make_tuple(prices_bids,volumes_bids));
      //gp.send1d(make_tuple(prices_asks,volumes_asks,prices_bids,volumes_bids));
      this_thread::sleep_for(chrono::milliseconds(2500));// 60 seconds
    }
  } catch( const exception& e ) {
    cerr << "korderbook Error: " << e.what() << endl;
  } catch(...) {
    cerr << "korderbook Unknow exception." << endl;
  }
}

void spread_thread_function(const string kpair)
{
  SafePrint{} << "initializing spread thread..." << endl;
  string last = "0";
  string data_directory = "data/spread_";
  KSpreadStorage kss;

  try {
    while(1) {
      //BEGIN Critical Section
      mtxCD.lock();
      last = CD.client.spread(kpair,last,kss);
      mtxCD.unlock();
      //END   Critical Section
      SafePrint() << "spread: " << kss.size() << " bytes" << endl;
      //gp << "set yrange [0:1500]\n";
      //gp << "set xrange [750:850]\n";
      //gp << "plot '-' with points title 'asks', '-' with points title 'bids'\n";
      //gp.send1d(make_tuple(prices_asks,volumes_asks));
      //gp.send1d(make_tuple(prices_bids,volumes_bids));
      //gp.send1d(make_tuple(prices_asks,volumes_asks,prices_bids,volumes_bids));
      // save to a file
      if(kss.size()>=360) {
        ofstream ffile(data_directory+last,ios::out | ios::binary | ios_base::ate);
        ffile << kss;
        ffile.close();
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

void trades_thread_function(const string kpair)
{
  SafePrint() << "initializing trades thread..." << endl;
  string data_directory = "data/trades_";
  string last = "0";
  try{
    while(1) {
      //Begin Critical Section
      mtxCD.lock();
      last = CD.client.trades(kpair,last,kts);
      mtxCD.unlock();
	  //SafePrint{} << "Trades: " << kts << endl;
      //END   Critical Section
      SafePrint{} << "trades: " << kts.size() << " bytes " << endl;
      // save to a file
      if(kts.size()>=10*1024) {
		ofstream ffile(data_directory+last,ios::out | ios::binary | ios_base::ate);
		ffile << kts;
		ffile.close();
		kts.clear();
      }
      this_thread::sleep_for(chrono::milliseconds(2500));
    }
  }
  catch(std::exception &e){
    string what = "trade_thread exception: ";
    what += e.what();
    SafePrint{} << what << endl;
  }
}

void log_message(char *filename, char *mnessage)
{
  FILE *logfile;
  logfile = fopen(filename,"a");
  if(!logfile) return;
  fclose(logfile);
}

void signal_handler(int sig)
{
  switch(sig){
  case SIGHUP:
    log_message(LOG_FILE,"hangup signal catched");
    break;
  case SIGTERM:
    log_message(LOG_FILE,"terminate signal catched");
    exit(EXIT_SUCCESS);
    break;
  default:
    break;
  }
}

static void daemonize(void)
{
  int pid, sid;
  char str[10];
  memset(str,0x00,sizeof(str));
  /*already a deamon*/
  if(getppid() == 1) return;
  pid = fork();
  if(pid<0){
    exit(EXIT_FAILURE);
  }
  if(pid>0){
    exit(EXIT_SUCCESS);
  }
  sid = setsid();
  if(sid<0){
    exit(EXIT_FAILURE);
  }
  /*if(chdir("/")<0){
    exit(EXIT_FAILURE);
    }*/
  //http://www.enderunix.org/docs/eng/daemon.php
  int lpf = open(TMP_FILENAME,O_RDWR|O_CREAT,0640);
  if(lpf<0) exit(1);
  if(lockf(lpf,F_TLOCK,0)<0) exit(0);
  /* first instance continues */
  sprintf(str,"%d,\n",strlen(str)); /* record pid to lockfile */
  signal(SIGCHLD,SIG_IGN);
  signal(SIGTSTP,SIG_IGN);
  signal(SIGTTOU,SIG_IGN);
  signal(SIGTTIN,SIG_IGN);
  signal(SIGHUP,signal_handler);
  signal(SIGTERM,signal_handler);

}

int main(int argc, char **argv)
{
  daemonize();

  curl_global_init(CURL_GLOBAL_ALL);
  cout << "getting assets..." << endl;
  string last_timestamp_spread;
  string kpair = "XBTUSD";
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

  // close out standard file descriptors
  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

  // Threads
  SafePrint{} << "starting threads..." << endl;
  thread order_book_thread(order_book_thread_function,kpair);
  thread ohlcs_thread(ohlcs_thread_function,kpair);
  thread spread_thread(spread_thread_function,kpair);
  thread trades_thread(trades_thread_function,kpair);

  try {
    while(1){
      this_thread::sleep_for(chrono::milliseconds(5000));
    }
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

