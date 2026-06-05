#include <Servo.h>
#include <Wire.h>

#define led_alp_fre1 53
#define led_alp_fre2 52
#define led_alp_lad1 13
#define led_alp_lad2 12
#define led_alp_lad3 11
#define led_int1 10
#define led_int2 51

#define buzzer 35

#define sensor_fumaca A0


Servo servo_porta;

byte comando = -1;
unsigned long t = 0;

bool _luzes_alpendre_frente_ = false;
bool _luzes_alpendre_lado_ = false;
bool _luzes_internas_ = false;
bool _sensor_fumaca_ = true;
bool _controle_porta = false;

bool ligar_buzzer = false;


void controle_luzes();
void controle_ifs();
void controle_porta();
void controle_sensor_fumaca();
void receberDados(int numBytes);


void setup() {
  Wire.begin(8); 
  Wire.onReceive(receberDados);
  Serial.begin(115200);

  pinMode(led_alp_fre1, OUTPUT);
  pinMode(led_alp_fre2, OUTPUT);
  pinMode(led_alp_lad1, OUTPUT);
  pinMode(led_alp_lad2, OUTPUT);
  pinMode(led_alp_lad3, OUTPUT);
  pinMode(led_int1, OUTPUT);
  pinMode(led_int2, OUTPUT);

  pinMode(buzzer, OUTPUT);

  servo_porta.attach(50);
}

void loop() {
  if (comando != -1) {
    controle_ifs();
    comando = -1;
  }

  controle_luzes();
  controle_sensor_fumaca();
  controle_porta();
}

void controle_porta() {
  servo_porta.write(_controle_porta ? 20: 90);
}

void controle_sensor_fumaca() {
  if (!_sensor_fumaca_) return;
  
  int val = analogRead(sensor_fumaca);
  Serial.println(val);
  if (val >= 34){
    if (millis() - t >= 1000){
      if (ligar_buzzer == true){
        digitalWrite(buzzer, HIGH);
        ligar_buzzer = false;
      }
      else{
        digitalWrite(buzzer, LOW);
        ligar_buzzer = true;
      }

      t = millis();
    }
  }
}

void controle_luzes() {  
  digitalWrite(led_int1, _luzes_internas_ ? HIGH : LOW);
  digitalWrite(led_int2, _luzes_internas_ ? HIGH : LOW);

  digitalWrite(led_alp_fre1, _luzes_alpendre_frente_ ? HIGH : LOW);
  digitalWrite(led_alp_fre2, _luzes_alpendre_frente_ ? HIGH : LOW);
  
  digitalWrite(led_alp_lad1, _luzes_alpendre_lado_ ? HIGH : LOW);
  digitalWrite(led_alp_lad2, _luzes_alpendre_lado_ ? HIGH : LOW);
  digitalWrite(led_alp_lad3, _luzes_alpendre_lado_ ? HIGH : LOW);
}

void controle_ifs() {
  if (comando == 190) {_luzes_alpendre_frente_ = true; return;}
  if (comando == 191) {_luzes_alpendre_frente_ = false; return;}

  if (comando == 192) {_luzes_alpendre_lado_ = true; return;}
  if (comando == 193) {_luzes_alpendre_lado_ = false; return;}

  if (comando == 194) {_luzes_internas_ = true; return;}
  if (comando == 195) {_luzes_internas_ = false; return;}

  if (comando == 200) {_sensor_fumaca_ = true; return;}
  if (comando == 201) {_sensor_fumaca_ = false; return;}

  if (comando == 210) {_controle_porta = true; return;}
  if (comando == 211) {_controle_porta = false; return;}

}

void receberDados(int numBytes) {
  while (Wire.available()) {
    comando = Wire.read();
  }
}
