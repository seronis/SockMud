
#ifndef HEADER_SOCKET
#define HEADER_SOCKET

// c++ headers
#include <string>

class Socket
{
public:
	Socket( int desc );
	~Socket( void );

	bool                 Read( void );
	void                 Write( std::string txt );
	bool                 Flush( void );
	void                 Close( void );
	bool                 IsOpen( void );

	int                  GetSocketID( void );
	std::string          GetInBuffer( void );
	void                 ClrInBuffer( void );
	void                 ClrOutBuffer( void );

private:
	int                  sockid;
	std::string          inBuffer;
	std::string          outBuffer;
};

#endif
