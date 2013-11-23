#include <string>
#include <iostream>

#include "server.h"
#include "session.h"

Session::Session( Socket * a_sock )
{
	this->m_pSocket = a_sock;
	this->m_pServer = a_sock->GetServer();
	this->state = SessionData::intro_splash;
	this->eol = "\n";
}

Session::~Session()
{
	if( this->m_pSocket != nullptr ) {
		if( m_pSocket->IsOpen() ) {
			//TODO: proper logging
			std::cerr << "BUG: session being destructed while socket still open.\n";
			m_pServer->CloseSocket( m_pSocket );
		}

		delete m_pSocket;
	} else {
		//TODO: proper logging
		std::cerr << "BUG: session being destructed with no socket attached.\n";
	}
}

std::string Session::splash( "Hi all!!!\n" );
void Session::SetSplash( std::string msg )
{
	Session::splash = msg;
}

void Session::Write( std::string _str )
{
	if( m_pSocket->IsOpen() )
		m_pSocket->Write( _str );
}

void Session::Process()
{
	//grab all read data from socket and buffer it locally
	this->inbuffer += this->m_pSocket->GetInBuffer();
	this->m_pSocket->ClrInBuffer();

	//extract one 'line' from local buffer if we dont have one already
	bool cmdFound = true;

	if( last_cmd.empty() ) {
		size_t firstNL = this->inbuffer.find( this->eol );

		if( firstNL != std::string::npos ) {
			last_cmd = inbuffer.substr( 0, firstNL );
			size_t endNL = firstNL + this->eol.length();
			inbuffer.erase( 0, endNL );
		} else
			cmdFound = false;
	}

	//actually process command
	switch( this->state ) {

	case SessionData::intro_splash: {
			//TODO: check for banned status. Disconnect or send Splash as appropriate
			m_pSocket->Write( Session::splash );
			m_pSocket->Write( "\nPress [enter] to continue: \n" );
			state = SessionData::login_detect_eol;
			return;
		}

	case SessionData::login_detect_eol: {
			if( cmdFound == false ) {
				//TODO: timeout after 30 seconds and disconnect socket
				return;
			}

			size_t CREOL = this->last_cmd.find( '\r' );

			if( CREOL != std::string::npos ) {
				this->eol = "\r\n";
			}

			last_cmd.clear();
			state = SessionData::login_nameget;
			return;
		}

	case SessionData::login_nameget: {
			m_pSocket->Write( "Please enter your login name: " );
			state = SessionData::login_nameck;
			return;
		}

	case SessionData::login_nameck: {
			if( cmdFound == false ) {
				//TODO: timeout after 30 seconds and disconnect socket
				return;
			}

			std::string msg = "\nHi -USERNAME-, we are gonna drop you into Echo mode now.  Enjoy!!\n";
			int pos = msg.find( "USERNAME" );
			msg.replace( pos, 8, last_cmd );
			m_pSocket->Write( msg );
			state = SessionData::login_complete;
			last_cmd.clear();
			return;
		}

	default: {
			//NOTE: the default case currently has no break so it 'falls through' to
			//	the login_complete state for now. We still need to realize we forgot
			//	to properly handle a connected state that we assigned somewhere.

			//TODO: proper logging
			std::cerr << "BUG: pSession->Process() detected unhandled state.\n";
			//assign state so that the bug msg only displays once per bad socket
			state = SessionData::login_complete;
		}
	case SessionData::login_complete: {
			if( cmdFound == false ) {
				return;
			}

			this->Write( last_cmd + "\n" );

			last_cmd.clear();
			break;
		}
	}

	return;
}
