// web.cpp
// Represents the website to be served and builds ontop of main


#include <iostream>
#include <string>
#include <map>

#include "net.cpp"

using namespace std;

map<string, string> request_to_path, request_to_filetype;

void build_request_cache() {
  ifstream fin("./mapping.txt");
  if (!fin.is_open()) return;

  while (fin) {
    string cur_line;
    getline(fin, cur_line);
    if (cur_line[0] == '#') continue;

    int delim = cur_line.find('=');

    string req = cur_line.substr(0, delim);
    string mapping = cur_line.substr(delim + 1);
    string path = mapping.substr(0, mapping.find(';'));
    string filetype = mapping.substr(mapping.find(';') + 1);
    
    cout << "Built Request Path Pair: " << req << " " << path << endl;
    cout << "Built Request Filetype Pair: " << req << " " << filetype << endl;

    request_to_path.insert(make_pair(req, path));
    request_to_filetype.insert(make_pair(req, filetype));
  }
}

reql* read_request(char request[], size_t request_size) {
  string headers = string(request);
  string start_line = headers.substr(0, headers.find(CRLF));
  
  reql* cur_request = new reql();
  int split_1 = start_line.find_first_of(' ');
  int split_2 = start_line.find_last_of(' ');
  
  cur_request->method   = start_line.substr(0, split_1);
  cur_request->target   = start_line.substr(split_1 + 1, split_2 - split_1 - 1);
  cur_request->http_ver = start_line.substr(split_2 + 1);

  return cur_request;
}

string webserver(int client_socket, char request[], size_t request_size) {
  cout << endl << "Incoming Request..." << endl;

  reql* request_line = read_request(request, request_size);
  cout << request_line->method << " " << request_line->target << endl;


  packet* result_packet = new packet;
  if (request_line->method == "GET") {
    if (request_to_path.find(request_line->target) == request_to_path.end()) {
      cout << "Request Target Not Found." << endl;
      result_packet->append_header("HTTP/1.1 404 Not Found");
    }
    else {
      result_packet
        ->append_header("HTTP/1.1 200 OK")
        ->append_header("Content-Type: " + request_to_filetype[request_line->target])
        ->pack(request_to_path[request_line->target]);
    }
  }
  else {
    cout << "Method Not Supported: " << request_line->method << endl;
    result_packet->append_header("HTTP/1.1 404 Not Found");
  }

  return result_packet->get_content();
}

