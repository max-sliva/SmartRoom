#pragma once

#ifdef SMARTHOUSE_DEVICE_MEGA
#include "CustomSerial.h"
#include "Interfaces.h"
#include "UsbListener.h"
#include <EasyNextionLibrary.h>
#endif

#ifdef SMARTHOUSE_DEVICE_MINI
#include "CustomSerial.h"
#include "StvorkaElement.h"
#include "LedHandler.h"
#include "LockHandler.h"
#endif