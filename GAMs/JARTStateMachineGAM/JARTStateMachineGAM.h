/**
 * @file JARTStateMachineGAM.h
 * @brief Header file for class JARTStateMachineGAM
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

 * @details This header file contains the declaration of the class JARTStateMachineGAM
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef GAMS_JACOUNTDOWNGAM_H_
#define GAMS_JACOUNTDOWNGAM_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "GAM.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

class JARTStateMachineGAM : public MARTe::GAM, public MARTe::StatefulI {
public:
    CLASS_REGISTER_DECLARATION()

    JARTStateMachineGAM();

    virtual ~JARTStateMachineGAM();

    virtual bool Initialise(MARTe::StructuredDataI & data);

    virtual bool Setup();

    virtual bool Execute();

    virtual bool PrepareNextState(const MARTe::char8 * const currentStateName,
                                  const MARTe::char8 * const nextStateName);

private:
    //The list of possible states
    enum JARealTimeState {
        MyState1 = 0,
        MyState2 = 1,
        MyState3 = 2
    };

    //The current rtState
    JARealTimeState currentState;

    //A given condition
    MARTe::uint32 conditionTrigger;

    //What to output in a given state and condition
    MARTe::uint32 output1;

    //What to output in another state and condition
    MARTe::uint32 output2;

    //The trigger signal
    MARTe::uint32 *triggerSignal;

    //Time signal
    MARTe::uint32 *currentTime;

    // Input signals.
    MARTe::uint32 *pulseDuration1Signal;
    MARTe::uint32 *pulseDuration2Signal;

    // Output signal to which the output value will be written.
    MARTe::uint32 *outputSignal;

    //Last trigger value
    MARTe::uint32 lastPulseTrigger;

    //Trigger until time
    MARTe::uint32 triggerUntilTime;
};



/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* GAMS_JACOUNTDOWNGAM_H_ */
