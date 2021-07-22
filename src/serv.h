#include <sys/socket.h> // For socket functions
#include <netinet/in.h> // For sockaddr_in
#include <cstdlib>      // For exit() and EXIT_FAILURE
#include <iostream>     // For cout
#include <unistd.h>     // For read
#include <string>
#include <map>
#include <vector>
#include <fstream>

#pragma once

using std::string;
using std::vector;

class Res
{
public:
  Res(Serv *){}

  void newConnection(int, string){}

  void send(string){}

  void send_file(string){}

  void setContentType(string){}
private:
  int _Connection;
  Serv *_Server;
  string ContentType;
  string Type = "text/plain";
  string Path;
};

typedef void (*ResFunction)(void);

using std::string;
using std::vector;

vector<string> SplitStr(string String, char delimeter)
{
  vector<string> Words = {};
  string Word = "";
  for (auto x : String)
  {
    if (x == delimeter)
    {
      Words.push_back(Word);
      Word = "";
    }
    else
    {
      Word = Word + x;
    }
  }
  return Words;
}


using std::string;
using std::vector;

#define BUFFER_SIZE 4096

int newSocket(int port)
{
  // Create a socket (IPv4, TCP)
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1)
  {
    std::cout << "Failed to create socket. errno: " << errno << std::endl;
    exit(EXIT_FAILURE);
  }

  // Listen to port 9999 on any address
  sockaddr_in sockaddr;
  sockaddr.sin_family = AF_INET;
  sockaddr.sin_addr.s_addr = INADDR_ANY;
  sockaddr.sin_port = htons(8080); // htons is necessary to convert a number to
                                   // network byte order
  if (bind(sockfd, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) < 0)
  {
    std::cout << "Failed to bind to port 8080. errno: " << errno << std::endl;
    exit(EXIT_FAILURE);
  }

  // Start listening. Hold at most 10 connections in the queue
  if (listen(sockfd, 10) < 0)
  {
    std::cout << "Failed to listen on socket. errno: " << errno << std::endl;
    exit(EXIT_FAILURE);
  }

  return sockfd;
}

class Serv
{
public:
  Serv(int port)
  {
    this->Socket = newSocket(port);
    this->Sockaddr_Len = sizeof(this->Socketaddr);
    getpeername(STDIN_FILENO, &this->Socketaddr, &this->Sockaddr_Len);
    this->Response = Res(this);
  }

  void Get(string Path, ResFunction Func)
  {
    GetPathes.insert(std::make_pair(Path, Func));
  }

  void Start()
  {
    while (true)
    {
      int connection = accept(Socket, (struct sockaddr *)&this->Socketaddr, (socklen_t *)&this->Sockaddr_Len);
      if (connection < 0)
      {
        std::cout << "Failed to grab connection. errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
      }

      char buffer[BUFFER_SIZE];
      read(connection, buffer, 100);
      vector<string> Split = SplitStr(buffer, ' ');

      std::string Message = "<h1>HI<h1>";
      int Len = Message.size();
      std::string msg = "HTTP/1.0 200 OK\nServer: Matserver v0.1\nContent-Type: text/html\nContent-Length: " + std::to_string(Len) + "\n\n" + Message;

      std::cout << msg;
    }
  }

  static void Send(int __fd, const void *__buf, size_t __n)
  {
    send(__fd, __buf, __n, 0);
  }

  string GetFile(string path)
  {
    if (Files.find(path) != Files.end())
    {
      std::fstream file = std::fstream(path);

      string text = "";

      while (file.good())
        text += file.get();

      file.close();

      Files.insert(std::make_pair(path, text));

      return text.substr(0, text.length() - 1);
    }
    else
    {
      return Files.find(path)->second;
    }
  }

private:
  int Socket;
  struct sockaddr Socketaddr;
  socklen_t Sockaddr_Len;
  std::map<string, ResFunction> GetPathes = {};
  std::map<string, ResFunction> PostPathes = {};
  std::map<string, string> Files = {};
  Res Response;
};