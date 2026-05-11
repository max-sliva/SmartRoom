/**
  Externed procedures to add additional action to handl to LockHandler;
*/

#ifndef LockHandler_h
#include "LockHandler.h"
#endif

#ifndef actionProcedures_h
#define actionProcedures_h

extern void grantAccessAction();
extern void grantAccessAction() __attribute__((weak));

extern void revokeAccessAction();
extern void revokeAccessAction() __attribute__((weak));

extern void repeatAccessAction();
extern void repeatAccessAction() __attribute__((weak));

extern void accessDeniedAction();
extern void accessDeniedAction() __attribute((weak));

extern void requestAccessAction();
extern void requestAccessAction() __attribute((weak));

extern void requestAbortAction();
extern void requestAbortAction() __attribute((weak));

extern void timeOutAction();
extern void timeOutAction() __attribute((weak));

#endif