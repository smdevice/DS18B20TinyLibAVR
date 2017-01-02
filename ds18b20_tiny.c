#include "ds18b20_tiny.h"

/** Temperature structure */
union {
    int16_t w;
    uint8_t b[2];
} _temp;

/** Current resolution of the temperature sensor */
uint8_t _resolution;

uint8_t DS18B20_Init(uint8_t resolution) {
    _setPinIn();
    _setPinLow();

    return DS18B20_SetResolution(resolution);
}

uint8_t DS18B20_SetResolution(uint8_t resolution) {
    if (resolution < 9 || resolution > 12 || _skipRom()) {
        return 1;
    }

    _resolution = resolution;

    _write(DS18B20_CMD_WRITE_SCRATCHPAD);
    _write(0);
    _write(0);
    _write((resolution - 9) << 5);

    return 0;
}

uint8_t DS18B20_ConvertTemp() {
    if(_skipRom()) {
        return 1;
    }

    _write(DS18B20_CMD_CONVERT_T);

    return 0;
}

uint8_t DS18B20_IsTempReady() {
    return _io(1);
}

uint8_t DS18B20_ReadTemp() {
    if (_skipRom()) {
        return 1;
    }

    _write(DS18B20_CMD_READ_SCRATCHPAD);

    _temp.b[0] = _read();
    _temp.b[1] = _read();

    return 0;
}

float DS18B20_GetTemp() {
    return _temp.w / 16;
}

uint16_t DS18B20_GetRawTemp() {
    return _temp.w;
}

uint8_t _reset() {
    uint8_t error = 0;

    _setPinLow();
    _setPinOut();

    _delay_us(480);

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        _setPinIn();

        _delay_us(64);

        error = _getPinState();
    }

    _delay_us(480 - 64);

    if(!_getPinState()) {
        error = 1;
    }

    return error;
}

uint8_t _io(uint8_t bit) {
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        _setPinOut();
    
        _delay_us(2);
    
        if (bit) {
            _setPinIn();
            _delay_us(10);
            bit = _getPinState();
            _delay_us(50);
        } else {
            _delay_us(60);
            _setPinIn();
        }
    }

    _delay_us(2);

    return bit;
}

void _write(uint8_t byte) {
    for (uint8_t i = 0; i < 8; i++) {
        _io(byte & 1);
        byte >>= 1;
    }
}

uint8_t _read() {
    uint8_t byte = 0;

    for (uint8_t i = 0; i < 8; i++) {
        byte |= _io(1) << i;
    }

    return byte;
}

uint8_t _skipRom() {
    if (_reset()) {
        return 1;
    }
    _write(OW_CMD_SKIP_ROM);

    return 0;
}