#include <SD.h>
#include <Adafruit_AS7341.h>
//#include <SoftwareSerial.h>
#include <Wire.h>
#include "RTClib.h"

unsigned long tempsensor = 0;
//unsigned long tempBlue = 500;

File dataFile;
Adafruit_AS7341 as7341;
//SoftwareSerial Bluetooth(2, 3);
RTC_DS3231 rtc;

float comprimento_dominante = 0.0;


void setup() {

  Serial.begin(9600);

  //Bluetooth.begin(9600);


  while (!as7341.begin()) {
    Serial.println("Erro AS7341");
    delay(1000);
  }

  as7341.setATIME(100);
  as7341.setASTEP(999);
  as7341.setGain(AS7341_GAIN_256X);


  while (!rtc.begin()) {
    Serial.println("Erro no RTC!");
    delay(1000);
  }

  DateTime now = rtc.now();

/*
  while (!SD.begin(10)) {
    Serial.println("Falha no Cartao SD!");
    delay(1000);
  }


  String Name =
    String("/") +
    String(now.day()) +
    String("_") +
    String(now.month()) +
    String("_") +
    String(now.year() % 100) +
    String(".csv");


  dataFile = SD.open(
    Name,
    FILE_WRITE
  );


  if (dataFile) {

    dataFile.print(
      "Ano;Mes;Dia;Hora;Minuto;Segundo;"
      "415nm;445nm;480nm;515nm;"
      "555nm;590nm;630nm;680nm;"
      "Comprimento_Dominante\n"
    );

    dataFile.flush();

  } else {

    while (true) {

      Serial.println("Erro no arquivo!");

      delay(1000);
    }
  }*/
}


void loop() {

  DateTime now = rtc.now();


  if (millis() - tempsensor >= 1000) {

    AS7341();

    //Salva_sd(now);

    tempsensor = millis();
  }


  /*
  if (millis() - tempBlue >= 2000) {

    AS7341();

    bluetooth();

    tempBlue = millis();
  }
  */
}


void AS7341() {

  if (!as7341.readAllChannels()) {

    Serial.println("Erro AS7341");

    return;
  }


  int comprimentos_onda[8] = {
    415,
    445,
    480,
    515,
    555,
    590,
    630,
    680
  };


  uint16_t intensidades[8] = {

    as7341.getChannel(AS7341_CHANNEL_415nm_F1),

    as7341.getChannel(AS7341_CHANNEL_445nm_F2),

    as7341.getChannel(AS7341_CHANNEL_480nm_F3),

    as7341.getChannel(AS7341_CHANNEL_515nm_F4),

    as7341.getChannel(AS7341_CHANNEL_555nm_F5),

    as7341.getChannel(AS7341_CHANNEL_590nm_F6),

    as7341.getChannel(AS7341_CHANNEL_630nm_F7),

    as7341.getChannel(AS7341_CHANNEL_680nm_F8)
  };


  float soma_ponderada = 0.0;

  float soma_pesos = 0.0;


  for (int i = 0; i < 8; i++) {

    soma_ponderada +=
      (float)intensidades[i] *
      (float)comprimentos_onda[i];

    soma_pesos +=
      (float)intensidades[i];
  }


  if (soma_pesos > 0) {

    comprimento_dominante =
      soma_ponderada / soma_pesos;

  } else {

    comprimento_dominante = 0.0;
  }


  for (int i = 0; i < 8; i++) {

    Serial.println(intensidades[i]);
  }


  Serial.println(
    comprimento_dominante,
    2
  );
}


void Salva_sd(DateTime now) {

  dataFile.print(
    String(now.year()) + ";"
  );

  dataFile.print(
    String(now.month()) + ";"
  );

  dataFile.print(
    String(now.day()) + ";"
  );

  dataFile.print(
    String(now.hour()) + ";"
  );

  dataFile.print(
    String(now.minute()) + ";"
  );

  dataFile.print(
    String(now.second()) + ";"
  );


  dataFile.print(
    String(
      as7341.getChannel(
        AS7341_CHANNEL_415nm_F1
      )
    ) + ";"
  );

  dataFile.print(
    String(
      as7341.getChannel(
        AS7341_CHANNEL_445nm_F2
      )
    ) + ";"
  );

  dataFile.print(
    String(
      as7341.getChannel(
        AS7341_CHANNEL_480nm_F3
      )
    ) + ";"
  );

  dataFile.print(
    String(
      as7341.getChannel(
        AS7341_CHANNEL_515nm_F4
      )
    ) + ";"
  );

  dataFile.print(
    String(
      as7341.getChannel(
        AS7341_CHANNEL_555nm_F5
      )
    ) + ";"
  );

  dataFile.print(
    String(
      as7341.getChannel(
        AS7341_CHANNEL_590nm_F6
      )
    ) + ";"
  );

  dataFile.print(
    String(
      as7341.getChannel(
        AS7341_CHANNEL_630nm_F7
      )
    ) + ";"
  );

  dataFile.print(
    String(
      as7341.getChannel(
        AS7341_CHANNEL_680nm_F8
      )
    ) + ";"
  );


  dataFile.println(
    String(comprimento_dominante)
  );


  dataFile.flush();
}


/*
void bluetooth() {

  Bluetooth.println(
    as7341.getChannel(
      AS7341_CHANNEL_415nm_F1
    )
  );

  Bluetooth.println(
    as7341.getChannel(
      AS7341_CHANNEL_445nm_F2
    )
  );

  Bluetooth.println(
    as7341.getChannel(
      AS7341_CHANNEL_480nm_F3
    )
  );

  Bluetooth.println(
    as7341.getChannel(
      AS7341_CHANNEL_515nm_F4
    )
  );

  Bluetooth.println(
    as7341.getChannel(
      AS7341_CHANNEL_555nm_F5
    )
  );

  Bluetooth.println(
    as7341.getChannel(
      AS7341_CHANNEL_590nm_F6
    )
  );

  Bluetooth.println(
    as7341.getChannel(
      AS7341_CHANNEL_630nm_F7
    )
  );

  Bluetooth.println(
    as7341.getChannel(
      AS7341_CHANNEL_680nm_F8
    )
  );

  Bluetooth.println(
    comprimento_dominante
  );
}
*/