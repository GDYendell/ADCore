#include <pv/clientFactory.h>
#include <pv/pvAccess.h>
#include <pv/ntndarray.h>

#define PVAOverrunCounterString     "OVERRUN_COUNTER"
#define PVAPvNameString             "PV_NAME"
#define PVAPvConnectionStatusString "PV_CONNECTION"

class pvaDriver;

typedef epics::pvAccess::Channel::shared_pointer ChannelPtr;
typedef epics::pvAccess::ChannelProvider::shared_pointer ChannelProviderPtr;
typedef std::tr1::shared_ptr<pvaDriver> pvaDriverPtr;

class epicsShareClass pvaDriver : public ADDriver,
        public virtual epics::pvAccess::ChannelRequester,
        public virtual epics::pvData::MonitorRequester
{

public:
    pvaDriver (const char *portName, const char *pvName, int maxBuffers,
            size_t maxMemory, int priority, int stackSize);

    // Overriden from ADDriver:
    virtual void report (FILE *fp, int details);

protected:
    int PVAOverrunCounter;
    int PVAPvName;
    int PVAPvConnectionStatus;

private:
    std::string m_pvName;
    std::string m_request;
    short m_priority;
    ChannelProviderPtr m_provider;
    ChannelPtr m_channel;
    epics::pvData::PVStructurePtr m_pvRequest;
    epics::pvData::MonitorPtr m_monitor;
    pvaDriverPtr m_thisPtr;

    // Implemented for pvData::Requester
    std::string getRequesterName (void);
    void message (std::string const & message,
            epics::pvData::MessageType messageType);

    // Implemented for pvAccess::ChannelRequester
    void channelCreated (const epics::pvData::Status& status,
            ChannelPtr const & channel);
    void channelStateChange (ChannelPtr const & channel,
            epics::pvAccess::Channel::ConnectionState state);

    // Implemented for pvData::MonitorRequester
    void monitorConnect (epics::pvData::Status const & status,
            epics::pvData::MonitorPtr const & monitor,
            epics::pvData::StructureConstPtr const & structure);
    void monitorEvent (epics::pvData::MonitorPtr const & monitor);
    void unlisten (epics::pvData::MonitorPtr const & monitor);
};
