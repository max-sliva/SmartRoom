#pragma once

#ifdef SMARTHOUSE_DEVICE_MEGA
#include "libraries/DefaultNextion.h"
#include "classes/CustomSerial.h"
#include "classes/Interfaces.h"
#include "classes/UsbListener.h"
#endif

#ifdef SMARTHOUSE_DEVICE_MINI
#include "classes/CustomSerial.h"
#include "classes/StvorkaElement.h"
#include "classes/LockHandler.h"
#include "classes/LockHandler.cpp"
#endif