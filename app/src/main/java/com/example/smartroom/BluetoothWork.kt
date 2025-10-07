package com.example.smartroom

import android.Manifest
import android.bluetooth.BluetoothAdapter
import android.bluetooth.BluetoothDevice
import android.bluetooth.BluetoothManager
import android.bluetooth.BluetoothSocket
import android.content.Context
import android.content.pm.PackageManager
import android.os.Parcel
import android.os.Parcelable
import androidx.compose.foundation.background
import androidx.compose.foundation.clickable
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.wrapContentSize
import androidx.compose.material3.DropdownMenu
import androidx.compose.material3.DropdownMenuItem
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateListOf
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.setValue
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.core.app.ActivityCompat
import java.io.IOException
import java.io.Serializable
import java.util.UUID

@Composable
fun BluetoothDropdownList(
    itemsInitial: List<String>,
    onUpdate: (x: String) -> Unit?
) { //комбобокс для выбора компорта для подключения к Arduino
    var expanded by remember { mutableStateOf(false) }
//    val items = listOf("com1", "com2", "com3")
//    val disabledValue = "B"
    var items = remember { mutableStateListOf<String>() }
    itemsInitial.forEach {
        if (!items.contains(it)) items.add(it)
    }
    var selectedIndex by remember { mutableStateOf(-1) }
    Box(modifier = Modifier.wrapContentSize(Alignment.TopStart)) {
        Text( //заголовок комбобокса
            if (selectedIndex < 0) "Выберите устройство: ▼" //если еще ничего не выбрано
            else items[selectedIndex], //если выбрано
            modifier = Modifier.clickable(onClick = { //при нажатии на текст раскрываем комбобокс
//                val tempPortList = SerialPortList.getPortNames().toList() //получаем активные порты
//                println("SerialPortList = $tempPortList")
//                tempPortList.forEach {//добавляем новые порты к списку
//                    if (!items.contains(it))items.add(it)
//                }
//                items.forEach{//убираем отключенные порты
//                    if (!tempPortList.contains(it)) {
////                        println("$it not in SerialPortList")
//                        items.remove(it)
//                    }
//                }
//                val bltList =
                expanded = true
            })
        )
        DropdownMenu( //сам выпадающий список для комбобокса
            expanded = expanded,
            onDismissRequest = { expanded = false },
            modifier = Modifier
                .fillMaxWidth()
                .background(Color.White)
        ) {
            items.forEachIndexed { index, s -> //заполняем элементы выпадающего списка
                DropdownMenuItem(
//                   Text(text = s),
                    text = { Text(text = s) },
                    onClick = { //обработка нажатия на порт
                        selectedIndex = index
                        expanded = false
                        onUpdate(s)
                        println("selected = $s")
                    }
                )
//                {
////                    val disabledText = if (s == disabledValue) {
////                        " (Disabled)"
////                    } else {
////                        ""
////                    }
//                    Text(text = s )
//                }
            }
        }
    }
}

class BluetoothWork(localContext: Context, myLauncherActivity: MainActivity) {
    private var context: Context = localContext
    private var bltList = listOf<String>()
    private var pairedDevices: Set<BluetoothDevice>? = null
    companion object {
        var currentSocket: BluetoothSocket? = null
    }
    init {
        context = localContext
        if (ActivityCompat.checkSelfPermission(
                context,
                Manifest.permission.BLUETOOTH_CONNECT
            ) != PackageManager.PERMISSION_GRANTED
        ) {
            println("Should Requesting Bluetooth permission")
            ActivityCompat.requestPermissions(
                myLauncherActivity,
                arrayOf(Manifest.permission.BLUETOOTH_CONNECT),
                2
            )
//            return emptyList()
        }
    }

    fun getBluetoothDevices(onUpdate: (list: List<String>) -> Unit): Set<BluetoothDevice>? {

        val bluetoothManager =
            context.getSystemService(Context.BLUETOOTH_SERVICE) as BluetoothManager
        val bluetoothAdapter: BluetoothAdapter = bluetoothManager.adapter
        if (!bluetoothAdapter.isEnabled) {
            println("Bluetooth is not enabled")
        }
        if (ActivityCompat.checkSelfPermission(
                context,
                Manifest.permission.BLUETOOTH_CONNECT
            ) != PackageManager.PERMISSION_GRANTED
        ) {
            println("Should Requesting Bluetooth permission")
//            return emptyList()
        }
        pairedDevices = bluetoothAdapter.bondedDevices
//        println("---!! set of devices size = ${pairedDevices?.size} !!!-----")
        pairedDevices?.forEach { device ->
            val deviceName = device.name
            val deviceHardwareAddress = device.address // MAC address
//            println("blt device = $deviceName")
            bltList = bltList.plus(deviceName)
        }
        onUpdate(bltList)
        return pairedDevices
    }

    fun connectToBluetoothDevice(device: BluetoothDevice): BluetoothSocket? {
        // Check if the device is already connected
        if (ActivityCompat.checkSelfPermission(
                context,
                Manifest.permission.BLUETOOTH_CONNECT
            ) != PackageManager.PERMISSION_GRANTED
        ) {
            println("Should Requesting Bluetooth permission")
//            return emptyList()
        }
        var socket: BluetoothSocket? = null
//        if (device.bondState!= BluetoothDevice.BOND_BONDED) {
        // Create a BluetoothSocket for the device
        socket = device.createRfcommSocketToServiceRecord(UUID.fromString("00001101-0000-1000-8000-00805F9B34FB"))

        // Attempt to connect to the device
        try {
            socket?.connect()
        } catch (e:IOException){
            println("-----!!in BluetoothWork  device is null  !!------------")
            return null
        }
        println("device is connected")
        // Do something with the socket, such as send or receive data
//        }
        return socket
    }

    fun setCurrentSocket(socket: BluetoothSocket){
        currentSocket = socket
    }

    fun printBltDevicesList(){
        bltList?.forEach { device ->
//            val deviceName = device.name
//            val deviceHardwareAddress = device.address // MAC address
            println("blt device = $device")
//            bltList = bltList.plus(deviceName)
        }
    }

    fun sendDataToBluetoothDevice(socket: BluetoothSocket, data: String) {
        try {
            if (ActivityCompat.checkSelfPermission(
                    context,
                    Manifest.permission.BLUETOOTH_CONNECT
                ) != PackageManager.PERMISSION_GRANTED
            ) {
                println("Should Requesting Bluetooth permission")
//                ActivityCompat.requestPermissions(::LauncherActivity, arrayOf(Manifest.permission.BLUETOOTH_CONNECT), REQUEST_CODE)
//            return emptyList()
            }
            // Get the BluetoothSocket for the device
//            val socket = device.createRfcommSocketToServiceRecord(UUID.fromString("00001101-0000-1000-8000-00805F9B34FB"))
//            socket.connect()
            // Convert the string to bytes
            val bytes = data.toByteArray()
            // Send the bytes to the device
            val outputStream = socket.outputStream
            outputStream.write(bytes)
            // Close the socket
//            socket.close()
        } catch (e: IOException) {
            e.printStackTrace()
        }
    }

    fun getDeviceList(): List<String> = bltList
    fun getDeviceByName(curDeviceName: String): BluetoothDevice {
        if (ActivityCompat.checkSelfPermission(
                context,
                Manifest.permission.BLUETOOTH_CONNECT
            ) != PackageManager.PERMISSION_GRANTED
        ) {
            println("Should Requesting Bluetooth permission")
//            return emptyList()
        }
        val curDevice = pairedDevices?.filter {
            it.name == curDeviceName
        }?.first()

        return curDevice!!
    }
}