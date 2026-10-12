#pragma once

#ifdef SMARTHOUSE_DEVICE_MEGA
#include "classes/CustomSerial.h"
#include "classes/Interfaces.h"
#include "libraries/Easy_Nextion_Library/src/EasyNextionLibrary.h"
#endif

#ifdef SMARTHOUSE_DEVICE_MINI
#include "classes/CustomSerial.h"
#include "classes/StvorkaElement.h"
#include "classes/LockHandler.h"
#endif