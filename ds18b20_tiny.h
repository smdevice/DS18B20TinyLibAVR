/** @file ds18b20_tiny.h
 *  @brief Tiny AVR library for DS18B20
 *
 *  This library is used to perform reading the temperature from the DS18B20 device
 *
 *  @author Oleksandr Kovalenko
 */

#include <util/delay.h>
#include <util/atomic.h>

#ifndef SMDEV_DS18B20_TINY_H
#define SMDEV_DS18B20_TINY_H

/** @name Pin configuration
 * Configure these definitions before using the library
 */
///@{
#define OW_PORT_OUT PORTB
#define OW_PORT_IN  PINB
#define OW_PORT_DIR DDRB
#define OW_PIN      PB0
///@}

/** @name Constants
 * For internal use only
 */
///@{
#define OW_CMD_SKIP_ROM 0xCC

#define DS18B20_CMD_READ_SCRATCHPAD     0xBE
#define DS18B20_CMD_WRITE_SCRATCHPAD    0x4E
#define DS18B20_CMD_CONVERT_T           0x44

#define OW_PIN_MASK (1 << OW_PIN)
///@}

/** @name Pin macro definitions
 * For internal use only
 */
///@{
#define _setPinOut()    OW_PORT_DIR |= OW_PIN_MASK
#define _setPinIn()     OW_PORT_DIR &= ~OW_PIN_MASK
#define _setPinHigh()   OW_PORT_OUT |= OW_PIN_MASK
#define _setPinLow()    OW_PORT_OUT &= ~OW_PIN_MASK
#define _getPinState()  (OW_PORT_IN & OW_PIN_MASK)
///@}

/** Initializes the 1-Wire bus and DS18B20 sensor
 *
 *  @param[in] resolution The resolution of the temperature sensor.
 *  Possible values: 9, 10, 11, 12
 *  @return 1 if error, else 0
 */
uint8_t DS18B20_Init(uint8_t resolution);

/** Sets resolution of the temperature sensor
 *
 *  @param[in] resolution The resolution of the temperature sensor.
 *  Possible values: 9, 10, 11, 12
 *  @return 1 if error, else 0
 */
uint8_t DS18B20_SetResolution(uint8_t resolution);

/** Starts temperature conversion
 *
 *  @return 1 if error, else 0
 */
uint8_t DS18B20_ConvertTemp(void);

/** Checks if result of the temperature conversion is ready
 *
 *  Run this function after DS18B20_ConvertTemp() to check if conversion has been completed
 *
 *  @return 1 if temperature is ready, else 0
 */
uint8_t DS18B20_IsTempReady(void);

/** Reads temperature from the device
 *
 *  Run this function after conversion has been completed
 *
 *  @see DS18B20_IsTempReady()
 *  @return 1 if error, else 0
 */
uint8_t DS18B20_ReadTemp(void);

/** Gets temperature value
 *
 *  Run this function after DS18B20_ReadTemp()
 *
 *  @see DS18B20_ReadTemp()
 *  @return Current temperature
 */
float DS18B20_GetTemp(void);

/** Gets raw temperature value
 *
 *  Run this function after DS18B20_ReadTemp()
 *
 *  @see DS18B20_ReadTemp()
 *  @return Current raw temperature value
 */
uint16_t DS18B20_GetRawTemp(void);

/** Resets the 1-Wire bus slave devices and ready them for a command
 *
 *  @private
 */
uint8_t _reset(void);

/** Reads/Writes time slot
 *
 *  @param[in] bit The bit to write. Set this parameter to 1 to perform reading
 *  @return The bit which is read from the slave device in case of reading operation
 *  @private
 */
uint8_t _io(uint8_t bit);

/** Writes byte to the slave device
 *
 *  @param[in] byte The byte to write to the device
 *  @private
 */
void _write(uint8_t byte);

/** Reads byte from the slave device
 *
 *  @return The byte which is read from the slave device
 *  @private
 */
uint8_t _read(void);

/** Resets the bus and writes skip ROM command
 *
 *  @return 1 if error, else 0
 *  @private
 */
uint8_t _skipRom(void);

#endif