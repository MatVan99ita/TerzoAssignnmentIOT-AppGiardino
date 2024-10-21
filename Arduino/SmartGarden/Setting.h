#define SERVO_MOTOR_PIN 7 //Servo pin

// Bluetooth pin
#define BLUE_RXD 2
#define BLUE_TXD 3

//Led pin
#define PIN_LED_SWITCH1 12 // digital
#define PIN_LED_SWITCH2 11 // digital
#define PIN_LED_FADE1 10 // analog
#define PIN_LED_FADE2 9 // analog


extern String led_type; // Used on manual mode
extern int led_id; // = {1, 2, 3}
extern bool led_max_on; // Used for led, switched on or max faded
extern int fade_value; // Led fade value based on light detected
extern int servo_speed; // Servo speed based on temperature
