/*
	Initial author: (https://github.com/)Convery
	License: LGPL 3.0
	Started: 2015-10-29
	Notes:
		Structures used in the client<->clientAPI-server communication.
		UDP support is disabled for initial release.
*/

#pragma once
#include <stdint.h>

// When sending a streamed packet, it should be wrapped.
struct StreamedWrapper
{
	uint32_t StreamBeginToken;
	union
	{
		struct StreamedClientHeader *Client;
		struct StreamedServerHeader *Server;
	} Header;
	struct StreamedBody *Body;
	uint32_t StreamEndToken;
};
using TCPPacket = StreamedWrapper;
using TCPPacketWrapper = StreamedWrapper;

// The body of the packet may be encrypted and compressed.
struct StreamedBody
{
	uint32_t DecryptionToken;
	uint32_t PayloadChecksum;
	uint8_t	*PayloadData;
};
using TCPPacketContent = StreamedBody;
using TCPRequestContent = StreamedBody;
using TCPResponseContent = StreamedBody;

// Generic header format for streamed data.
struct StreamedHeader
{
	uint16_t PayloadLength;
	uint8_t	 EncryptedPayload;
	uint8_t	 CompressedPayload;
	uint32_t TransactionID;
};

// Request (client) and response (server) headers are different.
struct StreamedClientHeader : public StreamedHeader
{
	uint8_t	ServiceID;
	uint8_t TaskID;
};
struct StreamedServerHeader : public StreamedHeader
{
	uint16_t TaskResult;
};
using TCPClientHeader = StreamedClientHeader;
using TCPRequestHeader = StreamedClientHeader;
using TCPServerHeader = StreamedServerHeader;
using TCPResponseHeader = StreamedServerHeader;
