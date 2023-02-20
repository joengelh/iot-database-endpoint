void setup()
{

    Serial.begin(9600); // Begin serial output
}

void loop()
{

    float Vi = 0.1;

    float Vc = 4.95; // Constant voltage from pin

    int sensorValue = analogRead(A0); // Read analog pin for voltage info in 10 bits

    float Vf = sensorValue * 4.95 / 1023; // Voltage through pin when in presence of VOCs (formaldehyde)

    Serial.print("Vi: ");

    Serial.println(sensorValue * 4.95 / 1023);

    Serial.print("Formaldehyde Concentration: ");

    Serial.print(concentrationPPMform(Vf, Vi));

    Serial.print("ppm\n"); // Print concentration data

    delay(500); // Delay 0.5 seconds
}

float concentrationPPMform(float V, float Vstart) // Formaldehde concentration formula

{

    float con = 212.32619543773774 * exp(-(7.653015806367451 * (4.95 - V) * Vstart) / (V * (4.95 - Vstart)));

    return con;
}
