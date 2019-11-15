#include <iostream>
#include <queue>
#include <string>

struct event_t
{
    static const int ARRIVAL = 1;
    static const int READY = 2;
    static const int DEPARTURE = 3;

    static const int STATION_A = 1;
    static const int STATION_B = 2;

    event_t(int _code, int _location, std::string _time) :
        type_code(_code), location(_location), event_time(_time) {}

    int type_code;
    int location;
    std::string event_time;

    bool operator< (const event_t& rhs) const
        {
            if(event_time == rhs.event_time) {
                return type_code > rhs.type_code;
            }
            return event_time > rhs.event_time;
        }
};

typedef std::priority_queue<event_t> event_queue_t;

struct solution_t
{
    int trains_at_a, trains_at_b;
};

std::string
add_minutes(const std::string& s, int delta)
{
    int hours = ::atoi(s.substr(0, 2).c_str());
    int minutes = ::atoi(s.substr(3, 2).c_str());
    minutes += delta;
    while(minutes > 59)
    {
        ++hours;
        minutes -= 60;
    }
    char buffer[6];
    sprintf(buffer, "%02d:%02d", hours, minutes);
    return std::string(buffer);
}

solution_t
solve(int turnaround, event_queue_t eventq)
{
    solution_t solution;
    solution.trains_at_a = 0;
    solution.trains_at_b = 0;
    int num_ready_a = 0, num_ready_b = 0;
    while(!eventq.empty())
    {
        event_t event = eventq.top();
        eventq.pop();

        std::cerr << "Event at " << event.event_time << "; code=";
        switch(event.type_code)
        {
        case event_t::ARRIVAL:     std::cerr << "Arrival"; break;
        case event_t::DEPARTURE:   std::cerr << "Departure"; break;
        case event_t::READY:       std::cerr << "Ready"; break;
        }
        std::cerr << "; location=" << event.location << std::endl;
        switch(event.type_code)
        {
        case event_t::ARRIVAL:
            if(turnaround > 0)
                eventq.push(event_t(event_t::READY, event.location, add_minutes(event.event_time, turnaround)));
            else
                if(event.location==event_t::STATION_A)
                    ++num_ready_a;
                else
                    ++num_ready_b;
            break;
        case event_t::DEPARTURE:
            if(event.location==event_t::STATION_A) {
                if(num_ready_a > 0) {
                    --num_ready_a;
                } else {
                    ++solution.trains_at_a;
                }
            } else {
                if(num_ready_b > 0) {
                    --num_ready_b;
                } else {
                    ++solution.trains_at_b;
                }
            }
            break;
        case event_t::READY:
            if(event.location==event_t::STATION_A)
                ++num_ready_a;
            else
                ++num_ready_b;
            break;
        }
    }
    return solution;
}

int
main(int argc, char *argv[])
{
    int ncases;
    std::cin >> ncases;
    for(int i = 1; i <= ncases; ++i)
    {
        event_queue_t eventq;

        int turnaround, num_atob, num_btoa;
        std::cin >> turnaround >> num_atob >> num_btoa;
        for(int j = 0; j < num_atob; ++j)
        {
            std::string departure, arrival;
            std::cin >> departure >> arrival;

            eventq.push(event_t(event_t::DEPARTURE, event_t::STATION_A, departure));
            eventq.push(event_t(event_t::ARRIVAL, event_t::STATION_B, arrival));
        }

        for(int j = 0; j < num_btoa; ++j)
        {
            std::string departure, arrival;
            std::cin >> departure >> arrival;

            eventq.push(event_t(event_t::DEPARTURE, event_t::STATION_B, departure));
            eventq.push(event_t(event_t::ARRIVAL, event_t::STATION_A, arrival));
        }

        solution_t solution = solve(turnaround, eventq);

        std::cout << "Case #" << i << ": " << solution.trains_at_a << " " << solution.trains_at_b << std::endl;
    }
}
