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
//----- acrxEntryPoint.cpp
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"

//-----------------------------------------------------------------------------
#define szRDS _RXST("Alxd")

//----------------------------------------------------------------------------- 

int get_color_name(int i, ACHAR* name)
{
	switch (i)
	{
	case -1:
		lstrcpy(name, L"*");
		break;
	case 0:
		lstrcpy(name, L"ByBlock");
		break;
	case 1:
		lstrcpy(name, L"Red");
		break;
	case 2:
		lstrcpy(name, L"Yellow");
		break;
	case 3:
		lstrcpy(name, L"Green");
		break;
	case 4:
		lstrcpy(name, L"Cyan");
		break;
	case 5:
		lstrcpy(name, L"Blue");
		break;
	case 6:
		lstrcpy(name, L"Magenta");
		break;
	case 7:
		lstrcpy(name, L"White");
		break;
	case 256:
		lstrcpy(name, L"ByLayer");
		break;
	default:
		swprintf(name, L"%d", i);
		break;
	}
	return 0;
}

int get_lineweight_name(int i, ACHAR* name)
{
	switch (i)
	{
	case -4:
		lstrcpy(name, L"*");
		break;
	case -3:
		lstrcpy(name, L"Default");
		break;
	case -2:
		lstrcpy(name, L"ByBlock");
		break;
	case -1:
		lstrcpy(name, L"ByLayer");
		break;
	default:
		swprintf(name, L"%d", i);
		break;
	}
	return 0;
}

int get_state_name(int i, ACHAR* name)
{
	if (i == -1)
		lstrcpy(name, L"Off");
	else if (i == 1)
		lstrcpy(name, L"Include");
	else
		lstrcpy(name, L"Exclude");
	return 0;
}

int get_inverse_name(bool b, ACHAR* name)
{
	if (b)
		lstrcpy(name, L"exclude");
	else
		lstrcpy(name, L" ");
	return 0;
}

bool get_inverse(bool d)
{
	ACHAR buf[133];
	ACHAR res[133];
	int	 ret;

	acedInitGet(0, L"Yes No");
	swprintf(buf, L"\nDo you want to use inverse [Yes/No]? <%s>: ", d ? L"Yes" : L"No");
	ret = acedGetKword(buf, res);

	if (ret == RTNORM)
		return (!lstrcmp(res, L"Yes"));

	return d;
}

int show_filter_state()
{
	ACHAR buf[15];
	ACHAR buf1[15];

	acutPrintf(L"\nActive selector condition: %s", DocVars.docData().b_On ? L"ON" : L"OFF");

	get_color_name(DocVars.docData().s_ColorNumber, buf);
	if (DocVars.docData().b_ColorInverse)
	{
		get_inverse_name(DocVars.docData().b_ColorInverse, buf1);
		acutPrintf(L"\n  Color number: %s %s", buf1, buf);
	}
	else
		acutPrintf(L"\n  Color number: %s", buf);

	if (DocVars.docData().b_LayerInverse)
	{
		get_inverse_name(DocVars.docData().b_LayerInverse, buf1);
		acutPrintf(L"\n  Layer name: %s %s", buf1, DocVars.docData().s_LayerFilter);
	}
	else
		acutPrintf(L"\n  Layer name: %s", DocVars.docData().s_LayerFilter);

	if (DocVars.docData().b_LinetypeInverse)
	{
		get_inverse_name(DocVars.docData().b_LinetypeInverse, buf1);
		acutPrintf(L"\n  Linetype name: %s %s", buf1, DocVars.docData().s_LinetypeFilter);
	}
	else
		acutPrintf(L"\n  Linetype name: %s", DocVars.docData().s_LinetypeFilter);

	get_lineweight_name(DocVars.docData().s_LineweightNumber, buf);
	if (DocVars.docData().b_LineweightInverse)
	{
		get_inverse_name(DocVars.docData().b_LineweightInverse, buf1);
		acutPrintf(L"\n  Lineweight value: %s %s", buf1, buf);
	}
	else
		acutPrintf(L"\n  Lineweight value: %s", buf);

	get_state_name(DocVars.docData().s_XrefFilter, buf);
	acutPrintf(L"\n  Xrefs: %s", buf);

	get_state_name(DocVars.docData().s_PointCloudFilter, buf);
	acutPrintf(L"\n  Point clouds: %s", buf);

	return 0;
}

//----- ObjectARX EntryPoint
class COSnapFilterApp : public AcRxArxApp {

public:
	COSnapFilterApp () : AcRxArxApp () {}

	virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt) {
		// TODO: Load dependencies here

		// You *must* call On_kInitAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kInitAppMsg (pkt) ;
		
		// TODO: Add your initialization code here
		DocVars.docData().m_pAlxdFilter = new AlxdFilter(acDocManager->curDocument());
		acutPrintf(L"\nAlxdOSnapFilter 2.0 Copyright © 2022 Alexander Shchetinin.");
		return (retCode) ;
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg (void *pkt) {
		// TODO: Add your code here

		// You *must* call On_kUnloadAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kUnloadAppMsg (pkt) ;

		// TODO: Unload dependencies here

		return (retCode) ;
	}

	virtual void RegisterServerComponents () {
	}
	static void Alxd_osnapfilter(void)
	{
		ACHAR buf[15];
		int	 ret;
		bool canExit = false;
		//ACHAR dummy[133];

		show_filter_state();

		while (!canExit)
		{
			acedInitGet(0, L"ON OFF Color LAyer LType LWeight XRef POintcloud eXit");
			//lstrcpy(buf, DocVars.docData().b_On ? L"ON" : L"OFF");
			//swprintf(dummy, L"\nEnter mode or select filter to change [ON/OFF/Color/LAyer/LType/LWeight/XRef/POintCloud/eXit] <eXit>: ", buf);
			ret = acedGetKword(L"\nEnter mode or select condition to change [ON/OFF/Color/LAyer/LType/LWeight/XRef/POintCloud/eXit] <eXit>: ", buf);

			if (ret == RTNORM)
			{
				if (!lstrcmp(buf, L"ON"))
				{
					DocVars.docData().b_On = true;
					canExit = true;
				}
				else
					if (!lstrcmp(buf, L"OFF"))
					{
						DocVars.docData().b_On = false;
						canExit = true;
					}
					else
						if (!lstrcmp(buf, L"Color"))
						{
							int iv;

							while (!canExit)
							{
								acedInitGet(0, L"Reset Select Inverse eXit");
								ret = acedGetInt(L"\nEnter color index or [Reset/Select/Inverse/eXit] <eXit>: ", &iv);
								if (ret == RTKWORD && acedGetInput(buf) == RTNORM)
								{
									if (!lstrcmp(buf, L"Reset"))
									{
										DocVars.docData().s_ColorNumber = -1;
									}
									else
										if (!lstrcmp(buf, L"Select"))
										{
											acedSetColorDialog(DocVars.docData().s_ColorNumber, Adesk::kTrue, 0);
										}
										else
											if (!lstrcmp(buf, L"Inverse"))
											{
												DocVars.docData().b_ColorInverse = get_inverse(DocVars.docData().b_ColorInverse);
											}
									canExit = true;
								}
								else
									if (ret == RTNORM)
									{
										if ((iv < -1) || (iv > 256))
											acutPrintf(L"Invalid color number.");
										else
										{
											DocVars.docData().s_ColorNumber = iv;
											canExit = true;
										}
									}
									else
										if (ret == RTCAN || ret == RTNONE)
											canExit = true;
							}
							canExit = false;
						}
						else
							if (!lstrcmp(buf, L"LAyer"))
							{
								acedInitGet(0, L"Name Inverse eXit");
								ret = acedGetKword(L"\nEnter layer condition option [Name/Inverse/eXit] <eXit>: ", buf);
								if (ret == RTNORM)
								{
									if (!lstrcmp(buf, L"Name"))
									{
										ACHAR dummy[133];
										if (acedGetString(1, L"\nEnter filter layer name: ", dummy) == RTNORM)
											acutUpdString(dummy, DocVars.docData().s_LayerFilter);
									}
									else
										if (!lstrcmp(buf, L"Inverse"))
										{
											DocVars.docData().b_LayerInverse = get_inverse(DocVars.docData().b_LayerInverse);
										}
								}
							}
							else
								if (!lstrcmp(buf, L"LType"))
								{
									acedInitGet(0, L"Name Inverse eXit");
									ret = acedGetKword(L"\nEnter linetype condition option [Name/Inverse/eXit] <eXit>: ", buf);
									if (ret == RTNORM)
									{
										if (!lstrcmp(buf, L"Name"))
										{
											ACHAR dummy[133];
											if (acedGetString(1, L"\nEnter filter linetype name: ", dummy) == RTNORM)
												acutUpdString(dummy, DocVars.docData().s_LinetypeFilter);
										}
										else
											if (!lstrcmp(buf, L"Inverse"))
											{
												DocVars.docData().b_LinetypeInverse = get_inverse(DocVars.docData().b_LinetypeInverse);
											}
									}
								}
								else
									if (!lstrcmp(buf, L"LWeight"))
									{
										int iv;

										while (!canExit)
										{
											acedInitGet(0, L"Reset Inverse eXit");
											ret = acedGetInt(L"\nEnter lineweight condition or [Reset/Inverse/eXit] <eXit>: ", &iv);
											if (ret == RTKWORD && acedGetInput(buf) == RTNORM)
											{
												if (!lstrcmp(buf, L"Reset"))
												{
													DocVars.docData().s_LineweightNumber = -4;
												}
												else
													if (!lstrcmp(buf, L"Inverse"))
													{
														DocVars.docData().b_LineweightInverse = get_inverse(DocVars.docData().b_LineweightInverse);
													}
												canExit = true;
											}
											else
												if (ret == RTNORM)
												{
													if (iv < -4)
														acutPrintf(L"Invalid lineweight value.");
													else
													{
														DocVars.docData().s_LineweightNumber = iv;
														canExit = true;
													}
												}
												else
													if (ret == RTCAN || ret == RTNONE)
														canExit = true;
										}
										canExit = false;
									}
									else
										if (!lstrcmp(buf, L"XRef"))
										{
											acedInitGet(0, L"Reset Include Exclude eXit");
											ret = acedGetKword(L"\nEnter external reference condition option [Reset/Include/Exclude/eXit] <eXit>: ", buf);
											if (ret == RTNORM)
											{
												if (!lstrcmp(buf, L"Include"))
												{
													DocVars.docData().s_XrefFilter = 1;
												}
												else
													if (!lstrcmp(buf, L"Exclude"))
													{
														DocVars.docData().s_XrefFilter = 0;
													}
													else
														if (!lstrcmp(buf, L"Reset"))
														{
															DocVars.docData().s_XrefFilter = -1;
														}
											}
										}
										else
											if (!lstrcmp(buf, L"POintcloud"))
											{
												acedInitGet(0, L"Reset Include Exclude eXit");
												ret = acedGetKword(L"\nEnter point cloud condition option [Reset/Include/Exclude/eXit] <eXit>: ", buf);
												if (ret == RTNORM)
												{
													if (!lstrcmp(buf, L"Include"))
													{
														DocVars.docData().s_PointCloudFilter = 1;
													}
													else
														if (!lstrcmp(buf, L"Exclude"))
														{
															DocVars.docData().s_PointCloudFilter = 0;
														}
														else
															if (!lstrcmp(buf, L"Reset"))
															{
																DocVars.docData().s_PointCloudFilter = -1;
															}
												}
											}
											else
												if (!lstrcmp(buf, L"eXit"))
													canExit = true;
			}
			else
				if (ret == RTCAN || ret == RTNONE)
					canExit = true;
		}
	}

	// ----- Alxd._osnapfilterstate command (do not rename)
	static void Alxd_osnapfilterstate(void)
	{
		// Add your code for command Alxd._osnapfilterstate here
		show_filter_state();
	}

	// ----- Alxd._osf command (do not rename)
	static void Alxd_osf(void)
	{
		// Add your code for command Alxd._osf here
		Alxd_osnapfilter();
	}

	// ----- Alxd._osfstate command (do not rename)
	static void Alxd_osfstate(void)
	{
		// Add your code for command Alxd._osfstate here
		Alxd_osnapfilterstate();
	}
	
} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(COSnapFilterApp)

ACED_ARXCOMMAND_ENTRY_AUTO(COSnapFilterApp, Alxd, _osnapfilter, osnapfilter, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(COSnapFilterApp, Alxd, _osnapfilterstate, osnapfilterstate, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(COSnapFilterApp, Alxd, _osf, osf, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(COSnapFilterApp, Alxd, _osfstate, osfstate, ACRX_CMD_TRANSPARENT, NULL)
