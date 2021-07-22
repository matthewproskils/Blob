#pragma once
#include "serv.h"

class Res
{
public:
  Res(Serv *Server)
  {
    this->_Server = Server;
  }

  void newConnection(int Connection, string Buffer)
  {
    this->_Connection = Connection;
    vector<string> Split = SplitStr(Buffer, ' ');

    this->Type = Split[0];
    this->Path = Split[1];
  }

  void send(string Msg)
  {
    _Server->Send(_Connection, Msg.c_str(), Msg.size());
  }

  void send_file(string Path)
  {
    string Msg = _Server->GetFile(Path);
    _Server->Send(_Connection, Msg.c_str(), Msg.size());
  }

  void setContentType(string Type)
  {
    this->ContentType = Type;
  }

private:
  int _Connection;
  Serv *_Server;
  string ContentType;
  string Type = "text/plain";
  string Path;
};


