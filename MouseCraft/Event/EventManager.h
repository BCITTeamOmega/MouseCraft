#pragma once

//created by main

#include <map>
#include <set>
#include <thread>
#include "ISubscriber.h"
#include "EventName.h"

/**
    Event Manager is a static class that dispatches messages between various components of the engine.
    Any class that implements ISubscriber can subscribe to events to be notified when that event is triggered.
*/
class EventManager {
public:
    // Use to notify subscribers of the specified event passing the given parameter
    static void Notify(EventName eventName, Param* params, bool async = false);

    // Subscribes the specified subscriber to an event
    static void Subscribe(EventName eventName, ISubscriber* subscriber);

    // Unsubscribes the specified subscriber from an event
    static void Unsubscribe(EventName eventName, ISubscriber* subscriber);
private:
    static std::map<EventName, std::set<ISubscriber*>> _eventMap;
    static void notifySubscribers(EventName eventName, Param* params);

    EventManager() {}
};