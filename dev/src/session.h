#ifndef SESSION_H
#define SESSION_H

#include <string>

#include "socket.h"

class SessionData
{
public:
	enum state { intro_splash, login_detect_eol,
				 login_nameget, login_nameck,
				 login_passget, login_passck,
				 login_complete
			   };
	SessionData();
	~SessionData();
private:
	std::string acctname;
	std::string nickname;
	std::string password;
};

class Session
{
public:
	Session( Socket * _sock );
	~Session();

	static void SetSplash( std::string _msg );
	void Write( std::string _str );
	void Process( void );

	Socket * m_pSocket;
	Server * m_pServer;

private:
	static std::string splash;
	SessionData::state state;
	std::string acctname;	//used as the 'login'. not visible to players
	std::string nickname;	//visible to other players
	std::string inbuffer;	//entire input so far received from socket
	std::string last_cmd;	//latest 'line' popped from inbuffer
	std::string eol;		//eol sequence used by client
};

#endif // SESSION_H
