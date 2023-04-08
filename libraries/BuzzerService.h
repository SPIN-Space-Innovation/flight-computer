#pragma once

#include <stdint.h>

class BuzzerService
{
    private:
        uint8_t _buzzerPin = 17;
        bool _canTurnOn = true;
        bool _locked = false;

        BuzzerService();
    public:
        BuzzerService(const BuzzerService& obj) = delete;
        BuzzerService& operator=(const BuzzerService& obj) = delete;

        BuzzerService(BuzzerService&& deadObj) = default;
        BuzzerService& operator=(BuzzerService&& deadObj) = default;

        static BuzzerService& GetInstance();

        void Lock();
        void Unlock();
        
        void TurnOn();
        void TurnOff();

        void CanTurnOn();
        void CannotTurnOn();
    
        ~BuzzerService();
};