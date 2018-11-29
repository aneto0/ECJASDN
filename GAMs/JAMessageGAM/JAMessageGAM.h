/**
 * @file JAMessageGAM.h
 * @brief Header file for class JAMessageGAM
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

 * @details This header file contains the declaration of the class JAMessageGAM
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef GAMS_JAMESSAGEGAM_H_
#define GAMS_JAMESSAGEGAM_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "GAM.h"
#include "Message.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

class JAMessageGAM : public MARTe::GAM, public MARTe::StatefulI  {
public:
    CLASS_REGISTER_DECLARATION()

    JAMessageGAM();

    virtual ~JAMessageGAM();

    virtual bool Initialise(MARTe::StructuredDataI & data);

    virtual bool Setup();

    virtual bool Execute();

    virtual bool PrepareNextState(const MARTe::char8 * const currentStateName,
                                  const MARTe::char8 * const nextStateName);
private:
    MARTe::uint32 **inputSignals;

    enum OperationMode {
        And, Or, Value, Xor, Nor, None
    };

    OperationMode operation;

    MARTe::uint32 valueToCheck;

    MARTe::ReferenceT<MARTe::Message> eventMsg;

    bool needsReset;
};



/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* GAMS_JAMESSAGEGAM_H_ */
