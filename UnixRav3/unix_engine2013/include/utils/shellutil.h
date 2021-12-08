#ifndef __RX_SHELL_CMD_H_H__
#define __RX_SHELL_CMD_H_H__


namespace ShellUtil {;


inline HRESULT GetCmdResult( const char* cmd, std::string & sMsg )
{
	RASSERT(cmd, E_INVALIDARG);
#ifndef _WIN32
	FILE* _console = popen( cmd, "r" );
	if( _console )
	{
		char result[2048] = {};
		long l = fread( result, 1, sizeof(result), _console );
		pclose( _console );
		if( l > 0 )	{
			sMsg.assign( result, strlen(result) );
			return S_OK;
		}
	}
#endif
	return E_FAIL;
}


};


#endif