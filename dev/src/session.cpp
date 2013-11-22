
#include <string>
#include <iostream>

#include "server.h"
#include "session.h"

Session::Session(Socket * a_sock)
{
	this->m_pSocket = a_sock;
	this->m_pServer = a_sock->GetServer();
	this->state = SessionData::intro_splash;
}

Session::~Session()
{
	if( this->m_pSocket != nullptr ) {
		if( m_pSocket->IsOpen() ) {
			//TODO: proper logging
			std::cerr << "ERROR: session being destructed while socket still open.\n";
			m_pServer->CloseSocket(m_pSocket);
		}
		delete m_pSocket;
	}
}

std::string Session::splash("Hi all!!!\n");
void Session::SetSplash(std::string msg)
{
	Session::splash = msg;
}

void Session::Write( std::string _str )
{
	m_pSocket->Write(_str);
}

void Session::Process()
{
	//grab all read data from socket and buffer it locally
	this->inbuffer += this->m_pSocket->GetInBuffer();
	this->m_pSocket->ClrInBuffer();

	//extract one 'line' from local buffer if we dont have one already
	if( last_cmd.empty() ) {
		size_t firstNL = this->inbuffer.find('\n');
		if( firstNL >= 0 ) {
			last_cmd = inbuffer.substr(0,firstNL);
			inbuffer.erase(0,firstNL);
		}
	}

	//actually process command
	switch( this->state )
	{

	case SessionData::intro_splash:
		//TODO: check for banned status. Disconnect or send Splash as appropriate
		m_pSocket->Write(Session::splash);
		state = SessionData::login_nameget;
		return;

	case SessionData::login_nameget:
		m_pSocket->Write("Please enter your login name: ");
		state = SessionData::login_nameck;
		return;

	case SessionData::login_nameck:
		{
			if( last_cmd.empty() )	return;

			std::string msg = "\nHi -USERNAME-, we are gonna drop you into Echo mode now.  Enjoy!!\n";
			int pos = msg.find("USERNAME");
			msg.replace( pos, 8, last_cmd);
			m_pSocket->Write(msg);
			state = SessionData::login_complete;
			last_cmd.clear();
			return;
		}

	case SessionData::login_complete:
	default:
		{
			if( last_cmd.empty() ) return;

			Session *pSession;
			std::list<Session*>::iterator iSession;
			std::list<Session*> sessionList = m_pServer->GetSessionList();
			for( iSession = sessionList.begin(); iSession != sessionList.end(); ) {
				pSession = *iSession++;
				std::string msg;
				msg += "(";
				msg += std::to_string(last_cmd.length());
				msg += ") ";
				msg += last_cmd;
				pSession->Write(msg);
			}
			last_cmd.clear();
			break;
		}
	}
	return;
}
