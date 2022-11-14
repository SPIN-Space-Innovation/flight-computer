#include <SPIN-Logger.hpp>

#include "Wire.h"
#include "Arduino.h"
#include "backup_deployer.h"

extern SPIN::Log::CFormattedLogger<1024> logger;

void BackupDeployer::setup() {
  Wire.begin();
  Wire.beginTransmission(BACKUP_DEPLOYER_ADDRESS);
  Wire.write(IDLE_COMMAND);
  Wire.endTransmission();
}

void BackupDeployer::startArmCountdown() {
  Wire.begin();
  Wire.beginTransmission(BACKUP_DEPLOYER_ADDRESS);
  Wire.write(ARM_COUNTDOWN_COMMAND);
  Wire.endTransmission();
}

void BackupDeployer::resetCountdown() {
  Wire.begin();
  Wire.beginTransmission(BACKUP_DEPLOYER_ADDRESS);
  Wire.write(DISARM_COMMAND);
  Wire.endTransmission();
}

uint16_t BackupDeployer::getStatus() {
  uint16_t response = 0;
  Wire.begin();
  Wire.requestFrom(BACKUP_DEPLOYER_ADDRESS, 2);
  while (Wire.available()) {
    response = response << 8;
    response |= Wire.read();
  }
  return response;
}
