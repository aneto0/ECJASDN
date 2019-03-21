/**
 * @file JAPulseDriverDemoGAM.h
 * @brief Header file for class JAPulseDriverDemoGAM
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

 * @details This header file contains the declaration of the class JAPulseDriverDemoGAM
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

class JAPulseDriverDemoGAM : public MARTe::GAM, public MARTe::StatefulI {
public:
    CLASS_REGISTER_DECLARATION()

    JAPulseDriverDemoGAM();

    virtual ~JAPulseDriverDemoGAM();

    virtual bool Initialise(MARTe::StructuredDataI & data);

    virtual bool Setup();

    virtual bool Execute();

    virtual bool PrepareNextState(const MARTe::char8 * const currentStateName,
                                  const MARTe::char8 * const nextStateName);

private:
    //Current time in us
    MARTe::uint32 *currentTime;

    //Pulse length in us
    MARTe::uint32 *pulseLengthTime;

    //Pulse trigger (1 trigger / 0 no trigger)
    MARTe::uint32 *pulseTrigger;

    //Output value to write.
    MARTe::uint32 *output;

    //Pulse on output value
    MARTe::uint8 onValue;

    //Pulse off output value
    MARTe::uint8 offValue;

    //Last trigger value
    MARTe::uint32 lastPulseTrigger;

    //Trigger until time
    MARTe::uint32 triggerUntilTime;
};



/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* GAMS_JACOUNTDOWNGAM_H_ */
