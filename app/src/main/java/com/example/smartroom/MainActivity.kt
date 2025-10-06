package com.example.smartroom

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.enableEdgeToEdge
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.padding
import androidx.compose.material3.Scaffold
import androidx.compose.material3.Switch
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.MutableState
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.res.stringResource
import androidx.compose.ui.unit.dp
import com.example.smartroom.ui.theme.SmartRoomTheme

class MainActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        setContent {
            SmartRoomTheme {
                Scaffold(modifier = Modifier.fillMaxSize()) { innerPadding ->
                    MainColumn(
                        name = "User",
                        modifier = Modifier.padding(innerPadding)
                    )
                }
            }
        }
    }
}

@Composable
fun MainColumn(name: String, modifier: Modifier = Modifier) {
    Column{
        val helloWord = stringResource(R.string.helloWord)
        Text(
            text = "$helloWord $name!",
            modifier = modifier
        )
//        Row(
//            verticalAlignment = Alignment.CenterVertically,
////            modifier = Modifier
////                .padding(start = 10.dp)
//        ){
            val jalousie = stringResource(R.string.jalousie)
            var jalousieChecked = remember { mutableStateOf(false) }
            ToggleBtn(jalousie, jalousieChecked, Pair(stringResource(R.string.jalousieUp),stringResource(R.string.jalousieDown)))
            val window = stringResource(R.string.Window)
            var windowChecked = remember { mutableStateOf(true) }
            ToggleBtn(window, windowChecked, Pair(stringResource(R.string.WindowClosed),stringResource(R.string.WindowOpened)))
//        }
    }
}

@Composable
private fun ToggleBtn(
    name: String,
    checked: MutableState<Boolean> =  mutableStateOf(false),
    objectStatus: Pair<String, String>
) {
    Row(
        verticalAlignment = Alignment.CenterVertically,
        horizontalArrangement = Arrangement.SpaceAround,
            modifier = Modifier
//                .padding(start = 10.dp)
                .fillMaxWidth()
    ) {
        Text(
            text = "$name"
        )
//        var checked by remember { mutableStateOf(false) }
        Switch(
            checked = checked.value,
            onCheckedChange = {
                checked.value = it // 'it' is the new boolean value
            },
            modifier = Modifier
                .padding(start = 10.dp)
        )
        Text(text = if (checked.value) objectStatus.second else objectStatus.first) //todo добавить текст с выбором из массива с нужными значениями в зависимости от кнопки
    }
}

