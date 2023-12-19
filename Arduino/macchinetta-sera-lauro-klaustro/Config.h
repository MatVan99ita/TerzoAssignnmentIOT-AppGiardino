/** configurazione pin */
#define POT_PIN A0
#define PIN_MAKE 13
#define PIN_UP 12
#define PIN_DOWN 11
#define SERVO_MOTOR_PIN 2
#define PIR_PIN 3
#define SONAR_TRIG_PIN 8
#define SONAR_ECHO_PIN 7
#define SCL SCL
#define SDA SDA
#define PIN_TEMP A1

/** configurazione variabili */
#define WELCOME_TIME 4000
#define T_IDLE 60000
#define WAIT_TIME 5000
#define T_TIMEOUT 5000
#define T_CHECK 100000
#define TEMP_MIN 17
#define TEMP_MAX 24
extern int sugarLevel;
extern bool isChanged;
extern int currentProduct;
extern bool make;
extern int temperature_value;
extern bool isSleeping;
extern bool servoBusy;
extern bool isSugarSettable;
extern int quantity[3];
extern bool needAssistance;
extern bool needRefill;
extern bool canSleep;
