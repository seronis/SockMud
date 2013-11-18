#ifndef SESSION_H
#define SESSION_H

#include <string>

#include "socket.h"

class SessionData
{
	public:
		enum state{ intro_splash, login_nameget, login_nameck, login_passget, login_passck, login_complete };
		SessionData();
		~SessionData();
	private:
		std::string acctname;
		std::string nickname;
};

class Session
{
	public:
		Session(Socket * a_sock);
		~Session();

		static void SetSplash( std::string msg );
	private:
		static std::string splash;
		SessionData::state state;
		Socket * m_pSocket;
		std::string acctname;	//used as the 'login'. not visible to players
		std::string nickname;	//visible to other players
};

#endif // SESSION_H
