
#ifndef HEADER_SOCKET
#define HEADER_SOCKET

// c++ headers
#include <string>

class Server;

class Socket
{
public:
	Socket( int desc, Server * a_pServer );
	~Socket( void );

	bool			Read( void );
	void			Write( std::string txt );
	bool			Flush( void );
	void			Close( void );
	bool			IsOpen( void ) const;

	Server *		GetServer( void );
	int				GetSocketID( void ) const;
	std::string		GetInBuffer( void );
	void			ClrInBuffer( void );
	void			ClrOutBuffer( void );

private:
	int				clientsockid;
	Server * 		m_pServer;
	std::string		inBuffer;
	std::string		outBuffer;
};

#endif
