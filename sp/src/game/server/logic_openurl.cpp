//====================== By Holly Liberatore / MoofEMP ======================//
//
// Purpose: Opens a URL in the client's browser, and returns whether it succeeded
//
//===========================================================================//

#if defined(_WIN32) && !defined(_X360)
#include <windows.h>
#endif

#include "cbase.h"

class CLogicOpenUrl : public CLogicalEntity
{
public:
	DECLARE_CLASS( CLogicOpenUrl, CLogicalEntity );
	DECLARE_DATADESC();

	CLogicOpenUrl( void ) { }

	void InputOpenUrl( inputdata_t &inputData );
	void InputOpenUrlByName( inputdata_t &inputData );

private:
	string_t m_strUrl;

	COutputEvent	m_OnOpenUrlSuccess;
	COutputEvent	m_OnOpenUrlFailure;
};

LINK_ENTITY_TO_CLASS( logic_openurl, CLogicOpenUrl );

BEGIN_DATADESC( CLogicOpenUrl )
	
	DEFINE_KEYFIELD( m_strUrl, FIELD_STRING, "url" ),

	DEFINE_INPUTFUNC( FIELD_VOID, "OpenUrl", InputOpenUrl ),
	DEFINE_INPUTFUNC( FIELD_STRING, "OpenUrlByName", InputOpenUrlByName ),

	DEFINE_OUTPUT( m_OnOpenUrlSuccess, "OnOpenUrlSuccess" ),
	DEFINE_OUTPUT( m_OnOpenUrlFailure, "OnOpenUrlFailure" ),

END_DATADESC()

//-----------------------------------------------------------------------------
// Purpose: Open keyvalue-stored URL, return based on whether it was successful
//-----------------------------------------------------------------------------
void CLogicOpenUrl::InputOpenUrl( inputdata_t &inputData )
{
	if((int)ShellExecute(NULL, "open", STRING(m_strUrl), NULL, NULL, SW_SHOWNORMAL)>32)
	{
		m_OnOpenUrlSuccess.FireOutput( inputData.pActivator, this );
	} else {
		m_OnOpenUrlFailure.FireOutput( inputData.pActivator, this );
	}
}

//-----------------------------------------------------------------------------
// Purpose: Open parameter URL, return based on whether it was successful
//-----------------------------------------------------------------------------
void CLogicOpenUrl::InputOpenUrlByName( inputdata_t &inputData )
{
	if((int)ShellExecute(NULL, "open", inputData.value.String(), NULL, NULL, SW_SHOWNORMAL)>32)
	{
		m_OnOpenUrlSuccess.FireOutput( inputData.pActivator, this );
	} else {
		m_OnOpenUrlFailure.FireOutput( inputData.pActivator, this );
	}
}
