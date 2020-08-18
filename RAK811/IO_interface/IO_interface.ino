
//811 supply 4 gpio for wisblock io_interface, Uart and I2C please refer to other example

int gpio_1 = PB2;
int gpio_2 = PA1;
int gpio_3 = PB3;
int gpio_4 = PB5;


void setup()
{
  Serial.begin(115200);
  while ( !Serial ) delay(10);    
  pinMode(gpio_1, OUTPUT);
  pinMode(gpio_2, OUTPUT);
  pinMode(gpio_3, OUTPUT);
  pinMode(gpio_4, OUTPUT);
}



void loop()
{
    Serial.println("gpio_1 test with a led!");
    digitalWrite(gpio_1, 1);
    delay(1000);
    Serial.println("gpio_1 test with a led!");
    digitalWrite(gpio_1, 0);    
    delay(1000);
}
