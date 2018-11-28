/**
 * @file JAPreProgrammedGAM.cpp
 * @brief Source file for class JAPreProgrammedGAM
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
 * the class JAPreProgrammedGAM (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "CLASSMETHODREGISTER.h"
#include "File.h"
#include "JAPreProgrammedGAM.h"
#include "MessageI.h"
#include "RegisteredMethodsMessageFilter.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

JAPreProgrammedGAM::JAPreProgrammedGAM() :
        GAM(), MessageI() {
    using namespace MARTe;
    filename = NULL_PTR(const MARTe::char8 *);
    timeSignal = NULL_PTR(MARTe::int32 *);
    valueSignals = NULL_PTR(MARTe::float32 **);
    preProgrammedValues = NULL_PTR(MARTe::float32 **);
    preProgrammedTime = NULL_PTR(MARTe::int32 *);

    numberOfPreProgrammedValues = 0u;
    numberOfPreProgrammedTimeRows = 0u;
    currentRow = 0u;
    mode = None;

    ReferenceT<RegisteredMethodsMessageFilter> filter(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    filter->SetDestination(this);
    (void) MessageI::InstallMessageFilter(filter);
    preparing = false;
    fastMux.Create();

}

JAPreProgrammedGAM::~JAPreProgrammedGAM() {
    using namespace MARTe;
    if (preProgrammedValues != NULL_PTR(MARTe::float32 **)) {
        uint32 i;
        for (i = 0u; i < numberOfPreProgrammedValues; i++) {
            delete preProgrammedValues[i];
        }
        delete[] preProgrammedValues;
    }
}

bool JAPreProgrammedGAM::Initialise(MARTe::StructuredDataI & data) {
    using namespace MARTe;
    bool ok = GAM::Initialise(data);
    if (ok) {
        ok = data.Read("OpenFileState", openFileState);
        if (!ok) {
            REPORT_ERROR(MARTe::ErrorManagement::ParametersError, "The OpenFileState shall be specified");
        }
    }
    if (ok) {
        ok = data.Read("Directory", directory);
        if (!ok) {
            REPORT_ERROR(MARTe::ErrorManagement::ParametersError, "The Directory shall be specified");
        }
    }
    return ok;
}

bool JAPreProgrammedGAM::Setup() {
    using namespace MARTe;
    bool ok = (numberOfInputSignals == 1u);
    if (ok) {
        ok = (numberOfOutputSignals > 0u);
        if (!ok) {
            REPORT_ERROR(MARTe::ErrorManagement::ParametersError, "At least one output signal shall be defined");
        }
    }
    else {
        REPORT_ERROR(MARTe::ErrorManagement::ParametersError, "One input signal with the filename shall be defined");
    }
    if (ok) {
        TypeDescriptor inputType = GetSignalType(InputSignals, 0);
        ok = (inputType == UnsignedInteger8Bit);
        if (!ok) {
            StreamString signalName;
            (void) GetSignalName(InputSignals, 0, signalName);
            REPORT_ERROR(MARTe::ErrorManagement::ParametersError, "Signal %s shall be defined as uint8", signalName.Buffer());
        }
    }
    if (ok) {
        uint32 i;
        TypeDescriptor timeType = GetSignalType(OutputSignals, 0);

        ok = (timeType == SignedInteger32Bit);
        if (!ok) {
            StreamString signalName;
            (void) GetSignalName(InputSignals, 0, signalName);
            REPORT_ERROR(MARTe::ErrorManagement::ParametersError, "Signal %s shall be defined as int32", signalName.Buffer());
        }

        for (i = 1u; (i < numberOfOutputSignals) && (ok); i++) {
            TypeDescriptor outputType = GetSignalType(OutputSignals, i);
            ok = (outputType == Float32Bit);
            if (!ok) {
                StreamString signalName;
                (void) GetSignalName(OutputSignals, i, signalName);
                REPORT_ERROR(MARTe::ErrorManagement::ParametersError, "Signal %s shall be defined as float32", signalName.Buffer());
            }
        }
    }
    if (ok) {
        valueSignals = new float32*[numberOfOutputSignals - 1];
        uint32 i;
        for (i = 1u; i < numberOfOutputSignals; i++) {
            valueSignals[i - 1] = reinterpret_cast<float32 *>(GetOutputSignalMemory(i));
        }
        timeSignal = reinterpret_cast<int32 *>(GetOutputSignalMemory(0));
    }
    return ok;
}

MARTe::ErrorManagement::ErrorType JAPreProgrammedGAM::LoadFile() {
    using namespace MARTe;

    bool ok = true;
    if (fastMux.FastTryLock()) {
        preparing = true;
        fastMux.FastUnLock();
    }

    const char8 * const filenameSignal = reinterpret_cast<const char8 * const >(GetInputSignalMemory(0));
    StreamString filename = directory;
    filename += DIRECTORY_SEPARATOR;
    filename += filenameSignal;
    REPORT_ERROR(MARTe::ErrorManagement::Debug, "Opening file %s", filename.Buffer());

    File f;
    ok = f.Open(filename.Buffer(), BasicFile::ACCESS_MODE_R);
    if (ok) {
        numberOfPreProgrammedTimeRows = 0u;
        StreamString tokenLine;
        while (f.GetLine(tokenLine)) {
            if (tokenLine[0] != '#') {
                numberOfPreProgrammedTimeRows++;
            }
            tokenLine = "";
        }
        numberOfPreProgrammedValues = 0u;
        f.Seek(0);
        uint32 t = 0u;
        tokenLine = "";
        while ((ok) && (f.GetLine(tokenLine))) {
            if (tokenLine[0] == '#') {
                tokenLine = "";
                continue;
            }
            if (numberOfPreProgrammedValues == 0) {
                StreamString token;
                char8 ignored;
                tokenLine.Seek(0);
                while (tokenLine.GetToken(token, ",", ignored)) {
                    numberOfPreProgrammedValues++;
                    token = "";
                }
                ok = (numberOfPreProgrammedValues == numberOfOutputSignals);
                numberOfPreProgrammedValues -= 1u;
                if (ok) {
                    preProgrammedTime = new int32[numberOfPreProgrammedTimeRows];
                    preProgrammedValues = new float32*[numberOfPreProgrammedTimeRows];
                    uint32 j;
                    for (j = 0u; j < numberOfPreProgrammedTimeRows; j++) {
                        preProgrammedValues[j] = new float32[numberOfPreProgrammedValues];
                    }
                }
                else {
                    REPORT_ERROR(MARTe::ErrorManagement::ParametersError,
                                 "Number of columns in csv file (%d) is not consistent with the number of output signals (%d)", numberOfPreProgrammedValues,
                                 numberOfOutputSignals);
                }
            }
            if (ok) {
                StreamString token;
                char8 ignored;
                tokenLine.Seek(0);
                uint32 idx = 0u;
                while (tokenLine.GetToken(token, ",", ignored)) {
                    if (idx == 0u) {
                        preProgrammedTime[t] = atoi(token.Buffer());
                    }
                    else {
                        ok = ((idx - 1) < numberOfPreProgrammedValues);
                        if (ok) {
                            preProgrammedValues[t][idx - 1] = static_cast<float32>(atof(token.Buffer()));
                        }
                        else {
                            REPORT_ERROR(MARTe::ErrorManagement::FatalError,
                                         "Number of columns in csv file is not consistent with the number of output signals in line %d", t);
                        }
                    }
                    token = "";
                    idx++;
                }
                t++;
            }
            tokenLine = "";
        }
    }
    if (fastMux.FastTryLock()) {
        preparing = false;
        fastMux.FastUnLock();
    }

    if (ok) {
        currentRow = 0u;
        mode = None;
    }
    else {
        numberOfPreProgrammedTimeRows = 0u;
    }
    //TODO RESET currentRow against a given state name?
    return ok;
}

MARTe::ErrorManagement::ErrorType JAPreProgrammedGAM::SetMode(MARTe::StreamString modeName) {
    using namespace MARTe;
    ErrorManagement::ErrorType err;
    if (modeName == "Heating") {
        mode = Heating;
    }
    else if (modeName == "PreProgrammed") {
        mode = PreProgrammed;
    }
    else {
        err.fatalError = true;
        REPORT_ERROR(MARTe::ErrorManagement::FatalError, "Unsupported mode %s", modeName);
    }
    return err;
}

bool JAPreProgrammedGAM::Execute() {
    using namespace MARTe;
    bool isLoadingFile = false;
    if (fastMux.FastTryLock()) {
        isLoadingFile = preparing;
        fastMux.FastUnLock();
    }

    bool ok = true;
    if (mode != None) {
        if (!isLoadingFile) {
            if ((currentRow < numberOfPreProgrammedTimeRows)) {
                int32 currentTime = preProgrammedTime[currentRow];
                bool writeToOutput = ((mode == Heating) && (currentTime <= 0));
                if (!writeToOutput) {
                    writeToOutput = ((mode == PreProgrammed) && (currentTime > 0));
                }

                if (writeToOutput) {
                    *timeSignal = currentTime;
                    uint32 j;
                    for (j = 0u; j < (numberOfOutputSignals - 1); j++) {
                        *valueSignals[j] = preProgrammedValues[currentRow][j];
                    }
                    currentRow++;
                }
            }
        }
    }
    return ok;
}
CLASS_METHOD_REGISTER(JAPreProgrammedGAM, LoadFile)
CLASS_METHOD_REGISTER(JAPreProgrammedGAM, SetMode)
CLASS_REGISTER(JAPreProgrammedGAM, "1.0")
