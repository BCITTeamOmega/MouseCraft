#include "EventManager.h"
std::map<EventName, std::set<ISubscriber*>> EventManager::_eventMap;

void EventManager::Notify(EventName eventName, Param* params, bool async) {
    if (async) {
        std::thread t(&notifySubscribers, eventName, params);
        t.detach();
    } else {
        notifySubscribers(eventName, params);
    }
}

void EventManager::Subscribe(EventName eventName, ISubscriber* subscriber) {
    auto event = _eventMap.find(eventName);
    if (event != _eventMap.end()) {
        event->second.insert(subscriber);
    } else {
        std::set<ISubscriber*> subscriberList;
        subscriberList.insert(subscriber);
        _eventMap.insert(std::pair<EventName,std::set<ISubscriber*>>(eventName, subscriberList));
    }
}

void EventManager::Unsubscribe(EventName eventName, ISubscriber* subscriber) {
    auto event = _eventMap.find(eventName);
    if (event != _eventMap.end()) {
        auto it = event->second.find(subscriber);
        if (it != event->second.end()) {
            event->second.erase(it);
        }
    }
}

void EventManager::notifySubscribers(EventName eventName, Param* params) {
    auto event = _eventMap.find(eventName);
    if (event != _eventMap.end()) {
        std::set<ISubscriber*> subscriberList = event->second;

        for (auto subscriber : subscriberList) {
            subscriber->Notify(eventName, params);
        }
    }
}