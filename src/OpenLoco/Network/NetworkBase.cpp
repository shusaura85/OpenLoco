#include "NetworkBase.h"

using namespace OpenLoco::Network;

NetworkBase::NetworkBase()
{
    _socket = Socket::createUdp();
}

NetworkBase::~NetworkBase()
{
    close();
}

bool NetworkBase::isClosed() const
{
    return _isClosed;
};

void NetworkBase::recievePacketLoop()
{
    while (!_endRecievePacketLoop)
    {
        Packet packet;
        size_t packetSize{};

        std::unique_ptr<INetworkEndpoint> endpoint;
        auto result = _socket->ReceiveData(&packet, sizeof(Packet), &packetSize, &endpoint);
        if (result == NetworkReadPacket::Success)
        {
            // Validate packet
            if (packet.header.dataSize <= packetSize - sizeof(PacketHeader))
            {
                recievePacket(std::move(endpoint), packet);
            }
        }
        else
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
}

void NetworkBase::beginRecievePacketLoop()
{
    _endRecievePacketLoop = false;
    _recievePacketThread = std::thread([this] { recievePacketLoop(); });
}

void NetworkBase::endRecievePacketLoop()
{
    _endRecievePacketLoop = true;
    if (_recievePacketThread.joinable())
    {
        _recievePacketThread.join();
    }
    _recievePacketThread = {};
}

void NetworkBase::recievePacket(std::unique_ptr<INetworkEndpoint> endpoint, const Packet& packet)
{
    std::unique_lock<std::mutex> lk(_receivedPacketsSync);
    _receivedPackets.push({ std::move(endpoint),
                            packet });
}

void NetworkBase::processReceivePackets()
{
    std::unique_lock<std::mutex> lk(_receivedPacketsSync);
    while (!_receivedPackets.empty())
    {
        auto& p = _receivedPackets.front();
        onRecievePacket(std::move(p.endpoint), p.packet);
        _receivedPackets.pop();
    }
}

void NetworkBase::update()
{
    processReceivePackets();
    onUpdate();
}

void NetworkBase::onClose()
{
}

void NetworkBase::onUpdate()
{
}

void NetworkBase::onRecievePacket(std::unique_ptr<INetworkEndpoint> endpoint, const Packet& packet)
{
}

void NetworkBase::close()
{
    if (_isClosed)
        return;

    endRecievePacketLoop();

    onClose();

    _socket = nullptr;
    _isClosed = true;
}
