/**
 * @file JAPreProgrammedGAM.h
 * @brief Header file for class JAPreProgrammedGAM
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

 * @details This header file contains the declaration of the class JAPreProgrammedGAM
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef GAMS_JAPREPROGRAMMEDGAM_H_
#define GAMS_JAPREPROGRAMMEDGAM_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "GAM.h"
#include "Message.h"
#include "MessageI.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

class JAPreProgrammedGAM : public MARTe::GAM, public MARTe::MessageI {
public:
    CLASS_REGISTER_DECLARATION()

    JAPreProgrammedGAM();

    virtual ~JAPreProgrammedGAM();

    virtual bool Initialise(MARTe::StructuredDataI & data);

    virtual bool Setup();

    virtual bool Execute();

    MARTe::ErrorManagement::ErrorType LoadFile();

    MARTe::ErrorManagement::ErrorType SetMode(MARTe::StreamString modeName);

private:
    MARTe::uint32 filenameSignalIndex;

    MARTe::int32 *timeSignal;

    MARTe::float32 **valueSignals;

    MARTe::int32 *preProgrammedTime;

    MARTe::uint32 *currentTime;

    MARTe::uint32 startTime;

    MARTe::float32 **preProgrammedValues;

    //Number of columns in csv, EXCLUDING the time
    MARTe::uint32 numberOfPreProgrammedValues;

    MARTe::uint32 numberOfPreProgrammedTimeRows;

    MARTe::StreamString directory;

    MARTe::uint32 currentRow;

    MARTe::FastPollingMutexSem fastMux;

    bool preparing;

    MARTe::StreamString currentState;

    enum OperationMode {
        Heating, PreProgrammed, None
    };

    //If modeName == Heating => mode = 1; modeName == PreProgrammed => mode = 2
    OperationMode mode;
};



/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* GAMS_JAPREPROGRAMMEDGAM_H_ */
