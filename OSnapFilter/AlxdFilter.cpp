#include "stdafx.h"
#include "AlxdFilter.h"
#include <AcDbPointCloudEx.h>

ACRX_CONS_DEFINE_MEMBERS(AlxdFilter, AcEdInputPointMonitor, 1);

AlxdFilter::AlxdFilter(AcApDocument* pDoc) : AcEdInputPointMonitor(), mpDocument(pDoc) {
	if (pDoc)
		pDoc->inputPointManager()->addPointMonitor(this);
}

AlxdFilter::~AlxdFilter() {
	Detach();
}

void AlxdFilter::Attach(AcApDocument* pDoc) {
	Detach();
	if (mpDocument == NULL) {
		if ((mpDocument = pDoc) != NULL)
			pDoc->inputPointManager()->addPointMonitor(this);
	}
}

void AlxdFilter::Detach() {
	if (mpDocument) {
		mpDocument->inputPointManager()->removePointMonitor(this);
		mpDocument = NULL;
	}
}

AcApDocument* AlxdFilter::Subject() const {
	return (mpDocument);
}

bool AlxdFilter::IsAttached() const {
	return (mpDocument != NULL);
}

bool AlxdFilter::excludeFromOsnapCalculation(
	const AcArray<AcDbObjectId>& nestedEntity,
	Adesk::GsMarker gsSelectionMark
) {
	//return (AcEdInputPointMonitor::excludeFromOsnapCalculation (nestedEntity, gsSelectionMark)) ;
	if (!DocVars.docData().b_On)
		return (AcEdInputPointMonitor::excludeFromOsnapCalculation(nestedEntity, gsSelectionMark));

	bool filter = false;
	bool localfilter = false;

	AcDbObjectId lastObjId;
	AcDbEntity* pLastEntity = NULL;

	lastObjId = nestedEntity.last();
	if (acdbOpenObject(pLastEntity, lastObjId, AcDb::kForRead) != Acad::eOk)
		return false;

	//	xref match
	if (DocVars.docData().s_XrefFilter == -1)
	{
		localfilter = true;
	}
	else
	{
		AcDbObjectId firstObjId;
		AcDbEntity* pFirstEntity = NULL;
		AcDbBlockReference* pBlockReference = NULL;

		firstObjId = nestedEntity.first();
		if (firstObjId == lastObjId)
		{
			pBlockReference = AcDbBlockReference::cast(pLastEntity);
		}
		else
		{
			if (acdbOpenObject(pFirstEntity, firstObjId, AcDb::kForRead) != Acad::eOk)
				return false;

			pBlockReference = AcDbBlockReference::cast(pFirstEntity);
		}

		if (pBlockReference != NULL)
		{
			AcDbObjectId btrId = pBlockReference->blockTableRecord();
			AcDbBlockTableRecord* pBTR = NULL;

			if (acdbOpenObject(pBTR, btrId, AcDb::kForRead) != Acad::eOk)
				return false;

			localfilter = pBTR->isFromExternalReference() && DocVars.docData().s_XrefFilter == 1;

			pBTR->close();
			pBlockReference->close();
		}
		else
			localfilter = true;

		filter |= !localfilter;

		if (pFirstEntity != NULL)
			pFirstEntity->close();
	}

	//	point cloud match
	if (DocVars.docData().s_PointCloudFilter == -1)
	{
		localfilter = true;
	}
	else
	{
		AcDbObjectId firstObjId;
		AcDbEntity* pFirstEntity = NULL;
		AcDbPointCloudEx* pPointCloudEx = NULL;

		pPointCloudEx = AcDbPointCloudEx::cast(pLastEntity);

		if (pPointCloudEx != NULL)
		{
			localfilter = DocVars.docData().s_PointCloudFilter == 1;

			pPointCloudEx->close();
		}
		else
			localfilter = false;

		filter |= !localfilter;
	}

	//	color match
	if (DocVars.docData().s_ColorNumber == -1)
		localfilter = true;
	else
		localfilter = (pLastEntity->colorIndex() == DocVars.docData().s_ColorNumber);

	if (DocVars.docData().b_ColorInverse)
		localfilter = !localfilter;

	filter |= !localfilter;

	//	layer match
	localfilter = acutWcMatchEx(pLastEntity->layer(), DocVars.docData().s_LayerFilter, true);

	if (DocVars.docData().b_LayerInverse)
		localfilter = !localfilter;

	filter |= !localfilter;

	//	linetype match
	localfilter = acutWcMatchEx(pLastEntity->linetype(), DocVars.docData().s_LinetypeFilter, true);

	if (DocVars.docData().b_LinetypeInverse)
		localfilter = !localfilter;

	filter |= !localfilter;

	//	lineweight match
	if (DocVars.docData().s_LineweightNumber == -4)
		localfilter = true;
	else
		localfilter = (pLastEntity->lineWeight() == DocVars.docData().s_LineweightNumber);

	if (DocVars.docData().b_LineweightInverse)
		localfilter = !localfilter;

	filter |= !localfilter;

	pLastEntity->close();
	return filter;
}

//Acad::ErrorStatus monitorInputPoint(
//	const AcEdInputPoint& /*input*/,
//	AcEdInputPointMonitorResult& output
//) {
	//appendToTooltipStr = false;

//	return (Acad::eOk);
//}