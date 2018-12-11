/**
 * @file RandomDataSource.cpp
 * @brief Source file for class RandomDataSource
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
 * the class RandomDataSource (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "CompilerTypes.h"
#include "RandomDataSource.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

RandomDataSource::RandomDataSource() :
        MARTe::DataSourceI() {
    using namespace MARTe;
    seed = 0u;
    signalPtr = NULL_PTR(MARTe::char8 *);
    signalTypeDescriptor = UnsignedInteger8Bit;
}

RandomDataSource::~RandomDataSource() {
    using namespace MARTe;
    if (signalPtr) {
        delete[] signalPtr;
    }
}

bool RandomDataSource::SetConfiguredDatabase(MARTe::StructuredDataI & data) {
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
        signalTypeDescriptor = GetSignalType(0u);
        ok = (signalTypeDescriptor.type == UnsignedInteger);
        if (!ok) {
            ok = (signalTypeDescriptor.type == SignedInteger);
        }
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "GetSignalType(0u) != Un/SignedInteger");
        }
    }

    return ok;
}

bool RandomDataSource::Initialise(MARTe::StructuredDataI & data) {
    using namespace MARTe;
    bool ok = DataSourceI::Initialise(data);
    if (ok) {
        ok = data.Read("Seed", seed);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The Seed shall be specified");
        }
    }

    return ok;
}

bool RandomDataSource::Synchronise() {
    using namespace MARTe;

    if (signalTypeDescriptor.numberOfBits == 8u) {
        GetValue<uint8>();
    }
    if (signalTypeDescriptor.numberOfBits == 16u) {
        GetValue<uint16>();
    }
    if (signalTypeDescriptor.numberOfBits == 32u) {
        GetValue<uint32>();
    }
    if (signalTypeDescriptor.numberOfBits == 64u) {
        GetValue<uint64>();
    }

    return true;
}

bool RandomDataSource::AllocateMemory() {
    signalPtr = new MARTe::char8[signalTypeDescriptor.numberOfBits];
    return true;
}

bool RandomDataSource::GetSignalMemoryBuffer(const MARTe::uint32 signalIdx, const MARTe::uint32 bufferIdx, void *&signalAddress) {
    signalAddress = &signalPtr[0];
    return true;
}

const MARTe::char8 *RandomDataSource::GetBrokerName(MARTe::StructuredDataI &data, const MARTe::SignalDirection direction) {
    using namespace MARTe;

    static bool firstTime = true;
    const char8 * broker;// = NULL_PTR(const char8 *);
    if (firstTime) {
        broker = "MemoryMapSynchronisedInputBroker";
    }
    else {
        firstTime = false;
        broker = "MemoryMapInputBroker";
    }
    return broker;
}

bool RandomDataSource::PrepareNextState(const MARTe::char8 * const currentStateName, const MARTe::char8 * const nextStateName) {
    return true;
}

CLASS_REGISTER(RandomDataSource, "1.0")

