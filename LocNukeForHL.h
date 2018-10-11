//°´StringTableÖÐID¼ÓÔØ×Ö·û´®
extern "C" __declspec(dllimport) 
BOOL /*WINAPI*//*__cdecl*/ LcnLoadString(		UINT nID,							// resource identifier
							   char* strBuffer,				// resource buffer
							   LPTSTR szModName,					// module name
							   LPTSTR szDefault=NULL
);

//°´×Ö·ûµ½×Ö·û·­Òë×Ö·û´®
extern "C" __declspec(dllimport) 
BOOL /*__cdecl*/ LcnTransString(		LPTSTR szKeyName,							// resource identifier
								CString& strBuffer,				// resource buffer
								LPTSTR szModName,					// module name
								LPTSTR szDefault=NULL
							   );

//°´Dlg ID·­Òë¶Ô»°¿ò
extern "C" __declspec(dllimport) 
BOOL /*WINAPI*//*__cdecl*/ LcnTranslateDialog(
									HWND hwnd,						// handle to dialog
									UINT nDLGID,						// resource identifier
									LPTSTR lpszModName				// module name
);
