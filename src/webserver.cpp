// webserver.cpp
// Represents a locally hosted webserver that responds to http requests

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <poll.h>

using namespace std;

#define QUEUE_LIMIT 5
#define TICK_LIMIT 600
#define RECV_BUFFER_SIZE 2048

bool SIG_RECEIVED = false;

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

  map<string, string> request_to_path, request_to_filetype, request_to_content;
  

  webserver(int port_number) {
    this->server_status = OFF;

    this->port_number = port_number;
    this->server_socket = socket(AF_INET, SOCK_STREAM, 0); // TODO: magic number
    this->current_tick = 0;

    signal(SIGINT, &webserver::sighandler);
  }

  void load(string path_to_mappings_txt, string path_to_site_dir) {
    ifstream fin(path_to_mappings_txt);
    if (!fin.is_open()) return;

    while (fin) {
      string cur_line;
      getline(fin, cur_line);
      if (cur_line[0] == '#') continue;
      if (cur_line.length() <= 0) continue;

      int delim = cur_line.find('=');

      string req = cur_line.substr(0, delim);
      string mapping = cur_line.substr(delim + 1);
      string path = mapping.substr(0, mapping.find(';'));
      string filetype = mapping.substr(mapping.find(';') + 1);

      request_to_path.insert(make_pair(req, path));
      cout << " * Built Request Path Pair:     " << req << " --> " << path << endl;

      request_to_filetype.insert(make_pair(req, filetype));
      cout << " * Built Request Filetype Pair: " << req << " --> " << filetype << endl;

      request_to_content.insert(make_pair(req, flatten_file(path, "\n")));
      cout << " * Built Request Content Pair:  " << req << " --> Content With " << request_to_content[req].length() << " Bytes" << endl;
    }
  }

  void start() {
    sockaddr_in server_address = {AF_INET, htons(this->port_number), INADDR_ANY};
    bind(this->server_socket, (struct sockaddr*)&server_address, sizeof(server_address));
    listen(this->server_socket, QUEUE_LIMIT);

    this->server_status = ON;

    this->server_file_descriptor[0].fd = this->server_socket;
    this->server_file_descriptor[0].events = POLLIN;
  }

  void tick(int poll_interval) {
    if (this->current_tick++ >= TICK_LIMIT || SIG_RECEIVED) this->server_status = KILL;
    
    int event_count = poll(this->server_file_descriptor, 1, poll_interval); // TODO: magic number
    if (event_count <= 0) return;
    else {
      cout << " * New Request Received" << endl;
      int client_socket = accept(this->server_socket, nullptr, nullptr);
      
      char buffer[RECV_BUFFER_SIZE] = {0};
      recv(client_socket, buffer, sizeof(buffer), 0); // TODO: magic number
      string request_packet = string(buffer);

      string reql = extract_request_line(request_packet);
      string content = extract_request_contents(request_packet);

      int split_1 = reql.find_first_of(' ');
      int split_2 = reql.find_last_of(' ');
      
      string req_method = reql.substr(0, split_1);
      string req_target = reql.substr(split_1 + 1, split_2 - split_1 - 1);

      string response_string = string();

      if (req_method != "GET" || req_target.length() <= 0) response_string = "HTTP/1.1 400 Bad Request";
      else {
        response_string = "HTTP/1.1 200 OK\n";
        response_string.append("Content-Type: " + request_to_filetype[req_target] + "\n");
        response_string.append(request_to_content[req_target] + "\n");
      }

      send(client_socket, response_string.c_str(), response_string.length(), 0);
      close(client_socket);
    }
  }

  string extract_request_line(string request_packet) {
    return request_packet.substr(0, request_packet.find("\r\n"));
  }

  string extract_request_contents(string request_packet) {
    return request_packet.substr(request_packet.find("\r\n"));
  }

  string flatten_file(string filepath, string end) {
    ifstream fin(filepath);
    if (!fin.is_open()) return nullptr;

    string output = "";

    while (fin) {
      string cur_line;
      getline(fin, cur_line);
      output.append(cur_line + end);
    }

    return output;
  }

  void quit() {
    close(this->server_socket);
  }

  static void sighandler(int signum) {
    cout << endl << " * Received SIGINT: (" << signum << ")" << endl;
    SIG_RECEIVED = true;
  }
};



