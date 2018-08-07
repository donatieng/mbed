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

#ifndef MBED_NFC_CONTROLLER_DRIVER_H
#define MBED_NFC_CONTROLLER_DRIVER_H

#include <stdint.h>
#include "events/EventQueue.h"

#include "stack/nfc_errors.h"
#include "stack/transceiver/transceiver.h"
#include "stack/platform/scheduler.h"

namespace mbed {
namespace nfc {

    /**
     * @addtogroup nfc
     * @{
     */
    
    /**
     * The abstraction for a NFC controller driver.
     * Implementers need to derive from this class and implement its methods.
     */ 
    struct NFCControllerDriver {
        /**
         * Initialize the driver and retrieve the interface to the controller.
         * 
         * @param[in] pTimer The MicroNFC timer instance for the scheduler to use
         * @return an initialized MicroNFC transceiver_t instance
         */ 
        virtual transceiver_t* initialize(scheduler_timer_t* timer) = 0;

        /**
         * Retrieve list of technologies supported by the controller
         * @param[out] initiator bitmask of technologies supported when the controller is in initiator mode
         * @param[out] target bitmask of technologies supported when the controller is in target mode
         */ 
        virtual void get_supported_nfc_techs(nfc_tech_t* initiator, nfc_tech_t* target) const = 0;
    };

    /**
     * @}
     */
    
} // namespace nfc
} // namespace mbed

#endif