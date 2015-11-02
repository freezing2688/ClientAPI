/*
    Initial author: (https://github.com/)Convery
    License: LGPL 3.0
    Started: 2015-11-01
    Notes:
        Callbacks for networking events.
*/

#include "..\Defines.h"
#include "NetworkEvents.h"
#include <vector>
#include <mutex>
#include <assert.h>

// Callback vectors, use eNetworkEventType to access each.
std::vector<void *> Callbacks[eNetworkEventType::NEV_MAX];
std::mutex CallbackGuard;

// Call the calback from external code.
bool InvokeCallback(eNetworkEventType Type, struct StreamedWrapper *Packet)
{
    // Sanity check, we could call the other function but the author should be notified.
    assert(Type == eNetworkEventType::NEV_UDPRECV || Type == eNetworkEventType::NEV_UDPSEND);

    // Iterate until the packet gets handled, reverse order as the first callback should be global.
    CallbackGuard.lock();
    for (auto Iterator = Callbacks[Type].rbegin(); Iterator != Callbacks[Type].rend(); ++Iterator)
    {
        if (((StreamedCallback *)(*Iterator))(Packet))
        {
            CallbackGuard.unlock();
            return true;
        }            
    }

    CallbackGuard.unlock();
    return false;
}
bool InvokeCallback(eNetworkEventType Type, struct DatagramWrapper *Packet)
{
    // Sanity check, we could call the other function but the author should be notified.
    assert(Type == eNetworkEventType::NEV_TCPRECV || Type == eNetworkEventType::NEV_TCPSEND);

    // Iterate until the packet gets handled, reverse order as the first callback should be global.
    CallbackGuard.lock();
    for (auto Iterator = Callbacks[Type].rbegin(); Iterator != Callbacks[Type].rend(); ++Iterator)
    {
        if (((DatagramCallback *)(*Iterator))(Packet))
        {
            CallbackGuard.unlock();
            return true;
        }
    }

    CallbackGuard.unlock();
    return false;
}

// Add or remove an event handler to the internal list.
void ListCallback(eNetworkEventType Type, StreamedCallback *Callback)
{
    // Sanity check, we could call the other function but the author should be notified.
    assert(Type == eNetworkEventType::NEV_UDPRECV || Type == eNetworkEventType::NEV_UDPSEND);

    // Append the new callback.
    CallbackGuard.lock();
    Callbacks[Type].push_back(Callback);
    CallbackGuard.unlock();
}
void ListCallback(eNetworkEventType Type, DatagramCallback *Callback)
{
    // Sanity check, we could call the other function but the author should be notified.
    assert(Type == eNetworkEventType::NEV_TCPRECV || Type == eNetworkEventType::NEV_TCPSEND);

    // Append the new callback.
    CallbackGuard.lock();
    Callbacks[Type].push_back(Callback);
    CallbackGuard.unlock();
}
void RemoveCallback(eNetworkEventType Type, StreamedCallback *Callback)
{
    // Sanity check, we could call the other function but the author should be notified.
    assert(Type == eNetworkEventType::NEV_UDPRECV || Type == eNetworkEventType::NEV_UDPSEND);

    // Append the new callback.
    CallbackGuard.lock();
    for (auto Iterator = Callbacks[Type].begin(); Iterator != Callbacks[Type].end(); ++Iterator)
    {
        if (*Iterator == Callback)
        {
            Callbacks[Type].erase(Iterator);
            break;
        }
    }
    CallbackGuard.unlock();
}
void RemoveCallback(eNetworkEventType Type, DatagramCallback *Callback)
{
    // Sanity check, we could call the other function but the author should be notified.
    assert(Type == eNetworkEventType::NEV_TCPRECV || Type == eNetworkEventType::NEV_TCPSEND);

    // Append the new callback.
    CallbackGuard.lock();
    for (auto Iterator = Callbacks[Type].begin(); Iterator != Callbacks[Type].end(); ++Iterator)
    {
        if (*Iterator == Callback)
        {
            Callbacks[Type].erase(Iterator);
            break;
        }
    }
    CallbackGuard.unlock();
}
