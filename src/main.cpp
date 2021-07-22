#include "server_init.hpp"

int main()
{
  Serv *Server = new Serv(8080);
  Server->Start();
}

// std::string msg = "HTTP/1.0 200 OK\nServer: Matserver v0.1\nContent-Type: text/html\nContent-Length: " + std::to_string(Len) + "\n\n" + Message;