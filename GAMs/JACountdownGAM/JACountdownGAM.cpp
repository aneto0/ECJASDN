/**
 * @file JACountdownGAM.cpp
 * @brief Source file for class JACountdownGAM
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
 * the class JACountdownGAM (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "JACountdownGAM.h"
#include "AdvancedErrorManagement.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

JACountdownGAM::JACountdownGAM() {
	countdownTime = NULL_PTR(MARTe::uint32 *);
	finished = NULL_PTR(MARTe::uint32 *);
	remainingTime = 0u;
	outputValue = 0u;
	step = 0u;
}

JACountdownGAM::~JACountdownGAM() {
}

bool JACountdownGAM::Initialise(MARTe::StructuredDataI & data) {
    using namespace MARTe;
    bool ok = GAM::Initialise(data);
    if (ok) {
        ok = data.Read("Step", step);
        if (!ok) {
            REPORT_ERROR(MARTe::ErrorManagement::ParametersError, "The Step shall be specified");
        }
    }
    if (ok) {
        ok = data.Read("OutputValue", outputValue);
        if (!ok) {
            REPORT_ERROR(MARTe::ErrorManagement::ParametersError, "The OutputValue shall be specified");
        }
    }
    return ok;
}

bool JACountdownGAM::PrepareNextState(const MARTe::char8 * const currentStateName, const MARTe::char8 * const nextStateName) {
    if (countdownTime != NULL_PTR(MARTe::uint32 *)) {
    	remainingTime = *countdownTime;
    }
    return true;
}

bool JACountdownGAM::Setup() {
    using namespace MARTe;
    bool ok = (numberOfInputSignals == 1u);
    if (ok) {
        ok = (numberOfOutputSignals == 1u);
        if (!ok) {
            REPORT_ERROR(MARTe::ErrorManagement::ParametersError, "One output signal shall be defined");
        }
    }
    else {
        REPORT_ERROR(MARTe::ErrorManagement::ParametersError, "One input signal with the countdown time shall be defined");
    }
    if (ok) {
        TypeDescriptor inputType = GetSignalType(InputSignals, 0);
        ok = (inputType == UnsignedInteger32Bit);
        if (!ok) {
            StreamString signalName;
            (void) GetSignalName(InputSignals, 0, signalName);
            REPORT_ERROR(MARTe::ErrorManagement::ParametersError, "Signal %s shall be defined as uint32", signalName.Buffer());
        }
    }
    if (ok) {
        TypeDescriptor timeType = GetSignalType(OutputSignals, 0);

        ok = (timeType == SignedInteger32Bit);
        if (!ok) {
            StreamString signalName;
            (void) GetSignalName(InputSignals, 0, signalName);
            REPORT_ERROR(MARTe::ErrorManagement::ParametersError, "Signal %s shall be defined as uint32", signalName.Buffer());
        }
    }
    if (ok) {
        countdownTime = reinterpret_cast<uint32 *>(GetInputSignalMemory(0));
        finished = reinterpret_cast<uint32 *>(GetOutputSignalMemory(0));
        remainingTime = *countdownTime;
    }
    return ok;
}

bool JACountdownGAM::Execute() {
    using namespace MARTe;
    if (remainingTime > step) {
    	remainingTime -= step;
    	//TODO remove this after testing
    	*finished = 0;
    } else {
    	remainingTime = 0;
    	*finished = outputValue;
    }
    return true;
}

CLASS_REGISTER(JACountdownGAM, "1.0")
