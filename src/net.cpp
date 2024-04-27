// net.cpp
// Provides helper structs and functions for main 

#include <string>

#define CRLF "\r\n"
#define LF   "\n"

using namespace std;


struct packet {
  string content;

  size_t header_len;
  size_t body_len;

  packet () {
    this->content = string();
    this->header_len = 0;
    this->body_len = 0;
  } 

  packet* append_header (string str) {
    string header = this->content.substr(0, header_len);
    string body = this->content.substr(header_len, this->content.length());

    this->content = header.append(str).append(CRLF).append(body);
    this->header_len += str.length() + 2;
    
    return this;
  }

  packet* append_body (string str) {
    if (body_len == 0) {
      this->content.append(CRLF);
      this->header_len++;
    }

    this->content.append(str);
    this->body_len += str.length();

    return this;
  } 

  string get_content() {
    return this->content;
  }
};
