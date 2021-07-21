//====================== By Holly Liberatore / MoofEMP ======================//
//
// Purpose: Opens a URL in the client's browser, and returns whether it succeeded
//
//===========================================================================//

#if defined(_WIN32) && !defined(_X360)
#include <windows.h>
#endif

#include <string>
#include "cbase.h"

#define SF_OPENURL_START_DISABLED (1 << 0)

class CLogicOpenUrl : public CLogicalEntity
{
public:
	DECLARE_CLASS( CLogicOpenUrl, CLogicalEntity );
	DECLARE_DATADESC();

	CLogicOpenUrl( void ) { }

	void InputDisable( inputdata_t &inputData );
	void InputEnable( inputdata_t &inputData );
	void InputOpenUrl( inputdata_t &inputData );
	void InputOpenUrlByName( inputdata_t &inputData );
	void InputSetUrl( inputdata_t &inputData );
	
	void Spawn(void);

private:
	string_t m_strUrl;

	bool m_bEnabled;

	void ExecuteUrl( std::string url, CBaseEntity *activator );

	COutputEvent	m_OnOpenUrlSuccess;
	COutputEvent	m_OnOpenUrlFailure;
};

LINK_ENTITY_TO_CLASS( logic_openurl, CLogicOpenUrl );

BEGIN_DATADESC( CLogicOpenUrl )
	
	DEFINE_FIELD( m_bEnabled, FIELD_BOOLEAN ),

	DEFINE_KEYFIELD( m_strUrl, FIELD_STRING, "url" ),

	DEFINE_INPUTFUNC( FIELD_VOID, "Disable", InputDisable ),
	DEFINE_INPUTFUNC( FIELD_VOID, "Enable", InputEnable ),
	DEFINE_INPUTFUNC( FIELD_VOID, "OpenUrl", InputOpenUrl ),
	DEFINE_INPUTFUNC( FIELD_STRING, "OpenUrlByName", InputOpenUrlByName ),
	DEFINE_INPUTFUNC( FIELD_STRING, "SetUrl", InputSetUrl ),

	DEFINE_OUTPUT( m_OnOpenUrlSuccess, "OnOpenUrlSuccess" ),
	DEFINE_OUTPUT( m_OnOpenUrlFailure, "OnOpenUrlFailure" ),

END_DATADESC()

//-----------------------------------------------------------------------------
// Purpose: Disable or enable the entity (disabling prevents any input functions from running)
//-----------------------------------------------------------------------------
void CLogicOpenUrl::InputDisable( inputdata_t &inputData ) { m_bEnabled = false; }
void CLogicOpenUrl::InputEnable ( inputdata_t &inputData ) { m_bEnabled = true ; }

//-----------------------------------------------------------------------------
// Purpose: Open keyvalue-stored URL
// Output: Whether the open succeeded
//-----------------------------------------------------------------------------
void CLogicOpenUrl::InputOpenUrl( inputdata_t &inputData )
{
	if( !m_bEnabled ) return;

	ExecuteUrl( STRING(m_strUrl), inputData.pActivator );
}

//-----------------------------------------------------------------------------
// Purpose: Open parameter URL
// Output: Whether the open succeeded
//-----------------------------------------------------------------------------
void CLogicOpenUrl::InputOpenUrlByName( inputdata_t &inputData )
{
	if( !m_bEnabled ) return;

	ExecuteUrl( inputData.value.String(), inputData.pActivator );
}

//-----------------------------------------------------------------------------
// Purpose: Internal method to avoid repeating code in input functions
//-----------------------------------------------------------------------------
void CLogicOpenUrl::ExecuteUrl( std::string url, CBaseEntity *activator )
{
	if( url.substr(0,8) != "https://" && url.substr(0,7) != "http://" )
	{
		// force https prefix to prevent arbitrary file execution on client disk with ShellExecute
		url = "https://" + url;
	}
	
	if( (int)ShellExecute( NULL, "open", url.c_str(), NULL, NULL, SW_SHOWNORMAL ) > 32 )
	{
		m_OnOpenUrlSuccess.FireOutput( activator, this );
	} else {
		m_OnOpenUrlFailure.FireOutput( activator, this );
	}
}

//-----------------------------------------------------------------------------
// Purpose: Setter method for URL keyvalue
//-----------------------------------------------------------------------------
void CLogicOpenUrl::InputSetUrl( inputdata_t &inputData )
{
	if( !m_bEnabled ) return;
	
	m_strUrl = inputData.value.StringID();
}

//-----------------------------------------------------------------------------
// Purpose: Respond to spawnflags when entity spawns
//-----------------------------------------------------------------------------
void CLogicOpenUrl::Spawn( void )
{
	m_bEnabled = !HasSpawnFlags( SF_OPENURL_START_DISABLED );
}
