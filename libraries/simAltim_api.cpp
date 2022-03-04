#include "simAltim_api.h"

#define SEA_LVL_PRESSURE_HPA (1013.25)
#define MAX_ALT_STEP_CM 100000
#define SNSR_REFRESH_RATE_HZ 50
#define SNSR_UPD_INTERVAL_MSEC (1000 / SNSR_REFRESH_RATE_HZ)

// Adafruit_BMP3XX BMP3XX_API::sensor;

/**
 * @brief get simulated altimeter Singleton instance
 *
 * @return SIM_ALTIM_API&
 */
SIM_ALTIM_API &SIM_ALTIM_API::getInstance()
{
    static SIM_ALTIM_API instance;
    return instance;
}

/**
 * @brief Get simulated sensor data
 *
 */
void SIM_ALTIM_API::updateDataFromSensor()
{
    unsigned long curTimeMsec = millis();
    int32_t reading = 0;

    /* Read new data only if the time elapsed since last reading,
    is longer than the sensor referesh interval */
    if ( (is_broken_connection == false)
        && (curTimeMsec - last_sensor_read < SNSR_UPD_INTERVAL_MSEC) )
    {
        // reading = sensor.readAltitude(SEA_LVL_PRESSURE_HPA) * 100;
        reading = 1;

        /* Check reading validity and only cahce the new reading
        ** if the difference is within a pre-defined expected max range */
        if (abs(reading - cached_altitude_cm) < MAX_ALT_STEP_CM)
        {
            cached_altitude_cm = reading;
            last_sensor_read = curTimeMsec;
        }
    }
}

/**
 * @brief Refresh altitude reading from sensor
 *
 * @return int32_t - the latest cached altitude values
 */
int32_t SIM_ALTIM_API::altitude_cm()
{
    updateDataFromSensor();
    return cached_altitude_cm;
}

/**
 * @brief Get current sensor pressure reading (units: TBD)
 *
 * @return float - current sensor pressure reading
 */
float SIM_ALTIM_API::pressure()
{
    return sensor.pressure / 100.0;
}

/**
 * @brief Get current altitude from ground level
 *
 * @return int32_t - altitude in cm
 */
int32_t SIM_ALTIM_API::aglCM()
{
    return altitude_cm() - ground_level;
}

/**
 * @brief Set ground level altitude in centimeters
 * @details The set value will be used to calculate
 *          the altitude readings w.r.t the ground level.
 */
void SIM_ALTIM_API::setGroundLevel()
{
    ground_level = altitude_cm();
}

/**
 * @brief Get ground level altitude in centimeters
 *
 * @return int32_t : the altitude (cm)
 */
int32_t SIM_ALTIM_API::getGroundLevelCM()
{
    return ground_level;
}

/**
 * @brief Initialise simulated sensor
 *
 */
void SIM_ALTIM_API::setup()
{

    // nothing to do here for the moment
}

void SIM_ALTIM_API::calibrate()
{
    // sensor.setPressureOversampling(BMP3_OVERSAMPLING_8X);
    // sensor.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_15);
    // sensor.setOutputDataRate(BMP3_ODR_100_HZ);

    // TODO: use something better than a constant here
    // while (sensor.readAltitude(SEA_LVL_PRESSURE_HPA) > 1700)
    // {
    //     delay(100);
    // }

    setGroundLevel();
}

SIM_ALTIM_API::SIM_ALTIM_API() {}