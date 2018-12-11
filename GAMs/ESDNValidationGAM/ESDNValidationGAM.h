/**
 * @file ESDNValidationGAM.h
 * @brief Header file for class ESDNValidationGAM
 * @date 04/06/2018
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

 * @details This header file contains the declaration of the class ESDNValidationGAM
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef ESDNVALIDATIONGAM_H_
#define ESDNVALIDATIONGAM_H_

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
/**
 * ESDN Connection status
 */
const MARTe::uint8 ESDN_CONN_STATUS_DISCONNECTED = 0u;
const MARTe::uint8 ESDN_CONN_STATUS_CONNECTED = 1u;
/**
 * @brief Validates the ESDN connection between a given SCU and the ECPC.
 * @details Implements the validation mechanism described in the EC FAT-tools specification.
 *
 * The configuration syntax is (names and signal quantities are only given as an example):
 * +ESDNValidationGAM = {
 *     Class = ESDNValidationGAM
 *     MaximumESDNFailures = 3 //Maximum number of consecutive failures that can occur before the ESDN connection to the SCU is considered to have failed.
 *     MinimumESDNSuccesses = 3 //Minimum number of consecutive successes that have to occur before the ESDN connection to the SCU is considered to be valid.
 *     ReplyPacketMaximumDelay = 1000 //Maximum delay in microseconds for the reply to arrive.
 *     InputSignals = {//Order is important!
 *         CommandPacketAbsoluteTime = { //Absolute time at which the command packet was sent.
 *             DataSource = DDB1
 *             Type = uint64
 *         }
 *         ESDNTime = { //ESDN time (1m resolution).
 *             DataSource = DDB1
 *             Type = uint32
 *         }
 *         WaveformPacketID = { //Last waveform packet identifier that was sent.
 *             DataSource = DDB1
 *             Type = uint16
 *         }
 *         ReplyPacketAbsoluteTime = { //Absolute time at which the reply packet was received.
 *             DataSource = DDB1
 *             Type = uint64
 *         }
 *         ReplyPacketESDNTime = { //ESDN time at which the reply packet was received.
 *             DataSource = DDB1
 *             Type = uint32
 *         }
 *         ReplyPacketWaveformID = { //Waveform packet identifier of the last reply received.
 *             DataSource = DDB1
 *             Type = uint16
 *         }
 *     }
 *     OutputSignals = {//Order is important!
 *         ReplyPacketLatency = { //Reply packet latency.
 *             DataSource = DDB1
 *             Type = uint32
 *         }
 *         ReplyPacketOnTimeCounter = { //Current number of on time counts.
 *             DataSource = DDB1
 *             Type = uint32
 *         }
 *         ReplyPacketLateCounter = { //Current number of on late counts.
 *             DataSource = DDB1
 *             Type = uint32
 *         }
 *         ReplyPacketWaveformLateCounter = { //Current number of on late counts w.r.t. to the waveform packet.
 *             DataSource = DDB1
 *             Type = uint32
 *         }
 *         ESDNSCUConnectionState = { //Current ESDN connection state.
 *             DataSource = DDB1
 *             Type = uint8
 *         }
 *     }
 * }
 */
class ESDNValidationGAM: public MARTe::GAM {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor. NOOP.
     */
    ESDNValidationGAM ();

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~ESDNValidationGAM();

    /**
     * @brief Verifies that:
     *  - GetNumberOfInputSignals() == GetNumberOfOutputSignals() &&
     *  - GetSignalType(InputSignals, *) == SignedInteger16Bit &&
     *  - GetSignalType(OutputSignals, *) == Float32Bit
     *  @return true if the conditions above are met.
     */
    virtual bool Setup();

    /**
     * @brief The configuration data detailed in the class description
     * @return true if all the compulsory parameters are set.
     */
    virtual bool Initialise(MARTe::StructuredDataI & data);

    /**
     * @brief Applies the filter to all the input signals.
     * @return true.
     */
    virtual bool Execute();

private:
    /**
     * The maximum number of consecutive failures that can occur before the ESDN connection to the SCU is considered to have failed.
     */
    MARTe::uint32 maximumESDNFailures;

    /**
     * The minimum number of consecutive successes that have to occur before the ESDN connection to the SCU is considered to be valid.
     */
    MARTe::uint32 minimumESDNSuccesses;

    /**
     * The maximum delay for the reply packet to arrive.
     */
    MARTe::uint32 replyPacketMaximumDelay;

    /**
     * Time at which the last waveform packet was sent.
     */
    MARTe::uint32 waveformPacketESDNTime;

    /**
     * Absolute time at which the command packet was sent.
     */
    MARTe::uint64 *commandPacketAbsoluteTime;

    /**
     * The ESDN time (1m resolution).
     */
    MARTe::uint32 *esdnTime;

    /**
     * The Waveform packet identifier.
     */
    MARTe::uint16 *waveformPacketID;

    /**
     * Absolute time at which the reply packet was received.
     */
    MARTe::uint64 *replyPacketAbsoluteTime;

    /**
     * ESDN time at which the reply packet was received.
     */
    MARTe::uint32 *replyPacketESDNTime;

    /**
     * The Waveform packet identifier of the last reply received.
     */
    MARTe::uint32 *replyPacketWaveformID;

    /**
     * The reply packet latency.
     */
    MARTe::uint32 *replyPacketLatency;

    /**
     * The current number of on time counts.
     */
    MARTe::uint32 *replyPacketOnTimeCounter;

    /**
     * The current number of on late counts.
     */
    MARTe::uint32 *replyPacketLateCounter;

    /**
     * The current number of on late counts for the waveform packet.
     */
    MARTe::uint32 *replyPacketWaveformLateCounter;

    /**
     * The current ESDN connection state.
     */
    MARTe::uint8 *esdnSCUConnectionState;

    /**
     * The last sent waveformPacketID
     */
    MARTe::uint32 lastWaveformPacketID;

    /**
     * Print debug messages?
     */
    bool logDebug;

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* ESDNVALIDATIONGAM_H_ */

