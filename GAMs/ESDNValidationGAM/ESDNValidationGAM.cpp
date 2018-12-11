/**
 * @file ESDNValidationGAM.cpp
 * @brief Source file for class ESDNValidationGAM
 * @date 01/03/2017
 * @author Andre Neto
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
 * the class ESDNValidationGAM (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "CompilerTypes.h"
#include "ESDNValidationGAM.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

ESDNValidationGAM::ESDNValidationGAM() :
        MARTe::GAM() {
    using namespace MARTe;
    maximumESDNFailures = 0u;
    minimumESDNSuccesses = 0u;
    lastWaveformPacketID = 0u;
    replyPacketMaximumDelay = 0u;
    waveformPacketESDNTime = 0u;
    commandPacketAbsoluteTime = NULL_PTR(MARTe::uint64 *);
    esdnTime = NULL_PTR(MARTe::uint32 *);
    waveformPacketID = NULL_PTR(MARTe::uint16 *);
    replyPacketAbsoluteTime = NULL_PTR(MARTe::uint64 *);
    replyPacketESDNTime = NULL_PTR(MARTe::uint32 *);
    replyPacketWaveformID = NULL_PTR(MARTe::uint32 *);
    replyPacketLatency = NULL_PTR(MARTe::uint32 *);
    replyPacketOnTimeCounter = NULL_PTR(MARTe::uint32 *);
    replyPacketLateCounter = NULL_PTR(MARTe::uint32 *);
    replyPacketWaveformLateCounter = NULL_PTR(MARTe::uint32 *);
    esdnSCUConnectionState = NULL_PTR(MARTe::uint8 *);
    logDebug = true;

}

ESDNValidationGAM::~ESDNValidationGAM() {
    using namespace MARTe;
}

bool ESDNValidationGAM::Setup() {
    using namespace MARTe;
    bool ok = (GetNumberOfInputSignals() == 6u);
    if (!ok) {
        REPORT_ERROR(ErrorManagement::ParametersError, "GetNumberOfInputSignals() != 6u");
    }
    if (ok) {
        ok = (GetSignalType(InputSignals, 0u) == UnsignedInteger64Bit);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "GetSignalType(InputSignals, 0u) != UnsignedInteger64Bit");
        }
    }
    if (ok) {
        ok = (GetSignalType(InputSignals, 1u) == UnsignedInteger32Bit);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "GetSignalType(InputSignals, 1u) != UnsignedInteger32Bit");
        }
    }
    if (ok) {
        ok = (GetSignalType(InputSignals, 2u) == UnsignedInteger16Bit);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "GetSignalType(InputSignals, 2u) != UnsignedInteger16Bit");
        }
    }
    if (ok) {
        ok = (GetSignalType(InputSignals, 3u) == UnsignedInteger64Bit);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "GetSignalType(InputSignals, 3u) != UnsignedInteger64Bit");
        }
    }
    if (ok) {
        ok = (GetSignalType(InputSignals, 4u) == UnsignedInteger32Bit);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "GetSignalType(InputSignals, 4u) != UnsignedInteger32Bit");
        }
    }
    if (ok) {
        ok = (GetSignalType(InputSignals, 5u) == UnsignedInteger16Bit);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "GetSignalType(InputSignals, 5u) != UnsignedInteger16Bit");
        }
    }
    if (ok) {
        ok = (GetNumberOfOutputSignals() == 5u);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "GetNumberOfOutputSignals() != 4u");
        }
    }
    if (ok) {
        ok = (GetSignalType(OutputSignals, 0u) == UnsignedInteger32Bit);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "GetSignalType(OutputSignals, 0u) != UnsignedInteger32Bit");
        }
    }
    if (ok) {
        ok = (GetSignalType(OutputSignals, 1u) == UnsignedInteger32Bit);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "GetSignalType(OutputSignals, 1u) != UnsignedInteger32Bit");
        }
    }
    if (ok) {
        ok = (GetSignalType(OutputSignals, 2u) == UnsignedInteger32Bit);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "GetSignalType(OutputSignals, 2u) != UnsignedInteger32Bit");
        }
    }
    if (ok) {
        ok = (GetSignalType(OutputSignals, 3u) == UnsignedInteger32Bit);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "GetSignalType(OutputSignals, 3u) != UnsignedInteger32Bit");
        }
    }
    if (ok) {
        ok = (GetSignalType(OutputSignals, 4u) == UnsignedInteger8Bit);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "GetSignalType(OutputSignals, 4u) != UnsignedInteger8Bit");
        }
    }
    if (ok) {
        uint32 i;
        uint32 readElements = 0u;
        for (i = 0u; (i < GetNumberOfInputSignals()) && (ok); i++) {
            ok = (GetSignalNumberOfSamples(InputSignals, i, readElements));
            if (ok) {
                ok = (readElements == 1u);
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "GetSignalNumberOfSamples(InputSignals, %d) != 1u", i);
                }
            }
            if (ok) {
                ok = (GetSignalNumberOfElements(InputSignals, i, readElements));
            }
            if (ok) {
                ok = (readElements <= 1u);
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "GetSignalNumberOfElements(InputSignals, %d) != 1u", i);
                }
            }
        }
    }
    if (ok) {
        uint32 i;
        uint32 readElements = 0u;
        for (i = 0u; (i < GetNumberOfOutputSignals()) && (ok); i++) {
            ok = (GetSignalNumberOfSamples(OutputSignals, i, readElements));
            if (ok) {
                ok = (readElements == 1u);
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "GetSignalNumberOfSamples(OutputSignals, %d) != 1u", i);
                }
            }
            if (ok) {
                ok = (GetSignalNumberOfElements(OutputSignals, i, readElements));
            }
            if (ok) {
                ok = (readElements <= 1u);
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "GetSignalNumberOfElements(OutputSignals, %d) != 1u", i);
                }
            }
        }
    }

    if (ok) {
        commandPacketAbsoluteTime = reinterpret_cast<MARTe::uint64 *>(GetInputSignalMemory(0u));
        esdnTime = reinterpret_cast<MARTe::uint32 *>(GetInputSignalMemory(1u));
        waveformPacketID = reinterpret_cast<MARTe::uint16 *>(GetInputSignalMemory(2u));
        replyPacketAbsoluteTime = reinterpret_cast<MARTe::uint64 *>(GetInputSignalMemory(3u));
        replyPacketESDNTime = reinterpret_cast<MARTe::uint32 *>(GetInputSignalMemory(4u));
        replyPacketWaveformID = reinterpret_cast<MARTe::uint32 *>(GetInputSignalMemory(5u));
        replyPacketLatency = reinterpret_cast<MARTe::uint32 *>(GetOutputSignalMemory(0u));
        replyPacketOnTimeCounter = reinterpret_cast<MARTe::uint32 *>(GetOutputSignalMemory(1u));
        replyPacketLateCounter = reinterpret_cast<MARTe::uint32 *>(GetOutputSignalMemory(2u));
        replyPacketWaveformLateCounter = reinterpret_cast<MARTe::uint32 *>(GetOutputSignalMemory(3u));
        esdnSCUConnectionState = reinterpret_cast<MARTe::uint8 *>(GetOutputSignalMemory(4u));
        *esdnSCUConnectionState = ESDN_CONN_STATUS_CONNECTED;
    }
    return ok;
}

bool ESDNValidationGAM::Initialise(MARTe::StructuredDataI & data) {
    using namespace MARTe;
    bool ok = GAM::Initialise(data);
    if (ok) {
        ok = data.Read("MaximumESDNFailures", maximumESDNFailures);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The MaximumESDNFailures shall be specified");
        }
    }
    if (ok) {
        ok = data.Read("MinimumESDNSuccesses", minimumESDNSuccesses);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The MinimumESDNSuccesses shall be specified");
        }
    }
    if (ok) {
        ok = data.Read("ReplyPacketMaximumDelay", replyPacketMaximumDelay);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The ReplyPacketMaximumDelay shall be specified");
        }
    }
    return ok;
}

bool ESDNValidationGAM::Execute() {
    using namespace MARTe;
    *replyPacketLatency = static_cast<MARTe::uint32>(*replyPacketAbsoluteTime - *commandPacketAbsoluteTime);
    *replyPacketLatency /= 1e3;
    bool lateReply = ((*esdnTime - *replyPacketESDNTime) > 1u);
    if (!lateReply) {
        lateReply = (*replyPacketLatency > replyPacketMaximumDelay);
    }
    if (!lateReply) {
        if (*waveformPacketID != lastWaveformPacketID) {
            //Packet was sent now?
            if (waveformPacketESDNTime == 0u) {
                waveformPacketESDNTime = *esdnTime;
            }
            //Try 2 times. The first will not fail for sure.
            lateReply = ((*esdnTime - waveformPacketESDNTime) > 100u);
            if (*waveformPacketID == *replyPacketWaveformID) {
                lastWaveformPacketID = *waveformPacketID;
                waveformPacketESDNTime = 0u;
            }
            if (lateReply) {
                (*replyPacketWaveformLateCounter)++;
            }
        }
    }
    if (lateReply) {
        (*replyPacketLateCounter)++;
        *replyPacketOnTimeCounter = 0u;
        if (*replyPacketLateCounter > maximumESDNFailures) {
            if (logDebug) {
                REPORT_ERROR(ErrorManagement::Warning, "(*esdnTime - *replyPacketESDNTime) > 1u (%d - %d > 1u)", *esdnTime,
                             *replyPacketESDNTime);
                REPORT_ERROR(ErrorManagement::Warning, "*replyPacketLatency > replyPacketMaximumDelay (%d > %d)", *replyPacketLatency,
                             replyPacketMaximumDelay);
                REPORT_ERROR(
                        ErrorManagement::Warning,
                        "(*esdnTime - waveformPacketESDNTime) > 1u (%d - %d > 1u) (*waveformPacketID = %d *replyPacketWaveformID = %d)",
                        *esdnTime, waveformPacketESDNTime, *waveformPacketID, *replyPacketWaveformID);
            }
            *esdnSCUConnectionState = ESDN_CONN_STATUS_DISCONNECTED;
        }
    }
    else {
        if (*esdnSCUConnectionState == ESDN_CONN_STATUS_DISCONNECTED) {
            (*replyPacketOnTimeCounter)++;
            if (*replyPacketOnTimeCounter > minimumESDNSuccesses) {
                *esdnSCUConnectionState = ESDN_CONN_STATUS_CONNECTED;
                *replyPacketLateCounter = 0u;
                *replyPacketWaveformLateCounter = 0u;
            }
        }
    }
    return true;
}

CLASS_REGISTER(ESDNValidationGAM, "1.0")

