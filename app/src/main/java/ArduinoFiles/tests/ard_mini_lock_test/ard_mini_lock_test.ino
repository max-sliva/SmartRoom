#include <Keypad.h> 
const byte numRows = 3; //кол-во строк 
const byte numCols = 4; //кол-во столбцов 
uint8_t a = 2, b = 3, c = 4, e = 5, f = 6, g = 7 ,h = 8;


//матрица, определяющая расположение эл-ов на клавиатуре 
char keymap[numRows][numCols] = 
{ 
  {'1', '4', '7','*'}, 
  {'2', '5', '8','0'}, 
  {'3', '6', '9','#'} 
}; 
 
//определяем подключение пинов для строк и столбцов  
//в виде одномерных массивов 
byte rowPins[numRows] = {c, h, f};  
byte colPins[numCols] = {e, b, a, g};  
 
//инициализируем клавиатуру 
Keypad myKeypad = Keypad(makeKeymap(keymap), rowPins, colPins,  
            numRows, numCols); 
 
void setup() 
{ 
  Serial.begin(9600); 
  while (!Serial);
  Serial.println("Serial Begin");
} 
 
void loop() 
{ 
  char keypressed = myKeypad.getKey(); //получаем символ на нажатой клавише 
  if (keypressed != NO_KEY) //если он не пустой 
  { 
    Serial.println(keypressed); //выводим его в монитор порта 
  } 
}