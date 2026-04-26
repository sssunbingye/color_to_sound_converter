#include "mbed.h"
#include "color_tcs34725.h"

#define I2C_SDA_PIN PA_10 // D0
#define I2C_SCL_PIN PA_9 // D1
#define INT_PIN PA_0 // A0
PwmOut buzzer(D6);

void PlayTone(int frequency) {
    // the unit of frequency is Hz
    buzzer.period(1.0f / frequency); // according to the formula period = 1 / frequency
    buzzer.write(0.5f); // 50% duty cycle
    wait_us(300000); // last for 0.3 second
    buzzer.write(0.0f); // stop
    wait_us(10000); // last for 0.01 second
}

int main() {
    I2C i2c(I2C_SDA_PIN, I2C_SCL_PIN);
    i2c.frequency(400000); // the frequency of I2C is 400kHz
    Color colorSensor;

    while (1) {
        colorSensor.UpdateCRGB();

        uint16_t R, G, B; // range from 0 to 65535
        R = colorSensor.getR();
        G = colorSensor.getG();
        B = colorSensor.getB();

        // convert RGB to frequency (range from 262Hz to 1976Hz)
        // In music scale, 262Hz to 1976Hz corresponds to C4 to B6
        int frequency_R = 262 + (R * 1715) / 65535;
        int frequency_G = 262 + (G * 1715) / 65535;
        int frequency_B = 262 + (B * 1715) / 65535;

        //One color corresponds to one arpeggio
        PlayTone(frequency_R);
        PlayTone(frequency_G);
        PlayTone(frequency_B);
        wait_us(500000); // wait for 0.5 second
    }
}
