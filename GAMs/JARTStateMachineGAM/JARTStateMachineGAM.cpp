/**
 * @file JARTStateMachineGAM.cpp
 * @brief Source file for class JARTStateMachineGAM
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
 * the class JARTStateMachineGAM (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "JARTStateMachineGAM.h"

#include "AdvancedErrorManagement.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

JARTStateMachineGAM::JARTStateMachineGAM() {
    currentState = MyState1;

    lastPulseTrigger = 0;
    triggerUntilTime = 0;

    conditionTrigger = 1;
    output1 = 1;
    output2 = 2;

    triggerSignal = NULL_PTR(MARTe::uint32 *);
    currentTime = NULL_PTR(MARTe::uint32 *);
    pulseDuration1Signal = NULL_PTR(MARTe::uint32 *);
    pulseDuration2Signal = NULL_PTR(MARTe::uint32 *);

    outputSignal = NULL_PTR(MARTe::uint32 *);
}

JARTStateMachineGAM::~JARTStateMachineGAM() {
}

bool JARTStateMachineGAM::Initialise(MARTe::StructuredDataI & data) {
    using namespace MARTe;
    bool ok = GAM::Initialise(data);
    if (ok) {
        ok = data.Read("ConditionTrigger", conditionTrigger);
        if (!ok) {
            REPORT_ERROR(MARTe::ErrorManagement::ParametersError, "The Condition1 shall be specified");
        }
    }
    if (ok) {
        ok = data.Read("Output1", output1);
        if (!ok) {
            REPORT_ERROR(MARTe::ErrorManagement::ParametersError, "The Output1 shall be specified");
        }
    }
    if (ok) {
        ok = data.Read("Output2", output2);
        if (!ok) {
            REPORT_ERROR(MARTe::ErrorManagement::ParametersError, "The Output2 shall be specified");
        }
    }
    return ok;
}

bool JARTStateMachineGAM::PrepareNextState(const MARTe::char8 * const currentStateName, const MARTe::char8 * const nextStateName) {

    return true;
}

bool JARTStateMachineGAM::Setup() {
    using namespace MARTe;
    bool ok = (numberOfInputSignals == 4u);
    if (ok) {
        ok = (numberOfOutputSignals == 1u);
        if (!ok) {
            REPORT_ERROR(MARTe::ErrorManagement::ParametersError, "One output signal shall be defined");
        }
    }
    else {
        REPORT_ERROR(MARTe::ErrorManagement::ParametersError, "Four input signals shall be defined");
    }
    if (ok) {
        uint32 c;
        for (c = 0u; c < numberOfInputSignals; c++) {
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
        ok = (outputTime == UnsignedInteger32Bit);
        if (!ok) {
            StreamString signalName;
            (void) GetSignalName(InputSignals, 0, signalName);
            REPORT_ERROR(MARTe::ErrorManagement::ParametersError, "Signal %s shall be defined as uint32", signalName.Buffer());
        }
    }
    if (ok) {
        currentTime = reinterpret_cast<uint32 *>(GetInputSignalMemory(0));
        triggerSignal = reinterpret_cast<uint32 *>(GetInputSignalMemory(1));
        pulseDuration1Signal = reinterpret_cast<uint32 *>(GetInputSignalMemory(2));
        pulseDuration2Signal = reinterpret_cast<uint32 *>(GetInputSignalMemory(3));
        outputSignal = reinterpret_cast<uint32 *>(GetOutputSignalMemory(0));
    }
    return ok;
}

bool JARTStateMachineGAM::Execute() {
    using namespace MARTe;
    if (currentState == MyState1) {
        if ((*triggerSignal == conditionTrigger) && (lastPulseTrigger == 0)) {
            //TODO possible overflow of time ignored
            triggerUntilTime = (*currentTime + *pulseDuration1Signal);
            currentState = MyState2;
        }
    }
    else if (currentState == MyState2) {
        if (triggerUntilTime == 0) {
            //TODO possible overflow of time ignored
            triggerUntilTime = (*currentTime + *pulseDuration2Signal);
            currentState = MyState3;
        }
    }
    else if (currentState == MyState3) {
        if (triggerUntilTime == 0) {
            currentState = MyState1;
            *outputSignal = 0;
        }
    }


    bool trigger = (*triggerSignal != 0);
    if (trigger) {
        trigger = (*currentTime < triggerUntilTime);
    }
    if (trigger) {
        if (currentState == MyState2) {
            *outputSignal = output1;
        }
        else if (currentState == MyState3) {
            *outputSignal = output2;
        }
    }
    else {
        triggerUntilTime = 0;
    }
    lastPulseTrigger = *triggerSignal;

    return true;
}

CLASS_REGISTER(JARTStateMachineGAM, "1.0")
