#include "kclient.cpp"
#include <stdlib.h>
#include <iostream>
#include <list>
#include <string>
#include <memory>
#include <filesystem>

using namespace std;
nmaespace fs = std::filesystem;

typedef struct client_data {
  KInput in;
  KClient client;
} client_data;

client_data CD;


list<string> get_file_names(string directory)
{
  list<string> tmp;
  return tmp;
}

int main(int argc, char argv[])
{
  string path = "/home/jpce/RPIO/kraken/build/data/";
  string filename = "15*";
  return EXIT_SUCCESS;
}


