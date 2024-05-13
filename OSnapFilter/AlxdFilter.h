#pragma once
#include "C:\ObjectARX 2021\inc\acedinpt.h"
class AlxdFilter :
    public AcEdInputPointMonitor
{
public:
    ACRX_DECLARE_MEMBERS(AlxdFilter);

protected:
    //----- Pointer to the document this reactor instance belongs to.
    AcApDocument* mpDocument;

public:
	AlxdFilter(AcApDocument* pDoc = NULL);
	virtual ~AlxdFilter();

	virtual void Attach(AcApDocument* pDoc);
	virtual void Detach();
	virtual AcApDocument* Subject() const;
	virtual bool IsAttached() const;

	virtual bool excludeFromOsnapCalculation(
		const AcArray<AcDbObjectId>& nestedEntity,
		Adesk::GsMarker gsSelectionMark
	);

	//virtual Acad::ErrorStatus monitorInputPoint(
	//	const AcEdInputPoint& /*input*/,
	//	AcEdInputPointMonitorResult& /*output*/);
};

#ifdef OSNAPFILTER_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(AlxdFilter)
#endif