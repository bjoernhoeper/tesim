/*

* Author:       Rick Candell (rick.candell@nist.gov)
* 
* Organization: National Institute of Standards and Technology
*               U.S. Department of Commerce
* License:      Public Domain

*/

#ifndef __TE_AD_IF__
#define __TE_AD_IF__

#include <windows.h>
// ADS headers for TwinCAT 3
#include "C:\TwinCAT\AdsApi\TcAdsDll\Include\TcAdsDef.h"
#include "C:\TwinCAT\AdsApi\TcAdsDll\Include\TcAdsAPI.h"

#include <string>

class TEADSInterface
{
public:

	struct ADSError
	{
		ADSError(const long nErr, const std::string errStr)
			: nErr(nErr), errStr(errStr){}

		long			nErr;
		std::string		errStr;
	};

	TEADSInterface();
	virtual ~TEADSInterface();

	void connect(std::string varName, const short nRemotePort);			// local connections
	void connect(std::string varName, const PAmsAddr remote_addr);	// explicit local/remote connections
	void write(const double* x);
	void read(float* x);

private:
	
	TEADSInterface(const TEADSInterface&);
	TEADSInterface& operator=(const TEADSInterface&);

	short			m_nLocalPort; 
	short			m_nRemotePort;
	AmsAddr			m_AmsAddr;
	PAmsAddr		m_pAmsAddr;
	ULONG			m_hVarHandle;
	std::string     m_varName;
};


#endif
