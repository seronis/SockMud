#include "session.h"

Session::Session(Socket * a_sock)
{
	this->m_pSocket = a_sock;
	this->state = SessionData::intro_splash;
}

Session::~Session()
{
	//dtor
}

std::string Session::splash("Hi all!!!\n");
void Session::SetSplash(std::string msg)
{
	Session::splash = msg;
}
