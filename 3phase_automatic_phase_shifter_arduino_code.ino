#include <LiquidCrystal.h> // Library for LCD
LiquidCrystal lcd(13, 12, 11, 10, 9, 8); // LCD initialization

#include "EmonLib.h"   // Include Emon Library
EnergyMonitor emon1;   // Create an instance for Phase R
EnergyMonitor emon2;   // Create an instance for Phase Y
EnergyMonitor emon3;   // Create an instance for Phase B

const int D2Pin = 2; // Digital pin for Phase R
const int D4Pin = 4; // Digital pin for Phase Y
const int D7Pin = 7; // Digital pin for Phase B

void setup()
{
  emon1.voltage(A0, 187, 1.7); // Voltage measurement for Phase R
  emon2.voltage(A1, 187, 1.7); // Voltage measurement for Phase Y
  emon3.voltage(A2, 187, 1.7); // Voltage measurement for Phase B
  lcd.begin(20, 4); // Initialize the LCD's number of columns and rows

  // Set up initial LCD text (if needed)
  lcd.setCursor(0, 0);
  lcd.print("Phase R Voltage: ");
  lcd.setCursor(0, 1);
  lcd.print("Phase Y Voltage: ");
  lcd.setCursor(0, 2);
  lcd.print("Phase B Voltage: ");
  pinMode(D2Pin, OUTPUT);
  pinMode(D4Pin, OUTPUT);
  pinMode(D7Pin, OUTPUT);
  digitalWrite(D2Pin, LOW);
  digitalWrite(D4Pin, LOW);
  digitalWrite(D7Pin, LOW);
}

void loop()
{
  // Calculate all phases' voltage
  emon1.calcVI(20, 2000); // Phase R
  emon2.calcVI(20, 2000); // Phase Y
  emon3.calcVI(20, 2000); // Phase B

  // Read voltage values for all phases
  float VoltageR = emon1.Vrms;  // Voltage for Phase R
  float VoltageY = emon2.Vrms;  // Voltage for Phase Y
  float VoltageB = emon3.Vrms;  // Voltage for Phase B

  // Update the LCD display with the voltage values for all phases
  lcd.setCursor(16, 0);
  lcd.print("    "); // Clear the previous value
  lcd.setCursor(16, 1);
  lcd.print("    ");
  lcd.setCursor(16, 2);
  lcd.print("    ");

  lcd.setCursor(16, 0);
  lcd.print(VoltageR, 1);
  lcd.setCursor(16, 1);
  lcd.print(VoltageY, 1);
  lcd.setCursor(16, 2);
  lcd.print(VoltageB, 1);

  // Control digital pins based on voltage thresholds and highest voltage
  if (VoltageR >= 100 && VoltageR <= 250) {
    digitalWrite(D2Pin, HIGH);
  } else {
    digitalWrite(D2Pin, LOW);
  }

  if (VoltageY >= 100 && VoltageY <= 250) {
    digitalWrite(D4Pin, HIGH);
  } else {
    digitalWrite(D4Pin, LOW);
  }

  if (VoltageB >= 100 && VoltageB <= 250) {
    digitalWrite(D7Pin, HIGH);
  } else {
    digitalWrite(D7Pin, LOW);
  }

  // Determine the highest voltage among R, Y, and B
  float highestVoltage = max(max(VoltageR, VoltageY), VoltageB);

  // Control digital pins based on the highest voltage
  if (VoltageR == highestVoltage) {
    digitalWrite(D2Pin, HIGH);
  } else {
    digitalWrite(D2Pin, LOW);
  }

  if (VoltageY == highestVoltage) {
    digitalWrite(D4Pin, HIGH);
  } else {
    digitalWrite(D4Pin, LOW);
  }

  if (VoltageB == highestVoltage) {
    digitalWrite(D7Pin, HIGH);
  } else {
    digitalWrite(D7Pin, LOW);
  }

  // You can add delays or additional code for further processing here
  if (highestVoltage == VoltageR) {
    lcd.setCursor(0, 3);
    lcd.print("Phase R Online   ");
  } else if (highestVoltage == VoltageY) {
    lcd.setCursor(0, 3);
    lcd.print("Phase Y Online   ");
  } else if (highestVoltage == VoltageB) {
    lcd.setCursor(0, 3);
    lcd.print("Phase B Online   ");
  }
}
