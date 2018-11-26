/**
 * @file GAMJAExample1.cpp
 * @brief Source file for class GAMJAExample1
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
 * the class GAMJAExample1 (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "GAMJAExample1.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

GAMJAExample1::GAMJAExample1() {

}

GAMJAExample1::~GAMJAExample1() {

}

bool GAMJAExample1::Initialise(MARTe::StructuredDataI & data) {
    bool ok = GAM::Initialise(data);
    return ok;
}

bool GAMJAExample1::Setup() {
    using namespace MARTe;
    bool ok = (GetNumberOfInputSignals() == 1u);
    if (ok) {
        ok = (GetNumberOfOutputSignals() == 1u);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "One and only one output signal");
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::ParametersError, "One and only one input signal");
    }
    if (ok) {
        TypeDescriptor inputType = GetSignalType(InputSignals, 0u);
        ok = (inputType == UnsignedInteger32Bit);
        //bla bla bla
    }
    if (ok) {
        inputSignal = reinterpret_cast<uint32 *>(GetInputSignalMemory(0));
        outputSignal = reinterpret_cast<int32 *>(GetOutputSignalMemory(0));
    }

    return ok;
}

bool GAMJAExample1::Execute() {
    bool ok = true;
    *outputSignal = -*inputSignal;
    return ok;
}

CLASS_REGISTER(GAMJAExample1, "1.0")
