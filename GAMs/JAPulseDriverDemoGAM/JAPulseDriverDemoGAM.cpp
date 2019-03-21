/**
 * @file JAPulseDriverDemoGAM.cpp
 * @brief Source file for class JAPulseDriverDemoGAM
 * @date Nov 26, 2018
 * @author aneto
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
 * the class JAPulseDriverDemoGAM (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "JAPulseDriverDemoGAM.h"

#include "AdvancedErrorManagement.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

JAPulseDriverDemoGAM::JAPulseDriverDemoGAM() {
    lastPulseTrigger = 0;
    triggerUntilTime = 0;
    onValue = 0;
    offValue = 0;
    currentTime = NULL_PTR(MARTe::uint32 *);
    pulseLengthTime = NULL_PTR(MARTe::uint32 *);
    pulseTrigger = NULL_PTR(MARTe::uint32 *);
    output = NULL_PTR(MARTe::uint8 *);
}

JAPulseDriverDemoGAM::~JAPulseDriverDemoGAM() {
}

bool JAPulseDriverDemoGAM::Initialise(MARTe::StructuredDataI & data) {
    using namespace MARTe;
    bool ok = GAM::Initialise(data);
    if (ok) {
        ok = data.Read("OnValue", onValue);
        if (!ok) {
            REPORT_ERROR(MARTe::ErrorManagement::ParametersError, "The OnValue shall be specified");
        }
    }
    if (ok) {
        ok = data.Read("OffValue", offValue);
        if (!ok) {
            REPORT_ERROR(MARTe::ErrorManagement::ParametersError, "The OffValue shall be specified");
        }
    }
    return ok;
}

bool JAPulseDriverDemoGAM::PrepareNextState(const MARTe::char8 * const currentStateName, const MARTe::char8 * const nextStateName) {

    return true;
}

bool JAPulseDriverDemoGAM::Setup() {
    using namespace MARTe;
    bool ok = (numberOfInputSignals == 3u);
    if (ok) {
        ok = (numberOfOutputSignals == 1u);
        if (!ok) {
            REPORT_ERROR(MARTe::ErrorManagement::ParametersError, "One output signal shall be defined");
        }
    }
    else {
        REPORT_ERROR(MARTe::ErrorManagement::ParametersError, "Three input signals shall be defined");
    }
    if (ok) {
        uint32 c;
        for (c=0u; c<numberOfInputSignals; c++) {
            TypeDescriptor inputType = GetSignalType(InputSignals, c);
            ok = (inputType == UnsignedInteger32Bit);
            if (!ok) {
                StreamString signalName;
                (void) GetSignalName(InputSignals, c, signalName);
                REPORT_ERROR(MARTe::ErrorManagement::ParametersError, "Signal %s shall be defined as uint32", signalName.Buffer());
            }
        }
    }
    if (ok) {
        TypeDescriptor outputTime = GetSignalType(OutputSignals, 0);
        ok = (outputTime == UnsignedInteger8Bit);
        if (!ok) {
            StreamString signalName;
            (void) GetSignalName(OutputSignals, 0, signalName);
            REPORT_ERROR(MARTe::ErrorManagement::ParametersError, "Signal %s shall be defined as uint8", signalName.Buffer());
        }
    }
    if (ok) {
        currentTime = reinterpret_cast<uint32 *>(GetInputSignalMemory(0));
        pulseLengthTime = reinterpret_cast<uint32 *>(GetInputSignalMemory(1));
        pulseTrigger = reinterpret_cast<uint32 *>(GetInputSignalMemory(2));
        output = reinterpret_cast<uint32 *>(GetOutputSignalMemory(0));
    }
    return ok;
}

bool JAPulseDriverDemoGAM::Execute() {
    using namespace MARTe;
    if ((*pulseTrigger == 1) && (lastPulseTrigger == 0)) {
        //TODO possible overflow of time ignored
        triggerUntilTime = (*currentTime + pulseLengthTime);
    }
    bool trigger = (*pulseTrigger != 0);
    if (trigger) {
        trigger = (*currentTime < triggerUntilTime);
    }
    if (trigger) {
        *output = onValue;
    }
    else {
        triggerUntilTime = 0;
        *output = offValue;
    }
    lastPulseTrigger = *pulseTrigger;
    return true;
}

CLASS_REGISTER(JAPulseDriverDemoGAM, "1.0")
