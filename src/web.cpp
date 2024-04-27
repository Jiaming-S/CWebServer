// web.cpp
// Represents the website to be served and builds ontop of main


#include <iostream>
#include <string>

#define GET_HTML 1
#define GET_JS 2
#define GET_CSS 3
#define GET_OTHER 0
#define UNKNOWN_METHOD -1

#include "net.cpp"

using namespace std;


int classify_request(char request[]) {
  string headers = string(request);
  string start_line = headers.substr(0, headers.find(CRLF));
  
  reql cur_request;
  int split_1 = start_line.find_first_of(' ');
  int split_2 = start_line.find_last_of(' ');
  cur_request.method   = start_line.substr(0, split_1);
  cur_request.target   = start_line.substr(split_1 + 1, split_2 - split_1 - 1);
  cur_request.http_ver = start_line.substr(split_2 + 1, start_line.length() - split_2 - 1);

  if (cur_request.method == "GET") {
    if (cur_request.target == "/") {
      return GET_HTML;
    }
    else {
      cout << "Unknown Target: " << cur_request.target << endl;
      return GET_OTHER;
    }
  }
  else {
    cout << "Unknown Method: " << cur_request.method << endl;
    return UNKNOWN_METHOD;
  }
}

string webserver(int client_socket, char request[], size_t request_size) {
  packet* result_packet = new packet;

  cout << endl << "Incoming Request..." << endl;
  int request_code = classify_request(request);
  cout << "Code: " << request_code << endl;

  if (request_code == GET_HTML) {
    result_packet
      ->append_header("HTTP/1.1 200 OK")
      ->append_header("Content-Type: text/html; charset=utf-8")
      ->pack("./assets/site.html");
  }

  return result_packet->get_content();
}



