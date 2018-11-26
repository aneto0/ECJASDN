#!/bin/bash
#Arguments -f FILENAME -m MESSAGE [-d cgdb|strace] -x DAN_CONFIG_LOCATION -r root
#-f FILENAME=MARTe configuration file
#-m MESSAGE=Start message
#-d cgdb=Run with cgdb
#-d strace=Run with strace
#-x DAN_CONFIG_LOCATION=Location of the DANConfig.xml (e.g. ~/Projects/ECJASDN/Configurations/DANTestConfig.xml)
#-r run as root

#Run with cgdb or strace?
DEBUG=""

#Consume input arguments
while [[ $# -gt 1 ]]
do
key="$1"

case $key in
    -f|--file)
    FILE="$2"
    shift # past argument
    ;;
    -m|--message)
    MESSAGE="$2"
    shift # past argument
    ;;
    -s|--state)
    STATE="$2"
    shift # past argument
    ;;
    -d|--debug)
    DEBUG="$2"
    shift # past argument
    ;;
    -x|--dan_config)
    DAN_CONFIG_LOCATION="$2"
    shift # past argument
    ;;   
    -i|--dan_ip)
    DAN_MASTER_IP="$2"
    shift # past argument
    ;;   
    -r|--root)
    RUN_AS_ROOT="root"
    shift # past argument
    ;;   
    --default)
    DEFAULT=YES
    ;;
    *)
    # unknown option
    ;;
esac
shift # past argument or value
done

if [ -z ${MARTe2_DIR+x} ]; then
    echo "Please set the MARTe2_DIR environment variable";
    exit;
fi

if [ -z ${MARTe2_Components_DIR+x} ]; then
    #Check if this is a CCS deployment
    MARTe2_Components_DIR_CSS=$MARTe2_DIR/Build/x86-linux/Components/
    if [ -d ${MARTe2_Components_DIR_CSS+x} ]; then
        MARTe2_Components_DIR=$MARTe2_DIR
    else
        echo "Please set the MARTe2_Components_DIR environment variable";
        exit;
    fi
fi

echo $MARTe2_Components_DIR

LD_LIBRARY_PATH=.
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_DIR/Build/x86-linux/Core/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/x86-linux/Components/DataSources/EPICSCA/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/x86-linux/Components/DataSources/LinuxTimer/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/x86-linux/Components/DataSources/LoggerDataSource/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/x86-linux/Components/DataSources/DAN/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/x86-linux/Components/DataSources/NI6259/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/x86-linux/Components/DataSources/NI6368/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/x86-linux/Components/DataSources/SDN/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/x86-linux/Components/DataSources/UDP/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/x86-linux/Components/DataSources/MDSWriter/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/x86-linux/Components/DataSources/RealTimeThreadAsyncBridge/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/x86-linux/Components/DataSources/RealTimeThreadSynchronisation/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/x86-linux/Components/DataSources/FileDataSource/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/x86-linux/Components/GAMs/IOGAM/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/x86-linux/Components/GAMs/BaseLib2GAM/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/x86-linux/Components/GAMs/ConversionGAM/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/x86-linux/Components/GAMs/FilterGAM/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/x86-linux/Components/GAMs/StatisticsGAM/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/x86-linux/Components/GAMs/WaveformGAM/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/x86-linux/Components/Interfaces/BaseLib2Wrapper/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/x86-linux/Components/Interfaces/SysLogger/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/x86-linux/Components/Interfaces/EPICS/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../Build/x86-linux/GAMs/ESDNValidationGAM/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../Build/x86-linux/GAMs/SCURUIFGAM/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../Build/x86-linux/Interfaces/SCURUIF/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../Build/x86-linux/Interfaces/NiDevice/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../Build/x86-linux/Interfaces/NiDevice/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../Build/x86-linux/Interfaces/NiSyncDevice/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../Build/x86-linux/DataSources/AsyncThreadGate/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../Build/x86-linux/DataSources/CircularBufferAcqDriver/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../Build/x86-linux/DataSources/CRioCircularFifoReader/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../Build/x86-linux/DataSources/CRioFifo/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../Build/x86-linux/DataSources/CRioMxi/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../Build/x86-linux/DataSources/DynamicWaveformGenerator/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../Build/x86-linux/DataSources/FixedWaveformGenerator/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../Build/x86-linux/DataSources/NiPci1588Tcn/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../Build/x86-linux/DataSources/RandomDataSource/

if [ ! -z "$CRIO_PROJ_DIR" ]; then
    LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../Build/x86-linux/ThirdPartyLibs/NI_9157/$(CRIO_PROJ_DIR)/NiLabviewGen/
fi
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../Build/x86-linux/GAMs/FilterDownsamplingGAM/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$EPICS_BASE/lib/$EPICS_HOST_ARCH
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/mdsplus/lib64/

echo $LD_LIBRARY_PATH
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH

#Starts the DAN services only if required
if [ ! -z "$DAN_CONFIG_LOCATION" ]; then
    export DAN_ARCHIVE_MASTER=$DAN_MASTER_IP
    echo $DAN_MASTER_IP
    /opt/codac/bin/danApiTool api close
    /opt/codac/bin/danApiTool api init $DAN_CONFIG_LOCATION
fi

if [ ! -z ${RUN_AS_ROOT+x} ]; then 
    #Disable CPU speed changing
    service cpuspeed stop

    #Allocate dynamic ticks to CPU #0
    for i in `pgrep rcu[^c]` ; do taskset -pc 0 $i ; done

    #Assign IRQ to correct CPU
    tuna -q eth0 -c 3 -x -m

    #Isolate cpus 1-3 (tasks and interrupts)
    tuna -c 1-3 --isolate

    if [ ! -z "$DAN_CONFIG_LOCATION" ]; then
        /etc/init.d/dan_archiver_writer -f start
    fi
fi

#Location of the MARTe2 application loader
MARTe2APP=$MARTe2_DIR/Build/x86-linux/App/MARTeApp.ex

#Start with cgdb or with strace
if [ "$DEBUG" = "cgdb" ]; then
    cgdb --args $MARTe2APP -l RealTimeLoader -f $FILE -m $MESSAGE
elif [ "$DEBUG" = "strace" ]; then
    strace -o/tmp/strace.err $MARTe2APP -l RealTimeLoader -f $FILE -m $MESSAGE
else
    if [ -z ${RUN_AS_ROOT+x} ]; then
        if [ -z ${STATE+x} ]; then
                $MARTe2APP -l RealTimeLoader -f $FILE -m $MESSAGE
        else
                $MARTe2APP -l RealTimeLoader -f $FILE -s $STATE 
        fi
    else
        if [ -z ${STATE+x} ]; then
             taskset 1 $MARTe2APP -l RealTimeLoader -f $FILE -m $MESSAGE
        else
             taskset 1 $MARTe2APP -l RealTimeLoader -f $FILE -s $STATE 
        fi
    fi
fi


if [ ! -z "$DAN_CONFIG_LOCATION" ]; then
    /opt/codac/bin/danApiTool api close
fi
