/**
 * @file JASequenceTimerGAM.cpp
 * @brief Source file for class JAMessageGAM
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
 * the class JAMessageGAM (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "JASequenceTimerGAM.h"
#include "MessageI.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

JASequenceTimerGAM::JASequenceTimerAM() {
    inputSignals = NULL_PTR(MARTe::uint32 **);
    operation = 0; //0 -> OR, 1 -> AND
    needsReset = false;
}

JASequenceTimerGAM::~JASequenceTimerAM() {
    if (inputSignals != NULL_PTR(MARTe::uint32 **)) {
        delete[] inputSignals;
    }
}

bool JASequenceTimerGAM::Initialise(MARTe::StructuredDataI & data) {
    using namespace MARTe;
    bool ok = GAM::Initialise(data);
    if (ok) {

    	/*Read time Delay*/

        MARTe::StreamString operationStr;
        if (data.Read("Operation", operationStr)) {
            if (operationStr == "AND") {
                operation = 1;
            }
            else if (operationStr == "OR") {
                operation = 0;
            }
            else {
                ok = false;
                REPORT_ERROR(ErrorManagement::ParametersError, "Operation %s is not defined", operationStr.Buffer());
            }
        }
    }
    if (ok) {
        ok = (Size() == 1);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "A Message object shall be added to this container");
        }
    }
    if (ok) {
        eventMsg = Get(0);
        ok = (eventMsg.IsValid());
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "A valid Message shall be added to this container");
        }
    }
    return ok;
}

bool JASequenceTimerGAM::Setup() {
    using namespace MARTe;
    bool ok = (numberOfInputSignals > 0u);
    if (ok) {
        inputSignals = new uint32*[numberOfInputSignals];
        uint32 i;
        for (i = 0u; i < numberOfInputSignals; i++) {
            inputSignals[i] = reinterpret_cast<uint32 *>(GetInputSignalMemory(i));
        }
    }
    else {
        REPORT_ERROR(MARTe::ErrorManagement::ParametersError, "At least one input signal shall be defined");
    }
    if (ok) {
        uint32 i;
        for (i = 0u; (i < numberOfInputSignals) && (ok); i++) {
            TypeDescriptor inputType = GetSignalType(InputSignals, 0u);
            ok = (inputType == UnsignedInteger32Bit);
            if (!ok) {
                StreamString signalName;
                (void) GetSignalName(InputSignals, i, signalName);
                REPORT_ERROR(MARTe::ErrorManagement::ParametersError, "Signal %s shall be defined as uint32", signalName.Buffer());
            }
        }
    }

    return ok;
}

bool JASequenceTimerGAM::PrepareNextState(const MARTe::char8 * const currentStateName, const MARTe::char8 * const nextStateName) {
    needsReset = false;
    return true;
}

bool JASequenceTimerGAM::Execute() {
    bool ok = true;
    bool eventDetected = false;
    if (operation == 0) {
        MARTe::uint32 j;
        for (j = 0; (j < numberOfInputSignals) && (!eventDetected); j++) {
            eventDetected = (*inputSignals[j] == 1u);
        }
    }
    else if (operation == 1) {
        MARTe::uint32 j;
        eventDetected = (*inputSignals[0] == 1u);
        for (j = 1; (j < numberOfInputSignals); j++) {
            eventDetected &= (*inputSignals[j] == 1u);
        }
    }
    if (eventDetected) {
        if (!needsReset) {
            ok = (MARTe::MessageI::SendMessage(eventMsg, this) == MARTe::ErrorManagement::NoError);
            needsReset = true;
        }
    }
    return ok;
}

CLASS_REGISTER(JASequenceTimerGAM, "1.0")
