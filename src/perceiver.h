#include <esp_task.h>
#include <vector>
#include <DHT.h>

class Perceiver {
    public:
    Perceiver(std::vector<DHT> dhtContainer);
        
    private:
    std::vector<TimerHandle_t> TimerHandles;
};
