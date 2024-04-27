// web.cpp
// Represents the website to be served and builds ontop of main


#include <string>

using namespace std;


int classify_request(char request[], size_t request_size) {
  
}

string webserver(int client_socket, char request[], size_t request_size) {
  packet* result_packet = new packet;

  if (classify_request(request, request_size) == 1) {
    result_packet
      ->append_header("HTTP/1.1 200 OK")
      ->append_header("Content-Type: text/html; charset=utf-8")
      ->append_body("<!DOCTYPE html>")
      ->append_body("<body>")
      ->append_body("<main>")
      ->append_body("<h1>Example Page</h1>")
      ->append_body("<p>Hi</p>")
      ->append_body("</main>")
      ->append_body("</body>");  
  }

  return result_packet->get_content();
}



