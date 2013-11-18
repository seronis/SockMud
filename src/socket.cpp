
// c headers
#include <errno.h>
#include <unistd.h>

// local headers
#include "socket.h"

Socket::Socket( int _id )
{
	sockid = _id;
}

Socket::~Socket()
{
	if( sockid != -1 )
		close( sockid );
}

int Socket::GetSocketID()
{
	return sockid;
}

bool Socket::IsOpen()
{
	return sockid != -1;
}

bool Socket::Read()
{
	char temp[4096 + 2];
	int size;

	while( true ) {
		if( ( size = read( sockid, temp, 4096 ) ) > 0 ) {

			temp[size] = '\0';
			inBuffer += ( std::string ) temp;

			if( temp[size - 1] == '\n' || temp[size - 1] == '\r' )
				break;
		} else if( size == 0 )
			return false;
		else if( errno == EAGAIN || size == 4096 )
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
		if( ( w = write( sockid, outBuffer.c_str(), b ) ) == -1 )
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
	if( this->sockid != -1 ) {
		close( this->sockid );
		this->sockid = -1;
	}
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
