// webserver.cpp
// Represents a locally hosted webserver that responds to http requests

#include <string>
#include <assert.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <poll.h>

using namespace std;


struct webserver {
  int port_number;
  int server_socket;
  int current_tick;

  pollfd server_file_descriptor[1];

  enum status {
    OFF,
    ON,
    KILL,
  } server_status;

  const static int QUEUE_LIMIT = 5;
  const static int TICK_LIMIT = 600;

  webserver(int port_number) {
    this->server_status = OFF;

    this->port_number = port_number;
    this->server_socket = socket(AF_INET, SOCK_STREAM, 0); // TODO: magic number
    this->current_tick = 0;
  }

  void setup(string path_to_mappings, string path_to_site) {}

  void start() {
    sockaddr_in server_address = {AF_INET, htons(this->port_number), INADDR_ANY};
    bind(this->server_socket, (struct sockaddr*)&server_address, sizeof(server_address));
    listen(this->server_socket, QUEUE_LIMIT);

    this->server_status = ON;

    this->server_file_descriptor[0].fd = this->server_socket;
    this->server_file_descriptor[0].events = POLLIN;
  }

  void tick(int poll_interval) {
    if (this->current_tick++ >= TICK_LIMIT) this->server_status = KILL;
    
    int event_count = poll(this->server_file_descriptor, 1, poll_interval); // TODO: magic number
    if (event_count <= 0) return;
    else {
      int client_socket = accept(this->server_socket, nullptr, nullptr);
      
      char buffer[2048] = {0};
      recv(client_socket, buffer, sizeof(buffer), 0);
      
      string response_string = "TODO";
      send(client_socket, response_string.c_str(), response_string.length(), 0);
      close(client_socket);
    }
  }

  void quit() {
    close(this->server_socket);
  }
};



