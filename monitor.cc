#include "monitor.h"
#include "intersection.h"

Monitor::Monitor(sc_module_name name) : sc_module(name) {

  SC_METHOD(check_traffic_lights_status);
  dont_initialize();
  for (size_t i{0}; i < 4; ++i) {
    sensitive << traffic_lights[i];
  }
}

void Monitor::check_traffic_lights_status() {

  os << "At time: " << sc_time_stamp() << std::endl;
  for (size_t i{0}; i < 4; ++i) {
    auto &&traffic_light{traffic_lights[i]};
    if (traffic_light->read() == GREEN) {
      assert(traffic_lights[(i + 1) % 4]->read() == RED);
      assert(traffic_lights[(i + 3) % 4]->read() == RED);
    }
    if (traffic_light->read() == RED) {
      timers[i] = 0;
    }

    if (traffic_light->read() == GREEN) {
      if (timestamp != sc_time_stamp()) {
        ++timers[i];
      }
      assert(timers[i] <= TIMEOUT);
    }
    timestamp = sc_time_stamp();

    if(traffic_light->read() == GREEN) {
     assert(car_status[i]->read());
    }
    os << i << ": " << (traffic_light->read() ? "GREEN" : "RED") << ";"
       << (car_status[i]->read() ? "Cars here" : "No cars") << std::endl;
  }
}
