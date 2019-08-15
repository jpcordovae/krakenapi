#include <stdlib.h>
#include <iostream>
#include <list>
#include <string>
#include <memory>

#include "kraken/kutils.hpp"
#include "kraken/kclient.hpp"

using namespace std;
using namespace kraken;

typedef struct client_data {
  KInput in;
  KClient client;
} client_data;

client_data CD;

int main(int argc, char **argv)
{
  std::vector<std::string> vString;
  string path = "/home/jcordovaech/krakenapi/build/data/";
  get_files_from_directory(path,vString);
  std::for_each(vString.begin(),vString.end(),[&](const std::string &str){
      std::cout << str << std::endl;
    });
  return EXIT_SUCCESS;
}
