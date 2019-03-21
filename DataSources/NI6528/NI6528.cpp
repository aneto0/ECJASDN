/**
 * @file NI6528.cpp
 * @brief Source file for class NI6528
 * @date 01/03/2017
 * @author Andre Neto
 *
 * @copyright Copyright 2015 F4E | European Joint Undertaking for ITER and
 * the Development of Fusion Energy ('Fusion for Energy').
 * Licensed under the EUPL, Version 1.1 or - as soon they will be approved
 * by the European Commission - subsequent versions of the EUPL (the "Licence")
 * You may not use this work except in compliance with the Licence.
 * You may obtain a copy of the Licence at: http://ec.europa.eu/idabc/eupl
 *
 * @warning Unless required by applicable law or agreed to in writing, 
 * software distributed under the Licence is distributed on an "AS IS"
 * basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the Licence permissions and limitations under the Licence.

 * @details This source file contains the definition of all the methods for
 * the class NI6528 (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "CompilerTypes.h"
#include "NI6528.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

NI6528::NI6528() :
        MARTe::DataSourceI() {
    using namespace MARTe;
    value = 0u;
    port = 0u;
    boardFileDescriptor = 0;
}

NI6528::~NI6528() {
    using namespace MARTe;
    (void) pxi6528_close_device(boardFileDescriptor);
}

bool NI6528::SetConfiguredDatabase(MARTe::StructuredDataI & data) {
    using namespace MARTe;
    bool ok = (DataSourceI::SetConfiguredDatabase(data));
    if (!ok) {
        REPORT_ERROR(ErrorManagement::ParametersError, "DataSourceI::SetConfiguredDatabas() failed");
    }
    if (ok) {
        ok = (GetNumberOfSignals() == 1u);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "GetNumberOfSignals() != 1u");
        }
    }
    if (ok) {
        ok = (GetSignalType(0u) == UnsignedInteger8Bit);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "GetSignalType(0u) != UnsignedInteger8Bit");
        }
    }

    return ok;
}

bool NI6528::Initialise(MARTe::StructuredDataI & data) {
    using namespace MARTe;
    bool ok = DataSourceI::Initialise(data);
    if (ok) {
        ok = data.Read("Port", port);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The Port shall be specified");
        }
    }
    if (ok) {
        ok = data.Read("DeviceName", deviceName);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The DeviceName shall be specified");
        }
    }
    int32 ret = pxi6528_open_device(&boardFileDescriptor, deviceName.Buffer(), O_NONBLOCK);
    ok = (ret == 0);
    if (!ok) {
        StreamString err = strerror(-ret);
        REPORT_ERROR(ErrorManagement::FatalError, "Could not open device (%s) : %s", deviceName.Buffer(), err.Buffer());
    }
    return ok;
}

bool NI6528::Synchronise() {
    using namespace MARTe;
    int32 ret = (pxi6528_write_port(boardFileDescriptor, port, value) > 0);
    bool ok = (ret > -1);
    if (!ok) {
        StreamString err = strerror(-ret);
        REPORT_ERROR(ErrorManagement::FatalError, "Could not write to device (%s) : %s", deviceName.Buffer(), err.Buffer());
    }

    return true;
}

bool NI6528::AllocateMemory() {
    return true;
}

bool NI6528::GetSignalMemoryBuffer(const MARTe::uint32 signalIdx, const MARTe::uint32 bufferIdx, void *&signalAddress) {
    signalAddress = &value;
    return true;
}

const MARTe::char8 *NI6528::GetBrokerName(MARTe::StructuredDataI &data, const MARTe::SignalDirection direction) {
    using namespace MARTe;
    return "MemoryMapSynchronisedOutputBroker";
}

bool NI6528::PrepareNextState(const MARTe::char8 * const currentStateName, const MARTe::char8 * const nextStateName) {
    return true;
}

CLASS_REGISTER(NI6528, "1.0")

