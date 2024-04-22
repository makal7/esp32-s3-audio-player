#pragma once
#include "Arduino.h"

class Encoder{
    public:
        Encoder(int CLKPin, int DTPin);
        void selectOption (int &selectedOption, int maxPossibleNumberOfOptinos);

    private:
        int CLKPin, DTPin;
        int prevCLKState;

};