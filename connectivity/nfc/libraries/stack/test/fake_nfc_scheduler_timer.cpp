#include "nfc_scheduler.h"
#include "fake_nfc_scheduler_timer.h"

static uint32_t timer_value = 0;

void nfc_scheduler_timer_fake_set_value(uint32_t value) {
    timer_value = value;
}

void nfc_scheduler_timer_init(nfc_scheduler_timer_t *timer) {

}

void nfc_scheduler_timer_start(nfc_scheduler_timer_t *timer) {

}

uint32_t nfc_scheduler_timer_get(nfc_scheduler_timer_t *timer) {
    return timer_value;
}

void nfc_scheduler_timer_stop(nfc_scheduler_timer_t *timer) {

}

void nfc_scheduler_timer_reset(nfc_scheduler_timer_t *timer) {

}