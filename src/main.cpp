// main.cpp
// Instantiates and runs a webserver 

#include <string>
#include <iostream>

#include "webserver.cpp"

#define SERVER_PORT 80
#define MAPPING_FILE_DIR "./mapping.txt"
#define WEBSITE_FOLDER_DIR "./public/"
#define SERVER_TICK_INTERVAL 100

using namespace std;


// Main 
int main () {
  cout << "Instantiating Server..." << endl;
  struct webserver* server = new webserver(SERVER_PORT);

  cout << "Loading Webpage Mappings and Files..." << endl;
  server->load(MAPPING_FILE_DIR, WEBSITE_FOLDER_DIR);

  cout << "Starting Server..." << endl;
  server->start();

  cout << "Running..." << endl;
  while (server->server_status != webserver::KILL) server->tick(SERVER_TICK_INTERVAL);

  cout << "Quitting Server..." << endl;
  server->quit();

  cout << "Server Closed." << endl;
}
