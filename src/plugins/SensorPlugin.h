#pragma once
#include "SinglePortPlugin.h"
#include "concurrency/OSThread.h"


/**
 * A simple example plugin that just replies with "Message received" to any message it receives.
 */
class SensorPlugin : public SinglePortPlugin, private concurrency::OSThread
{
  public:
    /** Constructor
     * name is for debugging output
     */
    SensorPlugin(); //: SinglePortPlugin("sensor", PortNum_PRIVATE_APP) {}

  protected:

    /** Called to handle a particular incoming message

    @return true if you've guaranteed you've handled this message and no other handlers should be considered for it
    */
    virtual bool handleReceived(const MeshPacket &mp);

    virtual int32_t runOnce();
};
