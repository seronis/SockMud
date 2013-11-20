
// c headers
#include <errno.h>
#include <unistd.h>

// local headers
#include "socket.h"

Socket::Socket( int a_id, Server * a_pServer )
{
	clientsockid = a_id;
	m_pServer = a_pServer;
}

Socket::~Socket()
{
	if( clientsockid != -1 )
		close( clientsockid );
}

int Socket::GetSocketID() const
{
	return clientsockid;
}

bool Socket::IsOpen() const
{
	return clientsockid != -1;
}

bool Socket::Read()
{
	char temp[4096 + 2];
	int size;

	while( true ) {
		if( ( size = read( clientsockid, temp, 4096 ) ) > 0 ) {

			temp[size] = '\0';
			inBuffer += ( std::string ) temp;

			if( temp[size - 1] == '\n' || temp[size - 1] == '\r' )
				break;
		} else if( size == 0 )
			return false;
		else if( errno == EAGAIN )
			break;
		else
			return false;
	}

	return true;
}

void Socket::Write( std::string txt )
{
	outBuffer += txt;
}

bool Socket::Flush()
{
	int b, w;

	// empty the entire buffer
	while( outBuffer.length() > 0 ) {
		b = ( outBuffer.length() < 4096 ) ? outBuffer.length() : 4096;

		// any write failures ?
		if( ( w = write( clientsockid, outBuffer.c_str(), b ) ) == -1 )
			return false;

		// move the buffer down
		outBuffer.erase( 0, w );
	}

	return true;
}

void Socket::Close()
{
	ClrInBuffer();
	ClrOutBuffer();
	if( this->clientsockid != -1 ) {
		close( this->clientsockid );
		this->clientsockid = -1;
	}
}

Server * Socket::GetServer()
{
	return m_pServer;
}

std::string Socket::GetInBuffer()
{
	return inBuffer;
}

void Socket::ClrInBuffer()
{
	inBuffer.erase( 0, inBuffer.length() );
}

void Socket::ClrOutBuffer()
{
	outBuffer.erase( 0, outBuffer.length() );
}
