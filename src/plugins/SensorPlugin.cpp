#include "configuration.h"
#include "SensorPlugin.h"
#include "MeshService.h"
#include "main.h"

// #include "concurrency/OSThread.h"

#include <assert.h>

// Create an a static instance of our plugin - this registers with the plugin system
SensorPlugin sensorPlugin;



SensorPlugin::SensorPlugin()
    : SinglePortPlugin("sensor", PortNum_TEXT_MESSAGE_APP),
    concurrency::OSThread("sensor")
{
}

void SensorPlugin::sendMessage(){
    DEBUG_MSG("Sending Message!\n");
    screen->print("Sending Message!\n");

    const char *message = "Hello there";

    auto packet = allocDataPacket();

    packet->decoded.data.payload.size = strlen(message);

    memcpy(packet->decoded.data.payload.bytes, message, packet->decoded.data.payload.size);

    service.sendToMesh(packet);

}

bool SensorPlugin::handleReceived(const MeshPacket &req)
{
    auto &p = req.decoded.data;
    // The incoming message is in p.payload
    DEBUG_MSG("Received message from=0x%0x, id=%d, msg=%.*s\n", req.from, req.id, p.payload.size, p.payload.bytes);


    return true; // We handled it
}


int32_t SensorPlugin::runOnce() {
    DEBUG_MSG("This is in the thread \nHi :) \n");

    // screen->print("Does this work?");
    // if (millis())
    // {
    //     /* code */
    // }
    sensorPlugin.sendMessage();
    

    return 10000;
}