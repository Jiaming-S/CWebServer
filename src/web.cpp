// web.cpp
// Represents the website to be served and builds ontop of main


#include <string>

#include "net.cpp"
#include "pack.cpp"

#define GET_HTML 1
#define GET_JS 2
#define GET_CSS 3
#define GET_OTHER 0
#define UNKNOWN_REQUEST -1

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
      return GET_OTHER;
    }
  }
  else {
    return UNKNOWN_REQUEST;
  }
}

string webserver(int client_socket, char request[], size_t request_size) {
  packet* result_packet = new packet;

  int request_code = classify_request(request);
  cout << "Incoming Request with code: " << request_code << endl;

  if (request_code == GET_HTML) {
    result_packet
      ->append_header("HTTP/1.1 200 OK")
      ->append_header("Content-Type: text/html; charset=utf-8")
      ->append_message("<!DOCTYPE html>")
      ->append_message("<body>")
      ->append_message("<main>")
      ->append_message("<h1>Example Page</h1>")
      ->append_message("<p>Hi</p>")
      ->append_message("</main>")
      ->append_message("</body>");
  }

  return result_packet->get_content();
}



