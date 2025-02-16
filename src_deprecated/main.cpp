// main.cpp
// Main 

#include <iostream>
#include <vector>
#include <math.h>
#include <algorithm>
#include <thread>

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <poll.h>

#define PORT_NUMBER 80
#define QUEUE_LIMIT 5
#define POLL_INTERVAL 100
#define MAX_SERVER_LOOP 600

#include "web.cpp"

using namespace std;


// generate internet socket address with specified port on all network interfaces 
sockaddr_in get_lan_addr_in(int port) {
  sockaddr_in server_address;
  server_address.sin_family = AF_INET; 
  server_address.sin_port = htons(port); 
  server_address.sin_addr.s_addr = INADDR_ANY;
  return server_address;
}

void main_server_process() {
  cout << "Starting..." << endl; 

  int server_socket = socket(AF_INET, SOCK_STREAM, 0);
  sockaddr_in server_address = get_lan_addr_in(PORT_NUMBER);

  bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address));
  listen(server_socket, QUEUE_LIMIT);

  pollfd server_file_descriptor[1];
  server_file_descriptor[0].fd = server_socket;
  server_file_descriptor[0].events = POLLIN;

  cout << "Polling..." << endl;
  for (int eventCount = 0; eventCount >= 0; eventCount = poll(server_file_descriptor, 1, POLL_INTERVAL)) {
    if (eventCount > 0) {
      int client_socket = accept(server_socket, nullptr, nullptr);
      char buffer[2048] = {0};

      recv(client_socket, buffer, sizeof(buffer), 0);

      string response_string = webserver(client_socket, buffer, sizeof(buffer));
      send(client_socket, response_string.c_str(), response_string.length(), 0);

      close(client_socket);
      cout << "Finished Request." << endl;
    }
  }
  
  close(server_socket);
  cout << "Done." << endl;
}

int main () {
  build_request_cache();
  main_server_process();
  return 0;
}
