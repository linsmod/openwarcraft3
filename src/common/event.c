#include "event.h"

void Event_StopPropagation(event_t *event) {
    if (event) {
        event->propagation_stopped = true;
    }
}

void Event_PreventDefault(event_t *event) {
    if (event) {
        event->default_prevented = true;
    }
}
