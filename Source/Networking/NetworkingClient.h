/*
    Initial author: (https://github.com/)Convery
    License: LGPL 3.0
    Started: 2015-11-02
    Notes:
        Simple, single connection, TCP/UDP client.
*/

#pragma once
#include <stdint.h>

class SimpleClientConnection
{
    size_t Socket;
    bool Streamed;

public:
    // Connect to an IP or hostname.
    bool Connect_IP(const char *IP, uint16_t Port, bool StreamedConnection);
    bool Connect_Host(const char *Hostname, uint16_t Port, bool StreamedConnection);
    void Disconnect();

    // Send and recv blocking.
    int32_t Send(uint32_t Length, char *Buffer);
    int32_t Recv(uint32_t Length, char *Buffer);

    // Loop recv calling NetworkEvents callbacks for each packet.
    void StartLoopingRecv();
};
