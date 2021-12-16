/*
┏━━━┓╋╋╋╋╋╋╋╋╋╋╋╋╋╋╋╋┏━━━┓╋╋╋┏┓┏┓
┃┏━━┛╋╋╋╋╋╋╋╋╋╋╋╋╋╋╋╋┃┏━┓┃╋╋╋┃┃┃┃
┃┗━━┳┓┏┳━━┳━━┳━┓┏┳━━┓┃┃╋┗╋┳┓┏┫┃┃┃┏━━┓
┃┏━━┫┃┃┃┏┓┃┃━┫┏┓╋┫┏┓┃┃┃╋┏╋┫┃┃┃┃┃┃┃┏┓┃
┃┗━━┫┗┛┃┗┛┃┃━┫┃┃┃┃┗┛┃┃┗━┛┃┃┗┛┃┗┫┗┫┗┛┃
┗━━━┻━━┻━┓┣━━┻┛┗┻┻━━┛┗━━━┻┻━━┻━┻━┻━━┛
╋╋╋╋╋╋╋┏━┛┃
╋╋╋╋╋╋╋┗━━┛


███████╗░█████╗░░░░░░░░██████╗██╗███╗░░░███╗
██╔════╝██╔══██╗░░░░░░██╔════╝██║████╗░████║
█████╗░░██║░░╚═╝█████╗╚█████╗░██║██╔████╔██║
██╔══╝░░██║░░██╗╚════╝░╚═══██╗██║██║╚██╔╝██║
███████╗╚█████╔╝░░░░░░██████╔╝██║██║░╚═╝░██║
╚══════╝░╚════╝░░░░░░░╚═════╝░╚═╝╚═╝░░░░░╚═╝

██████╗░██████╗░░█████╗░████████╗██████╗░███████╗██████╗░░█████╗░██╗░░░░░░██████╗
╚════██╗██╔══██╗██╔══██╗╚══██╔══╝██╔══██╗██╔════╝██╔══██╗██╔══██╗██║░░░░░██╔════╝
░█████╔╝██████╔╝██║░░██║░░░██║░░░██████╔╝█████╗░░██║░░██║███████║██║░░░░░╚█████╗░
░╚═══██╗██╔═══╝░██║░░██║░░░██║░░░██╔═══╝░██╔══╝░░██║░░██║██╔══██║██║░░░░░░╚═══██╗
██████╔╝██║░░░░░╚█████╔╝░░░██║░░░██║░░░░░███████╗██████╔╝██║░░██║███████╗██████╔╝
╚═════╝░╚═╝░░░░░░╚════╝░░░░╚═╝░░░╚═╝░░░░░╚══════╝╚═════╝░╚═╝░░╚═╝╚══════╝╚═════╝░



Copyright (c) 2020 Eugenio Ciullo


Gli sketch in questo repository sono stati creati da Eugenio Ciullo.
Verrà creato un Controller HID disposto di 3 Assi funzionanti attraverso 3 potenziometri collegati in A0 A1 e A3.

Bisogna installare nella Documents/Arduino directory la libreria ArduinoJoystickLibrary creata da MHeironimus reperibile al seguente link:
https://github.com/MHeironimus/ArduinoJoystickLibrary

Buon Divertimento
*/


#include "HX711.h"
#include <Joystick.h>

int Acceleratore = 0;
int Freno = 0;
int Frizione = 0;
int Frenoamano = 0;

const int LOADCELL_DOUT_PIN1 = 4;
const int LOADCELL_SCK_PIN1 = 5;
const int LOADCELL_DOUT_PIN2 = 8;
const int LOADCELL_SCK_PIN2 = 9;

// Se volete calibrare il range degli assi, sostituire i seguenti valori
const int ZonainferioreAcceleratore = 0;
const int ZonasuperioreAcceleratore = 1023;
const int ZonainferioreFreno = 0;
const int ZonasuperioreFreno = 1023;
const int ZonainferioreFrizione = 0;
const int ZonasuperioreFrizione = 1023;
const int ZonainferioreFrenoamano = 0;
const int ZonasuperioreFrenoamano = 1023;

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,JOYSTICK_TYPE_GAMEPAD,
  0, 0,                  // Button Count, Hat Switch Count
  false, false, true,     // X and Y, but no Z Axis
  true, true, true,   // No Rx, Ry, or Rz
  false, false,          // No rudder or throttle
  false, false, false);  // No accelerator, brake, or steering

const long calibration_factor1 = 100;
const long calibration_factor2 = 100;

HX711 scale1;
HX711 scale2;

void setup() {
  
  scale1.begin(LOADCELL_DOUT_PIN1, LOADCELL_SCK_PIN1);
  scale2.begin(LOADCELL_DOUT_PIN2, LOADCELL_SCK_PIN2);
  
  scale1.set_scale(calibration_factor1);
  scale2.set_scale(calibration_factor2);  
  
  Joystick.begin();
  
  Joystick.setRxAxisRange(ZonainferioreAcceleratore, ZonasuperioreAcceleratore);
  Joystick.setRyAxisRange(ZonainferioreFreno, ZonasuperioreFreno);
  Joystick.setRzAxisRange(ZonainferioreFrizione, ZonasuperioreFrizione);
  Joystick.setZAxisRange(ZonainferioreFrenoamano, ZonasuperioreFrenoamano);
  
//decommentare il prossimo valore se si vuole aprire la seriale

 Serial.begin(9600);

}

const bool initAutoSendState = true;

void loop() {
	
  Acceleratore = analogRead(A0);
  Freno = scale1.get_units();
  Frizione = analogRead(A2);
  Frenoamano = scale2.get_units();
 
 Joystick.setRxAxis(Acceleratore);
  Joystick.setRyAxis(Freno);
  Joystick.setRzAxis(Frizione);
  Joystick.setZAxis(Frenoamano);
  
// Decommentare le seriali eliminando /* e */ alla fine dei Serial.print

    Serial.print("Acceleratore : \t");
    Serial.print(Acceleratore);
    Serial.print("\t Freno: \t");
    Serial.print(Freno);
    Serial.print("\t Frizione : \t");
    Serial.print(Frizione);  
    Serial.print("\t Freno a mano : \t");
    Serial.println(Frenoamano);
	
  delay(5);
}
