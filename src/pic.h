#pragma once

void PICInit(void);
void PICEnable(void);
void PICDisable(void);
void PICSendEOI(uint8_t n);
