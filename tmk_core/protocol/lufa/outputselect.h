/*
Copyright 2017 Priyadi Iman Nurcahyo
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <stdint.h>

enum outputs {
    OUTPUT_AUTO,

    OUTPUT_NONE,
    OUTPUT_USB,
<<<<<<< HEAD
    OUTPUT_BLUETOOTH,
    OUTPUT_NRF24,

    // backward compatibility
    OUTPUT_USB_AND_BT,
    OUTPUT_USB_AND_NRF24
=======
    OUTPUT_BLUETOOTH
>>>>>>> e864786c0b64423aa2fa83f2cc8a2049af9957fd
};

#ifndef OUTPUT_DEFAULT
#    define OUTPUT_DEFAULT OUTPUT_AUTO
#endif

void    set_output(uint8_t output);
void    set_output_user(uint8_t output);
uint8_t auto_detect_output(void);
uint8_t where_to_send(void);
