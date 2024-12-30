#include <iostream>

#include <NinjaHSM/NinjaHSM.hpp>

using namespace NinjaHSM;

//==============================================================================
// Events
//==============================================================================

enum class EventId {
    EVENT_WITH_NO_DATA,
    EVENT_WITH_DATA_1,
    EVENT_WITH_DATA_2,
};

class EventWithData1 {
public:
    EventWithData1(uint32_t data) : data(data) {}
    uint32_t data;
};

class EventWithData2 {
public:
    EventWithData2(int64_t data) : data(data) {}
    int64_t data;
};

class EventWrapper {
public:
    EventWrapper(EventId id) : id(id) {}
    EventId id;
    union {
        EventWithData1 data1;
        EventWithData2 data2;
    };
};

int main() {
    std::cout << "basic_example running..." << std::endl;
    return 0;
}
