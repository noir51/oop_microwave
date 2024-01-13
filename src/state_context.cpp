
#include <typeinfo>

#include <logger.h>
#include <state_context.h>
#include <state_cooking.h>
#include <state_door_closed.h>
#include <state_door_opened.h>

StateContext::StateContext()
{
    countdown__ = MicrowaveCountdown::get_instance();
}

StateContext::~StateContext() {
    purge_state();
    observers__.clear();

    if (StateCooking::is_alive()) {
        StateCooking::destroy_instance();
    }
    if (StateDoorClosed::is_alive()) {
        StateDoorClosed::destroy_instance();
    }
    if (StateDoorOpened::is_alive()) {
        StateDoorOpened::destroy_instance();
    }
}

void
StateContext::transition_to(BaseState *state)
{
    if (state__ != nullptr) {
        state__ = nullptr;
    }
    state__ = state;
    state__->set_context(this);
    notify_observers();

    Logger::log("StateContext", "Application transitioned to BaseState object of type %s", typeid(*state).name());
}

void
StateContext::purge_state()
{
    if (state__ == nullptr) {
        return;
    }

    Logger::log("StateContext", "Purging BaseState object of type %s", typeid(*state__).name());

    state__->purge_state();
    state__ = nullptr;
}

void
StateContext::open_door()
{
    state__->open_door();
}

void
StateContext::close_door()
{
    state__->close_door();
}

void
StateContext::cook(int microwave_time)
{
    state__->cook(microwave_time);
}

void
StateContext::start_countdown(int microwave_time)
{
    countdown__->start(microwave_time);
}

void
StateContext::increase_countdown(int microwave_time)
{
    countdown__->add_time(microwave_time);
}

void
StateContext::stop_countdown()
{
    countdown__->stop();
}

void
StateContext::reset_countdown()
{
    countdown__->reset();
}

void
StateContext::register_observer(IObserver *observer)
{
    observers__.insert(observer);
}

void
StateContext::unregister_observer(IObserver *observer)
{
    auto it = observers__.find(observer);
    if (it != observers__.end()) {
        observers__.erase(observer);
    }
}

void
StateContext::notify_observers()
{
    for (IObserver *observer : observers__) {
        observer->update(STATE_CONTEXT_NOTIFY_ID);
    }
}

std::string
StateContext::get_state_name() const
{
    return state__->get_name();
}

std::string
StateContext::get_state_description() const
{
    return state__->get_description();
}

int
StateContext::get_countdown_time() const
{
    return countdown__->get_time();
}

int
StateContext::is_countdown_running() const
{
    return countdown__->is_running();
}
