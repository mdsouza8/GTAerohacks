#include <Stepper.h>
#include <string.h>

#include <string.h>
#include <SFE_BMP180.h>
#define MAXTIMINGS  85
#define HUMITPIN    11 
#define STEPS_PER_MOTOR_REVOLUTION 32   
#define STEPS_PER_OUTPUT_REVOLUTION 32 * 64  //2048  

#define CE 8
#define CSN 18
#define IRQ 38
#define GASPIN 2

//#define READ 0
//#define TOGGLE 1


//Enrf24 radio(CE, CSN, IRQ);
//const uint8_t txaddr[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0x01 };
const int N = 3;
//char cmd [33];
char cmd;
//const char* READ = "READ";
//const char* TOGGLE = "TOGGLE";

//char READ = "R";
//char TOGGLE = "T";

const int steps = 800;

int humit_dat[5] = { 0, 0, 0, 0, 0 };
int temperature, humidity;
bool drive_downwards = true;
SFE_BMP180 pressure;
Stepper winch(STEPS_PER_MOTOR_REVOLUTION, 27, 29, 28, 30);

void read_humit_dat()
{
    uint8_t laststate   = HIGH;
    uint8_t counter     = 0;
    uint8_t j       = 0, i;
    float   f; 
  
    humit_dat[0] = humit_dat[1] = humit_dat[2] = humit_dat[3] = humit_dat[4] = 0;
 
    pinMode( HUMITPIN, OUTPUT );
    digitalWrite( HUMITPIN, LOW );
    delay( 18 );
    digitalWrite( HUMITPIN, HIGH );
    delayMicroseconds( 40 );
    pinMode( HUMITPIN, INPUT );
 
    for ( i = 0; i < MAXTIMINGS; i++ )
    {
        counter = 0;
        while ( digitalRead( HUMITPIN ) == laststate )
        {
            counter++;
            delayMicroseconds( 1 );
            if ( counter == 255 )
            {
                break;
            }
        }
        laststate = digitalRead( HUMITPIN );
 
        if ( counter == 255 )
            break;
 
        if ( (i >= 4) && (i % 2 == 0) )
        {
            humit_dat[j / 8] <<= 1;
            if ( counter > 16 )
                humit_dat[j / 8] |= 1;
            j++;
        }
    }
 
    if ( (j >= 40) &&
         (humit_dat[4] == ( (humit_dat[0] + humit_dat[1] + humit_dat[2] + humit_dat[3]) & 0xFF) ) )
    {
        f = humit_dat[2] * 9. / 5. + 32;
//        Serial.print( "Humidity = ");
//        Serial.print(humit_dat[0]);
//        Serial.println(humit_dat[1]);
//
//        Serial.print( "Humidity[0] = ");
//        Serial.print(humit_dat[0]);
//        Serial.print( "Humidity[1] = ");
//        Serial.println(humit_dat[1]);
//        Serial.print("Temp[0[ = ");
//        Serial.print(humit_dat[2]);
//        Serial.print("Temp[1[ = ");
//        Serial.print(humit_dat[3]);
        
//        Serial.print("Temp = ");
//        Serial.print(humit_dat[2]);
//        Serial.print(humit_dat[3]);

//        humidity = humit_dat[0] * 10 
    }else  {
        Serial.println( "Data not good, skip\n" );
    }
}


float gas_read()
{
    int reading = analogRead(GASPIN);
    float sensor_volt = reading / (float) 16383 * 5;  //14bit ADC
    return sensor_volt;
}


double getPressure()
{
  char status;
  double T,P, p0, a;

  // You must first get a temperature measurement to perform a pressure reading.
  
  // Start a temperature measurement:
  // If request is successful, the number of ms to wait is returned.
  // If request is unsuccessful, 0 is returned.

  status = pressure.startTemperature();
  if (status != 0)
  {
    // Wait for the measurement to complete:

    delay(status);

    // Retrieve the completed temperature measurement:
    // Note that the measurement is stored in the variable T.
    // Use '&T' to provide the address of T to the function.
    // Function returns 1 if successful, 0 if failure.

    status = pressure.getTemperature(T);
    if (status != 0)
    {
      // Start a pressure measurement:
      // The parameter is the oversampling setting, from 0 to 3 (highest res, longest wait).
      // If request is successful, the number of ms to wait is returned.
      // If request is unsuccessful, 0 is returned.

      status = pressure.startPressure(1);
      if (status != 0)
      {
        // Wait for the measurement to complete:
        delay(status);

        // Retrieve the completed pressure measurement:
        // Note that the measurement is stored in the variable P.
        // Use '&P' to provide the address of P.
        // Note also that the function requires the previous temperature measurement (T).
        // (If temperature is stable, you can do one temperature measurement for a number of pressure measurements.)
        // Function returns 1 if successful, 0 if failure.

        status = pressure.getPressure(P,T);
        if (status != 0)
        {
          return(P);
        }
        else Serial.println("error retrieving pressure measurement\n");
      }
      else Serial.println("error starting pressure measurement\n");
    }
    else Serial.println("error retrieving temperature measurement\n");
  }
  else Serial.println("error starting temperature measurement\n");
}

void toggleMotor()
{
  if (drive_downwards){
    winch.step(steps);
  } else {
    winch.step(-steps);
  }
//  drive_downwards != drive_downwards;
  drive_downwards = !drive_downwards;
  Serial.print(drive_downwards);
  delay(10);
}


void setup() {
  Serial.begin(9600);
//  Serial.print("hi!");

  winch.setSpeed(700);
//  radio.begin();  // Defaults 1Mbps, channel 0, max TX power
  
  pressure.begin();
  
//  radio.setTXaddress((void*)txaddr);
  Serial.println("begun");
  delay(20);
}

void loop() {
  if (Serial.available()){
//    Serial.print("Serial Available");
    cmd = Serial.read();
    delay(20);
    switch ((int) cmd) {
      case 114:
        Serial.print("voltage: ");
        delay(20);
        Serial.println(gas_read(), DEC);
        delay(20);
        Serial.print("pressure: ");
        delay(20);
        Serial.println(getPressure(), DEC);
        delay(20);
        read_humit_dat();
        humidity = (humit_dat[0]*10) + humit_dat[1];
        temperature = (humit_dat[2]*10) + humit_dat[3];
        Serial.print("humidity: ");
        delay(20);
        Serial.println(humidity, DEC);
        delay(20);
        Serial.print("temperature: ");
        delay(20);
        Serial.println(temperature, DEC);
        delay(20);
        break;
      case 116:// T
        toggleMotor();
        break;
      case 97: // A
        Serial.print("Hello!");
        delay(10);
        break;
      default:
        break;
    } 
  }
  
//  delay(500);
}


