
//library headers
#include <cstring>
#include <unistd.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <arpa/inet.h>

// local headers
#include "server.h"

Server::Server()
{
	listenersock_id = -1;

	FD_ZERO( &fSet );
	FD_ZERO( &rSet );

	memset( &my_addr, 0, sizeof( my_addr ) );

	// initialize lastSleep
	gettimeofday( &lastSleep, NULL );
}

Server::~Server()
{
	// close the connection if it is active
	if( listenersock_id != -1 )
		close( listenersock_id );
}

bool Server::Listen( int port )
{
	// try to create a communications endpoint
	if( ( listenersock_id = socket( AF_INET, SOCK_STREAM, 0 ) ) == -1 )
		return false;

	// set options for this control socket
	int reuse = 1;
	if( setsockopt( listenersock_id, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof( int ) ) == -1 )
		return false;

	// attach the control socket to it's own filedescriptor set
	FD_SET( listenersock_id, &fSet );

	// settings for this socket, (and set the listning port)
	my_addr.sin_family      = AF_INET;
	my_addr.sin_addr.s_addr = INADDR_ANY;
	my_addr.sin_port        = htons( port );

	// try to bind the mud port
	if( bind( listenersock_id, ( struct sockaddr * ) &my_addr, sizeof( my_addr ) ) == -1 )
		return false;

	// start listening
	if( listen( listenersock_id, 3 ) == -1 )
		return false;

	return true;
}

bool Server::PollSockets()
{
	// copy the permanent descriptor set
	memcpy( &rSet, &fSet, sizeof( fd_set ) );

	// poll the descriptor set
	static struct timeval tv;
	if( select( FD_SETSIZE, &rSet, NULL, NULL, &tv ) < 0 )
		return false;

	// attempt to establish new connections
	Accept();

	// iterate through all sockets and read pending data
	Socket *pSocket;
	std::list<Socket*>::iterator iSocket;
	for( iSocket = socketList.begin(); iSocket != socketList.end(); ) {
		pSocket = *iSocket++;

		// attempt to read from this socket if pending incoming data
		if( FD_ISSET( pSocket->GetSocketID(), &rSet ) ) {

			// if read fails, close the connection
			if( pSocket->Read() == false )
				CloseSocket( pSocket );
		}
	}

	return true;
}

void Server::FlushSockets()
{
	std::list<Socket*>::iterator iSocket;
	Socket *pSocket;

	// iterate through all sockets and flush outgoing data
	for( iSocket = socketList.begin(); iSocket != socketList.end(); ) {
		pSocket = *iSocket++;

		// Attempt to flush this socket, close socket if failure
		if( pSocket->Flush() == false )
			CloseSocket( pSocket );
	}
}

// Sleep() should be called with an argument that divided 1000,
// like 4, 5, 8 or 10. This is the amount of "commands" that will
// be processed each second, and it is recommended to have a
// constant defined for this purpose.
void Server::Sleep( int tps )
{
	//how many ticks per second do we want?
	if( tps <= 0 )	return;

	struct timeval newTime;
	gettimeofday( &newTime, NULL );

	// calculate exact amount of time we need to sleep
	int secs, usecs;
	usecs = ( int )( lastSleep.tv_usec -  newTime.tv_usec ) + 1000000 / tps;
	secs  = ( int )( lastSleep.tv_sec  -  newTime.tv_sec );

	while( usecs < 0 ) {
		usecs += 1000000;
		secs  -= 1;
	}

	while( usecs >= 1000000 ) {
		usecs -= 1000000;
		secs  += 1;
	}

	// sleep if needed
	if( secs > 0 || ( secs == 0 && usecs > 0 ) ) {
		struct timeval sleepTime;

		sleepTime.tv_usec = usecs;
		sleepTime.tv_sec  = secs;

		select( 0, NULL, NULL, NULL, &sleepTime );
	}

	// remember when we last slept
	gettimeofday( &lastSleep, NULL );
}

void Server::CloseSocket( Socket *pSocket )
{
	// ensure server is no longer tracking socket.
	socketList.remove( pSocket );
	FD_CLR( pSocket->GetSocketID(), &fSet );

	// at this point socket is only tracked by its owning session.
	// have it do a friendly shutdown and flag itself for safe deletion.
	pSocket->Close();
}

void Server::Accept()
{
	//did we find a connection pending acceptance while polling?
	if( !FD_ISSET( listenersock_id, &rSet ) )
		return;

	int clientsockid;
	int len = sizeof( my_addr );
	if( ( clientsockid = accept( listenersock_id, ( struct sockaddr * ) &my_addr, ( socklen_t * ) &len ) ) == -1 )
		return;

	// allocate a new socket and set non-blocking I/O
	int opt_on = 1;
	Socket * pSocket = new Socket( clientsockid );
	ioctl( clientsockid, FIONBIO, &opt_on );

	// allocate a new session and turn ownership of the socket over to it
	Session * pSession = new Session(pSocket);

	// attach to lists
	socketList.push_back( pSocket );
	sessionList.push_back( pSession );

	// attach to file descriptor set
	FD_SET( clientsockid, &fSet );
}

std::list<Socket*> Server::GetSocketList()
{
	return socketList;
}

std::list<Session*> Server::GetSessionList()
{
	return sessionList;
}
