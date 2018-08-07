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

#ifndef MBED_NFC_NDEF_CAPABLE_H
#define MBED_NFC_NDEF_CAPABLE_H

#include <stdint.h>

#include "NFCDefinitions.h"

namespace mbed {
namespace nfc {
    class NFCNDEFCapable {
    public:
        virtual bool is_ndef_supported() const { return false; }

        void set_ndef_message_parser(ndef::MessageParser* parser) const;
        void set_ndef_message_builder(ndef::MessageBuilder* builder) const;

    protected:
        ndef::MessageParser* ndef_message_parser();
        ndef::MessageBuilder* ndef_message_builder();

    private:
        bool _ndef_support;
        ndef::MessageParser* _message_parser;
        ndef::MessageBuilder* _message_builder;
    };
} // namespace nfc
} // namespace mbed

#endif