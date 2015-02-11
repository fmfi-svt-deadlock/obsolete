#include "hal.h"
#include "leds.h"

uint8_t hal_leds_init() {

    // Set GPIO data direction on leds outputs
    LED_R_DDR |= bit(LED_R_PIN);
    LED_G_DDR |= bit(LED_G_PIN);
    LED_B_DDR |= bit(LED_B_PIN);

    return 0;
}

uint8_t hal_leds_get_status() {
    uint8_t status = 0;
    if (is_set(LED_R_PORT, bit(LED_R_PIN))) status |= bit(HAL_FLAG_RED_LED);
    if (is_set(LED_G_PORT, bit(LED_G_PIN))) status |= bit(HAL_FLAG_GREEN_LED);
    if (is_set(LED_B_PORT, bit(LED_B_PIN))) status |= bit(HAL_FLAG_BLUE_LED);
    return status;
}

void hal_leds_set_status(uint8_t status) {
    if (is_set(status, bit(HAL_FLAG_RED_LED))) LED_R_PORT |= bit(LED_R_PIN);
    else LED_R_PORT &= ~bit(LED_R_PIN);

    if (is_set(status, bit(HAL_FLAG_GREEN_LED))) LED_G_PORT |= bit(LED_G_PIN);
    else LED_G_PORT &= ~bit(LED_G_PIN);

    if (is_set(status, bit(HAL_FLAG_BLUE_LED))) LED_B_PORT |= bit(LED_B_PIN);
    else LED_B_PORT &= ~bit(LED_B_PIN);
}
