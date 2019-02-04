#pragma once

#include "EventName.h"

// Custom generic parameter type that can be safely cast to other types using TypeParam template
class Param {
public:
    virtual ~Param() {};
};

template <typename T>
struct TypeParam:Param {
    TypeParam(T p) : Param(p) {}
    T Param;
};

// Subscriber Interface is to be inherited by all classes that want to be notified by the Event Manager
class ISubscriber {
public:
    // Action to take when notified by an event
    virtual void Notify(EventName eventName, Param *params) = 0;
};