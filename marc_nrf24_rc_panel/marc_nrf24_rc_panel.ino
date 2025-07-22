/*   Данный скетч делает следующее: передатчик (TX) отправляет массив
     данных, который генерируется согласно показаниям с кнопки и с
     двух потенциомтеров. Приёмник (RX) получает массив, и записывает
     данные на реле, сервомашинку и генерирует ШИМ сигнал на транзистор.
    by AlexGyver 2016
*/

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display


RF24 radio(9, 10); // "создать" модуль на пинах 9 и 10 Для Уно
//RF24 radio(9,53); // для Меги

byte address[][6] = {"1Node", "2Node", "3Node", "4Node", "5Node", "6Node"}; //возможные номера труб

int x_pot_pin=A2;
int y_pot_pin=A3;

int transmit_data[3];  // массив, хранящий передаваемые данные

void setup() {
lcd.init();                      // initialize the lcd 
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(6,0);
  lcd.print("MARS");
  
  
  Serial.begin(9600);   //открываем порт для связи с ПК

  //pinMode(button, INPUT_PULLUP); // настроить пин кнопки

  radio.begin();              // активировать модуль
  radio.setAutoAck(1);        // режим подтверждения приёма, 1 вкл 0 выкл
  radio.setRetries(0, 15);    // (время между попыткой достучаться, число попыток)
  radio.enableAckPayload();   // разрешить отсылку данных в ответ на входящий сигнал
  radio.setPayloadSize(32);   // размер пакета, в байтах

  radio.openWritingPipe(address[0]);  // мы - труба 0, открываем канал для передачи данных
  radio.setChannel(0x70);     // выбираем канал (в котором нет шумов!)

  radio.setPALevel (RF24_PA_MAX);   // уровень мощности передатчика. На выбор RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
  radio.setDataRate (RF24_250KBPS); // скорость обмена. На выбор RF24_2MBPS, RF24_1MBPS, RF24_250KBPS
  //должна быть одинакова на приёмнике и передатчике!
  //при самой низкой скорости имеем самую высокую чувствительность и дальность!!

  radio.powerUp();        //начать работу
  radio.stopListening();  //не слушаем радиоэфир, мы передатчик
}

void loop() {
  
  transmit_data[0] = 1;
  transmit_data[1] = analogRead(x_pot_pin); // получить значение
  transmit_data[2] = analogRead(y_pot_pin);



    radio.powerUp();    // включить передатчик
    radio.write(&transmit_data, sizeof(transmit_data)); // отправить по радио

    radio.powerDown();  // выключить передатчик
  delay(10);
}
