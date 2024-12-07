#include <DHT.h>
#include <Servo.h>

// Definiciones de pines y tipo de sensor
#define BUZZER_PIN 8
#define DHTTYPE DHT11
#define DHTPIN 10
#define RED_PIN 5
#define GREEN_PIN 4
#define BLUE_PIN 3
#define SERVO_PIN 11

DHT dht(DHTPIN, DHTTYPE);
Servo servo;

float tmpDeg;         // Temperatura en Celsius
float tmpFar;         // Temperatura en Fahrenheit
int hum;              // Humedad relativa
int waterSensor;      // Nivel de agua

// Configuración inicial
void setup() {
    Serial.begin(9600);
    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(RED_PIN, OUTPUT);
    pinMode(GREEN_PIN, OUTPUT);
    pinMode(BLUE_PIN, OUTPUT);
    dht.begin();
    servo.attach(SERVO_PIN);
    delay(2000); // Tiempo para estabilizar los sensores
}

// Bucle principal
void loop() {
    readSensors();
    displayData();
    controlBuzzer();
    controlLED();
    controlServo();
    delay(2000); // Pausa entre lecturas
}

// Lee los valores de los sensores
void readSensors() {
    tmpDeg = dht.readTemperature();
    tmpFar = dht.readTemperature(true);
    hum = dht.readHumidity();
    waterSensor = analogRead(A0);
}

// Muestra los datos en la consola serial
void displayData() {
    if (isnan(tmpDeg) || isnan(tmpFar) || isnan(hum)) {
        Serial.println("Error al leer el sensor DHT");
        return;
    }

    Serial.print("Temperatura (C): ");
    Serial.print(tmpDeg);
    Serial.print(" - Temperatura (F): ");
    Serial.print(tmpFar);
    Serial.print(" - Humedad (%): ");
    Serial.print(hum);
    Serial.print(" - Cantidad de Agua (%): ");
    Serial.println(waterSensor);
}

// Controla el buzzer según la temperatura
void controlBuzzer() {
    if (tmpDeg > 30.0) {
        tone(BUZZER_PIN, 540, 1000); // Activa el buzzer si la temperatura es alta
    } else {
        noTone(BUZZER_PIN);
    }
}

void controlLED() {
    if (isnan(hum)) {
        Serial.println("Error: Lectura de humedad no válida.");
        setColor(0, 0, 255); // Azul como error
        return;
    }

    if (hum < 30) {
        setColor(255, 0, 0); // Rojo: Humedad muy baja
    } else if (hum >= 30 && hum < 60) {
        setColor(255, 255, 0); // Amarillo: Humedad moderada
    } else {
        setColor(0, 255, 0); // Verde: Humedad alta
    }
}


// Controla el movimiento del servo según el nivel de agua
void controlServo() {
    if (waterSensor < 150) {
        servo.write(190); // Posición para poca agua
    } else {
        servo.write(-300); // Posición para suficiente agua
    }
}

// Configura el color del LED RGB
void setColor(int redValue, int greenValue, int blueValue) {
    analogWrite(RED_PIN, redValue);
    analogWrite(GREEN_PIN, greenValue);
    analogWrite(BLUE_PIN, blueValue);
}
