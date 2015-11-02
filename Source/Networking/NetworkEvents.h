/*
    Initial author: (https://github.com/)Convery
    License: LGPL 3.0
    Started: 2015-11-01
    Notes:
        Callbacks for networking events.
*/

#pragma once
#include "NetworkStructures.h"

// Event type to handle.
enum eNetworkEventType
{
    NEV_TCPRECV = 0,
    NEV_UDPRECV = 1,
    NEV_TCPSEND = 2,
    NEV_UDPSEND = 3,
    NEV_ERROR = 4,
    NEV_MAX
};

// Event callbacks should use this format. Returns if it handled the packet.
using StreamedCallback = bool(struct StreamedWrapper *Packet);
using DatagramCallback = bool(struct DatagramWrapper *Packet);

// Add or remove an event handler to the internal list.
void ListCallback(eNetworkEventType Type, StreamedCallback *Callback);
void ListCallback(eNetworkEventType Type, DatagramCallback *Callback);
void RemoveCallback(eNetworkEventType Type, StreamedCallback *Callback);
void RemoveCallback(eNetworkEventType Type, DatagramCallback *Callback);

// Call the calback from external code.
bool InvokeCallback(eNetworkEventType Type, struct StreamedWrapper *Packet);
bool InvokeCallback(eNetworkEventType Type, struct DatagramWrapper *Packet);
