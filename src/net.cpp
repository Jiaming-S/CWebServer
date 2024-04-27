// net.cpp
// Provides helper structs and functions for web 

#include <string>
#include <fstream>

#define CRLF "\r\n"
#define LF   "\n"
#define DELIMITER "\r\n\r\n"

using namespace std;


struct reql {
  string method;
  string target;
  string http_ver;
};


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

  packet* append_message (string str) {
    if (body_len == 0) {
      this->content.append(CRLF);
      this->header_len++;
    }

    this->content.append(str).append(CRLF);
    this->body_len += str.length() + 2;

    return this;
  }

  void pack(string filepath) {
    ifstream fin(filepath);
    if (!fin.is_open()) return;

    while (fin) {
      string cur_line;
      getline(fin, cur_line);
      this->append_message(cur_line);
    }
  }

  string get_content() {
    return this->content;
  }
};

