// (C) Copyright 2002-2012 by Autodesk, Inc. 
//
// Permission to use, copy, modify, and distribute this software in
// object code form for any purpose and without fee is hereby granted, 
// provided that the above copyright notice appears in all copies and 
// that both that copyright notice and the limited warranty and
// restricted rights notice below appear in all supporting 
// documentation.
//
// AUTODESK PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// AUTODESK SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTY OF
// MERCHANTABILITY OR FITNESS FOR A PARTICULAR USE.  AUTODESK, INC. 
// DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//
// Use, duplication, or disclosure by the U.S. Government is subject to 
// restrictions set forth in FAR 52.227-19 (Commercial Computer
// Software - Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
// (Rights in Technical Data and Computer Software), as applicable.
//

//-----------------------------------------------------------------------------
//----- DocData.cpp : Implementation file
//-----------------------------------------------------------------------------
#include "StdAfx.h"

//-----------------------------------------------------------------------------
//----- The one and only document manager object. You can use the DocVars object to retrieve
//----- document specific data throughout your application
AcApDataManager<CDocData> DocVars ;

//-----------------------------------------------------------------------------
//----- Implementation of the document data class.
CDocData::CDocData () {
	m_pAlxdFilter = NULL;

	b_On = false;
	b_ColorInverse = false;
	b_LayerInverse = false;
	b_LinetypeInverse = false;
	b_LineweightInverse = false;

	s_ColorNumber = -1;
	s_LayerFilter = NULL;
	s_LinetypeFilter = NULL;
	acutUpdString(L"*", s_LayerFilter);
	acutUpdString(L"*", s_LinetypeFilter);
	s_LineweightNumber = -4;
	s_XrefFilter = -1;
	s_PointCloudFilter = -1;
}

//-----------------------------------------------------------------------------
CDocData::CDocData (const CDocData &data) {
	m_pAlxdFilter = data.m_pAlxdFilter;

	b_On = data.b_On;
	b_ColorInverse = data.b_ColorInverse;
	b_LayerInverse = data.b_LayerInverse;
	b_LinetypeInverse = data.b_LinetypeInverse;
	b_LineweightInverse = data.b_LineweightInverse;
	s_XrefFilter = data.s_XrefFilter;
	s_PointCloudFilter = data.s_PointCloudFilter;

	s_ColorNumber = data.s_ColorNumber;
	s_LayerFilter = data.s_LayerFilter;
	s_LinetypeFilter = data.s_LinetypeFilter;
	acutUpdString(data.s_LayerFilter, s_LayerFilter);
	acutUpdString(data.s_LinetypeFilter, s_LinetypeFilter);
	s_LineweightNumber = data.s_LineweightNumber;
}

//-----------------------------------------------------------------------------
CDocData::~CDocData () {
	if (m_pAlxdFilter) delete m_pAlxdFilter;

	acutDelString(s_LayerFilter);
	acutDelString(s_LinetypeFilter);
}
