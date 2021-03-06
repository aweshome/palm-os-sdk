/******************************************************************************
 *
 * Copyright (c) 1997-1999 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: ExgMgr.h
 *
 * Description:
 *		Include file for Exg system functions
 *
 * History:
 *   	5/23/97 Created by Gavin Peacock
 *
 *****************************************************************************/

#ifndef __EXGMGR_H__
#define __EXGMGR_H__

#include <PalmTypes.h>
#include <CoreTraps.h>

#include <ErrorBase.h>
#include <DataMgr.h>

#define exgMemError	 			(exgErrorClass | 1)
#define exgErrStackInit 		(exgErrorClass | 2)  // stack could not initialize
#define exgErrUserCancel 		(exgErrorClass | 3)
#define exgErrNoReceiver 		(exgErrorClass | 4)	// receiver device not found
#define exgErrNoKnownTarget		(exgErrorClass | 5)	// can't find a target app
#define exgErrTargetMissing		(exgErrorClass | 6)  // target app is known but missing
#define exgErrNotAllowed		(exgErrorClass | 7)  // operation not allowed
#define exgErrBadData			(exgErrorClass | 8)  // internal data was not valid
#define exgErrAppError			(exgErrorClass | 9)  // generic application error
#define exgErrUnknown			(exgErrorClass | 10) // unknown general error
#define exgErrDeviceFull		(exgErrorClass | 11) // device is full
#define exgErrDisconnected		(exgErrorClass | 12) // link disconnected
#define exgErrNotFound			(exgErrorClass | 13) // requested object not found
#define exgErrBadParam			(exgErrorClass | 14) // bad parameter to call
#define exgErrNotSupported		(exgErrorClass | 15) // operation not supported by this library
#define exgErrDeviceBusy		(exgErrorClass | 16) // device is busy
#define exgErrBadLibrary		(exgErrorClass | 17) // bad or missing ExgLibrary


typedef struct {
	UInt16				dbCardNo;			// card number of the database	
	LocalID				dbID;				// LocalID of the database
	UInt16 				recordNum;			// index of record that contain a match
	UInt32				uniqueID;			// postion in record of the match.
	UInt32				matchCustom;		// application specific info
} ExgGoToType;	

typedef ExgGoToType *ExgGoToPtr;


typedef struct ExgSocketType {
	UInt16	libraryRef;	// identifies the Exg library in use
	UInt32 	socketRef;	// used by Exg library to identify this connection
	UInt32 	target;		// Creator ID of application this is sent to
	UInt32	count;		// # of objects in this connection (usually 1)
	UInt32	length;		// # total byte count for all objects being sent (optional)
	UInt32	time;		// last modified time of object (optional)
	UInt32	appData;	// application specific info
	UInt32 	goToCreator; // creator ID of app to launch with goto after receive
	ExgGoToType goToParams;	// If launchCreator then this contains goto find info
	UInt16	localMode:1; // Exchange with local machine only mode 
	UInt16	packetMode:1;// Use connectionless packet mode (Ultra)
	UInt16	noGoTo:1; 	// Do not go to app (local mode only)
	UInt16 	noStatus:1; // Do not display status dialogs
	UInt16	reserved:12;// reserved system flags
	Char *description;	// text description of object (for user)
	Char *type;		// Mime type of object (optional)
	Char *name;		// name of object, generally a file name (optional)
} ExgSocketType;
typedef ExgSocketType *ExgSocketPtr;


// structures used for sysAppLaunchCmdExgAskUser launch code parameter
// default is exgAskDialog (ask user with dialog...
typedef enum { exgAskDialog,exgAskOk,exgAskCancel } ExgAskResultType;

typedef struct {
	ExgSocketPtr		socketP;
	ExgAskResultType	result;			// what to do with dialog	
	UInt8 				reserved;
} ExgAskParamType;	
typedef ExgAskParamType *ExgAskParamPtr;

// Optional parameter structure used with ExgDoDialog for category control
typedef struct {
	UInt16			version;		// version of this structure (should be zero)
	DmOpenRef		db;				// open database ref (for category information)
	UInt16			categoryIndex;	// index of selected category
} ExgDialogInfoType;


#define exgSeparatorChar '\t'		// char used to separate multiple registry entries

#define exgRegLibraryID 0xfffc					// library register thier presence
#define exgRegExtensionID 0xfffd				// filename extenstion registry
#define exgRegTypeID 0xfffe						// MIME type registry
	
#define exgDataPrefVersion 0
#define exgMaxTitleLen     20					// max size for title from exgLibCtlGetTitle

#define exgLibCtlGetTitle	1					// get title for Exg dialogs
#define exgLibCtlSpecificOp 0x8000				// start of range for library specific control codes


typedef Err	(*ExgDBReadProcPtr) 
				(void *dataP, UInt32 *sizeP, void *userDataP);

typedef Boolean	(*ExgDBDeleteProcPtr)
				(const char *nameP, UInt16 version, UInt16 cardNo,
				LocalID dbID, void *userDataP);

typedef Err	(*ExgDBWriteProcPtr)
				(const void *dataP, UInt32 *sizeP, void *userDataP);

#ifdef __cplusplus
extern "C" {
#endif

Err ExgInit(void)  
		SYS_TRAP(sysTrapExgInit);

Err ExgConnect(ExgSocketPtr socketP)
		SYS_TRAP(sysTrapExgConnect);

Err ExgPut(ExgSocketPtr socketP)
		SYS_TRAP(sysTrapExgPut);

Err ExgGet(ExgSocketPtr socketP)
		SYS_TRAP(sysTrapExgGet);

Err ExgAccept(ExgSocketPtr socketP)
		SYS_TRAP(sysTrapExgAccept);

Err ExgDisconnect(ExgSocketPtr socketP,Err error)
		SYS_TRAP(sysTrapExgDisconnect);

UInt32 ExgSend(ExgSocketPtr socketP, const void * const bufP, const UInt32 bufLen, Err *err)
		SYS_TRAP(sysTrapExgSend);

UInt32 ExgReceive(ExgSocketPtr socketP, void *bufP, const UInt32 bufLen, Err *err)
		SYS_TRAP(sysTrapExgReceive);

Err ExgRegisterData(const UInt32 creatorID, const UInt16 id, const Char * const dataTypesP)
		SYS_TRAP(sysTrapExgRegisterData);

Err ExgNotifyReceive(ExgSocketPtr socketP)
		SYS_TRAP(sysTrapExgNotifyReceive);


Err	ExgDBRead(
		ExgDBReadProcPtr		readProcP,
		ExgDBDeleteProcPtr		deleteProcP,
		void*					userDataP,
		LocalID*				dbIDP,
		UInt16					cardNo,
		Boolean*				needResetP,
		Boolean					keepDates)
		SYS_TRAP(sysTrapExgDBRead);

Err	ExgDBWrite(
		ExgDBWriteProcPtr		writeProcP,
		void*					userDataP,
		const char*				nameP,
		LocalID					dbID,
		UInt16					cardNo)
		SYS_TRAP(sysTrapExgDBWrite);


Boolean ExgDoDialog(ExgSocketPtr socketP, ExgDialogInfoType *infoP, Err *errP)
		SYS_TRAP(sysTrapExgDoDialog);

#ifdef __cplusplus 
}
#endif

#endif  // __EXGMGR_H__

