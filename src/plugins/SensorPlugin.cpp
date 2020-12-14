#include "configuration.h"
#include "SensorPlugin.h"
#include "MeshService.h"
#include "main.h"

#include "concurrency/OSThread.h"

#include <assert.h>

// Create an a static instance of our plugin - this registers with the plugin system
SensorPlugin sensorPlugin;



SensorPlugin::SensorPlugin()
    : SinglePortPlugin("sensor", PortNum_PRIVATE_APP),
    concurrency::OSThread("sensor")
{
}

bool SensorPlugin::handleReceived(const MeshPacket &req)
{
    auto &p = req.decoded.data;
    // The incoming message is in p.payload
    DEBUG_MSG("Received message from=0x%0x, id=%d, msg=%.*s\n", req.from, req.id, p.payload.size, p.payload.bytes);

    return true; // We handled it
}


int32_t SensorPlugin::runOnce() {
    DEBUG_MSG("Hi :) ");

    return 10;
}