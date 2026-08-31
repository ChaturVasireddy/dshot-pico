#include "dshot.h"
#include "dshot.pio.h"
typedef struct {
    PIO pio;
    uint sm;
    uint offset;
    uint pin;
} dshot_actuator;

void dshot_init(dshot_actuator *actuator) {
        bool success = pio_claim_free_sm_and_add_program_for_gpio_range(&dshot_program, &actuator->pio, &actuator->sm, &actuator->offset, actuator->pin, 1, true);
        hard_assert(success);
        dshot_program_init(actuator->pio, actuator->sm, actuator->offset, actuator->pin);
}

void dshot_arm(const dshot_actuator *actuator) {
    for (int i = 0;i < 500;i++) {       //potential bug, need testing
        pio_sm_put_blocking(actuator->pio, actuator->sm, 0x00000000 << 16); //arming
        sleep_us(700);
    }
}

void mode3d(const dshot_actuator *actuator) {
    for (int i = 0;i < 10;i++) {
        pio_sm_put_blocking(actuator->pio, actuator->sm, (uint32_t)0x0145 << 16); //3dmode
        sleep_us(700);
    }
}

void thrust(const dshot_actuator *actuator, uint16_t throttle) {
        throttle &= 0x7FF;
        uint16_t packet = (throttle << 1) | 0;
        uint16_t crc = (packet ^ (packet >> 4) ^ (packet >> 8)) & 0x0F;         //calulating 4bit CRC
        uint16_t escframe = (packet << 4) | crc;        //final 16bit frame that needs to be sent
        pio_sm_put_blocking(actuator->pio, actuator->sm, (uint32_t)escframe << 16);
}

// int main (){
//     dshot_actuator T1 = {
//         .pin = 2
//     };
//     dshot_init(&T1);
//     dshot_arm(&T1);
//     thrust(&T1, 1000);
// }