package com.example.smartroom

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.enableEdgeToEdge
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.padding
import androidx.compose.material3.Scaffold
import androidx.compose.material3.Switch
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.MutableState
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.setValue
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
            ToggleBtn(jalousie)
            val window = stringResource(R.string.Window)
            var checked = remember { mutableStateOf(true) }
            ToggleBtn(window, checked)
//        }
    }
}

@Composable
private fun ToggleBtn(jalousie: String, checked: MutableState<Boolean> = mutableStateOf(false)) {
    //todo добавить в качестве параметра пару значений для переключения при нажатии
    Row(
        verticalAlignment = Alignment.CenterVertically,
            modifier = Modifier
                .padding(start = 10.dp)
    ) {
        Text(
            text = "$jalousie"
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
//        Text() //todo добавить текст с выбором из массива с нужными значениями в зависимости от кнопки
    }
}

