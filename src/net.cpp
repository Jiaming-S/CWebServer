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

  packet* append_message (string str, string delimiter) {
    if (body_len == 0) {
      this->content.append(delimiter);
      this->header_len++;
    }

    this->content.append(str).append(delimiter);
    this->body_len += str.length() + 2;

    return this;
  }

  packet* pack(string filepath) {
    ifstream fin(filepath);
    if (!fin.is_open()) return nullptr;

    while (fin) {
      string cur_line;
      getline(fin, cur_line);
      this->append_message(cur_line, CRLF);
    }

    return this;
  }

  packet* pack_image(string image_filepath) {
    ifstream fin(image_filepath);
    if (!fin.is_open()) return nullptr;

    // check for PNG signature
    if (fin.peek() != 0x89) {
      cout << "Packing PNG Failed: " << fin.peek() << " != 89" << endl;
      return nullptr;
    }

    while (fin) {
      string cur_line;
      getline(fin, cur_line);
      this->append_message(cur_line, LF);
    }

    return this;
  }

  string get_content() {
    return this->content;
  }
};

