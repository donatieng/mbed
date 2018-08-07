/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef MBED_NFC_EEPROM_H
#define MBED_NFC_EEPROM_H

#include <stdint.h>

#include "NFCDefinitions.h"
#include "NFCTarget.h"
#include "NFCEEPROMDriver.h"

namespace mbed {
namespace nfc {

    /**
     * @addtogroup nfc
     * @{
     */

    /** 
     * The NFC EEPROM class represents a NFC target device connected using a wired
     * link (I2C, SPI, etc).
     * 
     * These EEPROMs essentially provide addressable memory that can be accessed
     * using either a wired or NFC interface.
     * 
     * In NFC mode these most often conform to one of the NFC tag types defined 
     * by the NFC Forum, therefore encoding NDEF data in these EEPROMs will 
     * ensure that it is understandable by a NFC reader.
     */ 
    class NFCEEPROM : public NFCTarget, public NFCEEPROMDriver::Delegate {
    public:
        /** 
         * Construct a NFCEEPROM instance.
         * 
         * @param[in] driver a pointer to a NFCEEPROMDriver instance
         */  
        NFCEEPROM(NFCEEPROMDriver* driver);

        virtual ~NFCTarget();

        /**
         * The NFCEEPROM delegate. Users of the NFCEEPROM class need to implement this delegate's methods to receive events.
         */
        struct Delegate : NFCTarget::Delegate {
        };

        /** 
         * Set the delegate that will receive events generated by this EEPROM.
         * 
         * @oaram[in] delegate the delegate instance to use
         */ 
        void set_delegate(Delegate* delegate);

    private:
        // Implementation of NFCEEPROMDriver::Delegate
        virtual void has_started_session(bool success);
        virtual void has_read_bytes(bool success);
        virtual void has_written_bytes(bool success);
        virtual void has_set_size(bool success);
        virtual void has_gotten_size(bool success, size_t size);
        virtual void has_erased_bytes(bool success);
    };

    /**
     * @}
     */


} // namespace nfc
} // namespace mbed

#endif