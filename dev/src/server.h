
#ifndef HEADER_SERVER
#define HEADER_SERVER

// c++ headers
#include <list>

// c headers
#include <arpa/inet.h>

// local headers
#include "socket.h"
#include "session.h"

class Server
{
public:
	Server( void );
	~Server( void );

	bool				Listen( int port );
	void				SetTicksPerSecond( int tps );
	void				FlushSockets( void );
	bool				PollSockets( void );
	void				Sleep( void );
	std::list<Socket*>	GetSocketList( void );
	std::list<Session*>	GetSessionList( void );
	void				CloseSocket( Socket *pSocket );

private:
	void                Accept( void );

	std::list<Socket*>	socketList;
	std::list<Session*>	sessionList;
	int                 listenersock_id;
	fd_set              fSet;
	fd_set              rSet;
	sockaddr_in         my_addr;
	struct timeval      lastSleep;
	int					tps_rate;
};

#endif
