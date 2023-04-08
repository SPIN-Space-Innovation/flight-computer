#include "BuzzerService.h"

#include <stdint.h>

#include <Arduino.h>

static BuzzerService* buzzerService = (BuzzerService*)NULL;

BuzzerService::BuzzerService()
{
    pinMode(this->_buzzerPin, OUTPUT);
}

BuzzerService& BuzzerService::GetInstance()
{
    bool buzzerServiceNotInitialized = (buzzerService == (BuzzerService*)NULL);

    if (buzzerServiceNotInitialized)
    {
        buzzerService = new BuzzerService();
    }

    return *buzzerService;
}

void BuzzerService::Lock()
{
    this->_locked = true;
}

void BuzzerService::Unlock()
{
    this->_locked = false;
}

void BuzzerService::TurnOn()
{
    if (!this->_canTurnOn || this->_locked)
    {
        return;
    }

    tone(this->_buzzerPin, 500);
}

void BuzzerService::TurnOff()
{
    if (this->_locked)
    {
        return;
    }

    noTone(this->_buzzerPin);
}

void BuzzerService::CanTurnOn()
{
    this->_canTurnOn = true;
}

void BuzzerService::CannotTurnOn()
{
    this->_canTurnOn = false;
    
    noTone(this->_buzzerPin);
}

BuzzerService::~BuzzerService() { }