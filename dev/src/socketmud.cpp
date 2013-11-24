#include <string>
#include <iostream>

#include "server.h"
#include "session.h"

bool gameClosing = false;

int main( int argc, char** argv )
{
	int port;

	switch( argc ) {
	case 2:
		port = std::stoi( argv[1] );

		if( port < 1024 || port > 65535 ) {
			std::printf( "\n[port] must be between 1024 and 65535\n" );
		} else break;

	default:
		std::printf( "\n  Syntax: %s [port]\n", argv[0] );
		return 1;
	}

	Server *mServer = new Server();

	if( mServer->Listen( port ) == false )
		return 1;

	mServer->SetTicksPerSecond( 5 );

	std::string str_splash;
	str_splash += "Welcome to the server.\n";
	str_splash += "  - \n";
	str_splash += "     and stuff ...\n\n";
	Session::SetSplash( str_splash );

	while( !gameClosing || !mServer->GetSocketList().empty() ) {
		// Poll all sockets for incoming data and buffer.
		// Also manages accepting 1 new connection per call from listener.
		mServer->PollSockets();

		Session *pSession;
		std::list<Session*>::iterator iSession;
		std::list<Session*> sessionList = mServer->GetSessionList();

		for( iSession = sessionList.begin(); iSession != sessionList.end(); ) {
			pSession = *iSession++;
			pSession->Process();
		}

		//send any pending output to all connected sockets
		mServer->FlushSockets();

		//sleeps until the next time interval determined from our desired 'ticks per second' rate
		mServer->Sleep();
	}

	std::printf( "\nERROR: Game over man.. GAME OVER!!!\n" );
	return 0;
}
