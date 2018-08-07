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

#ifndef MBED_NFC_REMOTE_ENDPOINT_H
#define MBED_NFC_REMOTE_ENDPOINT_H

#include <stdint.h>

#include "NFCDefinitions.h"

namespace mbed {
namespace nfc {

    /**
     * @addtogroup nfc
     * @{
     */
    
    /**
     * This is the base class for all remote endpoints (initiators and targets)
     * addressable over the air interface.
     */ 
    class NFCRemoteEndpoint {
    public:

        /**
         * The NFCRemoteEndpoint base delegate.
         */
        struct Delegate {
            /**
             * This method is called when the endpoint is lost (air interface link disconnnected)
             */
            virtual void on_lost() {};           
        };

        /**
         * Check if the endpoint is lost.
         * @return whether the endpoint is lost
         */ 
        bool is_lost() const;

        /**
         * Get the list of RF protocols supported and activated over the air interface.
         * @return a bitmask of activated protocols
         */ 
        nfc_rf_protocols_bitmask_t rf_protocols() const;
    };

    /**
     * @}
     */

} // namespace nfc
} // namespace mbed

#endif