#include "configuration.h"
#include "SensorPlugin.h"
#include "MeshService.h"
#include "main.h"

#include <CayenneLPP.h>

// #include "concurrency/OSThread.h"

#include <assert.h>

// Create an a static instance of our plugin - this registers with the plugin system
SensorPlugin sensorPlugin;



SensorPlugin::SensorPlugin()
    : SinglePortPlugin("sensor", PortNum_TEXT_MESSAGE_APP),
    concurrency::OSThread("sensor")
{
}

void SensorPlugin::sendMessage(const char * message, size_t size){
    DEBUG_MSG("Sending Message!\n");
    screen->print("Sending Message!\n");

    // const char *message = "Hello there";

    CayenneLPP lpp(160);

    float temperature = random(20, 30) + (random(0,99)/100.00);
    // float humidity = 65;//random(20,100) + (random(0-99) / 100.00);
    // // float pressure = 1012.00;

    lpp.reset();
    lpp.addTemperature(1, temperature);
    lpp.addRelativeHumidity(2, 65);

    // uint8_t buffer[4];

    // lpp.copy(buffer);

    auto packet = allocDataPacket();

    packet->decoded.data.payload.size = lpp.getSize();
    // packet->decoded.data.payload.bytes = lpp.getBuffer();
    memcpy(packet->decoded.data.payload.bytes, lpp.getBuffer(), packet->decoded.data.payload.size);

    DEBUG_MSG("LPP Message: Size %d \nMessage in bytes: ", packet->decoded.data.payload.size);
    for (uint8_t i = 0; i < packet->decoded.data.payload.size; i++)
    {
        /* code */
        DEBUG_MSG("%02X ", packet->decoded.data.payload.bytes[i]);
    }
    DEBUG_MSG("\n");
    // Serial2.write(lpp.getBuffer());

    service.sendToMesh(packet);

}

bool SensorPlugin::handleReceived(const MeshPacket &req)
{
    auto &p = req.decoded.data;
    // The incoming message is in p.payload

    DynamicJsonDocument jsonBuffer(4096);
    CayenneLPP lpp(160);

    JsonArray root = jsonBuffer.to<JsonArray>();


    DEBUG_MSG("Received message from=0x%0x, id=%d, msg=%.*s\n", req.from, req.id, p.payload.size, p.payload.bytes);

    uint8_t buffer[100];

    memcpy(buffer, p.payload.bytes, p.payload.size);

    lpp.decode(buffer, p.payload.size, root);



    serializeJsonPretty(root, console);


    return false; // We handled it
}


int32_t SensorPlugin::runOnce() {
    DEBUG_MSG("This is in the thread \nHi :) \n");

    // screen->print("Does this work?");
    if (millis() - lastTime >= 10000)
    {
        /* code */
        const char *message = "Hello there";

        sensorPlugin.sendMessage(message, strlen(message));

        lastTime = millis();
    }   

    return 10000;
}