#include <PID_v1.h>
#include <Wire.h>
#include <Adafruit_MLX90614.h>

Adafruit_MLX90614 mlx = Adafruit_MLX90614();
int pinVoltmeter=A0;
int PWM_pin=3;
int Vref=5;

double timer=0;
//***********Constants thermistance***************//
//double R2=9980;
//double A=1.4091*0.001;
//double B=1.7779*0.0001;
//double C=2.6782*0.0000001;
//
double temperature_read;
double temperature_mean;

//**Constants PID*****//
int kp=50; int ki=25; int kd=0;
float PID_error=0;
double PID_value=0;

//String freezeStr="";
//long freeze=0;
String PID_frozeStr="";
long PID_froze=150;

double set_temperature=29.65;
PID myPID(&temperature_mean, &PID_value, &set_temperature, kp, ki, kd,DIRECT);


void setup() {
  Serial.begin(9600);
  myPID.SetMode(AUTOMATIC);
  mlx.begin();
  //Serial.print(kp); Serial.print(", "); Serial.print(ki); Serial.print(", "); Serial.print(kd);
}

void loop() {
  //*******read temperature during 10 seconds and mean********//
  temperature_read = mlx.readObjectTempC();
  for (int i=1; i<10; i++){
    delay(1000);
    temperature_read=temperature_read+mlx.readObjectTempC();}
  temperature_mean=(temperature_read)/10;

  //*******Compute PID********//
  if (temperature_mean>0 && temperature_mean<40){// && freeze==0){
    myPID.Compute();
    analogWrite(PWM_pin,PID_value);
  }

//else{
//  PID_value=PID_froze;
//  analogWrite(PWM_pin,PID_value);
//}

  //*******print temperature********//
  Serial.print(temperature_mean);
  Serial.print(", ");
  Serial.print(mlx.readAmbientTempC());
  //*******print PID value********//
  Serial.print(", ");
  Serial.println(PID_value);
  //delay(500);
  
}
