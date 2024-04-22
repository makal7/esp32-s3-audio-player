#include "encoder.hpp"

Encoder::Encoder(int CLKPin, int DTPin){
    pinMode(CLKPin, INPUT);
    pinMode(DTPin, INPUT);

    this->CLKPin = CLKPin;
    this->DTPin = DTPin;
}

void Encoder::selectOption(int &selectedOption, int maxPossibleNumberOfOptinos){
        int CLK_state = digitalRead(CLKPin);

        if (CLK_state != prevCLKState && CLK_state == LOW) 
        {
            if (digitalRead(DTPin) == HIGH) {
                if (selectedOption > 0 )
                    selectedOption--;
            } 
            else {
                if (selectedOption < maxPossibleNumberOfOptinos)
                    selectedOption++;
            }
        }

        prevCLKState = CLK_state;
}