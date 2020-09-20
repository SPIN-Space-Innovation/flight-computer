#include <stdint.h>

#define BACKUP_DEPLOYER_ADDRESS 9
#define ARM_COUNTDOWN_COMMAND 1
#define DISARM_COMMAND 3
#define IDLE_COMMAND 4

class BackupDeployer {
  public:
    static void setup();
    static void startArmCountdown();
    static void resetCountdown();
    static uint16_t getStatus();
};
