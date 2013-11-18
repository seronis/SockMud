// ****************************************************************************
// SocketMud II Copyright 2004 Brian Graversen
// ****************************************************************************
// Revision History
// ----------------
// 19/01/04) Initial SocketMud II code released
// ****************************************************************************
// This product can be used freely as long as this copyright header remains
// intact. This header may not be removed or changed, and any work released
// based on this code must contain this header information in all files.
// ****************************************************************************

#include <string>
#include <iostream>

#include "server.h"
#include "session.h"

int main( int argc, char** argv )
{
	int port;

	switch( argc ) {
	case 2:
		port = std::stoi( argv[1] );

		if( port < 1024 || port > 9999 ) {
			printf( "[port] must be between 1024 and 9999\n" );
			return 1;
		}

		break;

	default:
		printf( "Syntax: %s [port]\n", argv[0] );
		return 1;
	}

	Server *mServer = new Server();
	if( mServer->Listen( port ) == false )
		return 1;

	std::string str_splash;
	str_splash += "Welcome to the server.\n";
	str_splash += " - \n";
	str_splash += " and stuff ...\n\n";
	Session::SetSplash(str_splash);

	while( 1 ) {
		// poll all sockets for incoming data and buffer
		// also manages accepting 1 new connection per call
		mServer->PollSockets();

		std::list<Session*> sessionList = mServer->GetSessionList();
		std::list<Session*>::iterator iSession;
		Session *pSession;

		// echo everything that each socket has sent to us
		for( iSession = sessionList.begin(); iSession != sessionList.end(); ) {
			pSession = *iSession++;

		//	pSocket->
		//	pSocket->Write( pSocket->GetInBuffer() );
		//	pSocket->ClrInBuffer();
		}

		// flush all outgoing data
		mServer->FlushSockets();

		// sleep the rest of the pulse
		mServer->Sleep( 5 );
	}

	return 1;
}
