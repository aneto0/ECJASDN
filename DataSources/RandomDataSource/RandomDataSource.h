/**
 * @file RandomDataSource.h
 * @brief Header file for class RandomDataSource
 * @date 07/06/2018
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

 * @details This header file contains the declaration of the class RandomDataSource
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef RANDOM_DATASOURCE_H_
#define RANDOM_DATASOURCE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "DataSourceI.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
/**
 * @brief DataDource which generates random numbers against a given seed.
 *
 * The configuration syntax is (names and signal quantities are only given as an example):
 * +RandomDataSource = {
 *     Class = RandomDataSource
 *     Seed = 8 //Seed against which the the seed will be generated. Note that each signal
 *     Signals = {
 *         Random1 = { //Maximum one signal
 *             Type = uint64 //All the integer types are supported
 *         }
 *     }
 * }
 */
class RandomDataSource: public MARTe::DataSourceI {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor. NOOP.
     */
RandomDataSource    ();

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~RandomDataSource();

    /**
     * @brief The configuration data detailed in the class description
     * @return true if all the compulsory parameters are set.
     */
    virtual bool Initialise(MARTe::StructuredDataI & data);

    /**
     * @brief Verifies that at most one signal has been set with the correct type (i.e. any integer).
     * @return true if the above conditions are met.
     */
    virtual bool SetConfiguredDatabase(MARTe::StructuredDataI & data);

    /**
     * @brief @see DataSourceI::Synchronise
     */
    virtual bool Synchronise();

    /**
     * @brief @see DataSourceI::AllocateMemory
     */
    virtual bool AllocateMemory();

    /**
     * @brief @see DataSourceI::GetSignalMemoryBuffer
     */
    virtual bool GetSignalMemoryBuffer(const MARTe::uint32 signalIdx, const MARTe::uint32 bufferIdx, void *&signalAddress);

    /**
     * @return "MemoryMapSynchronisedInputBroker"
     */
    virtual const MARTe::char8 *GetBrokerName(MARTe::StructuredDataI &data, const MARTe::SignalDirection direction);

    /**
     * @brief NOOP
     */
    virtual bool PrepareNextState(const MARTe::char8 * const currentStateName, const MARTe::char8 * const nextStateName);

private:
    /**
     * The seed to compute the signals.
     */
    MARTe::uint32 seed;

    /**
     * The signal pointer
     */
    MARTe::char8 *signalPtr;

    /**
     * The signal type descriptor.
     */
    MARTe::TypeDescriptor signalTypeDescriptor;

    /**
     * Compute the random value
     */
    template<typename T>
    void GetValue();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/
template<typename T>
void RandomDataSource::GetValue() {
    *(reinterpret_cast<T *>(&signalPtr[0u])) = static_cast<T>(rand_r(&seed));
}

#endif /* RANDOM_DATASOURCE_H_ */

