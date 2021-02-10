target_include_directories(munfc-test
    PRIVATE
    .
)

target_sources(munfc-test
    PRIVATE
    fake_nfc_scheduler_timer.h
    fake_nfc_scheduler_timer.cpp
    tech/isodep/isodep_target.cpp
    tech/isodep/mock_transceiver.h
    tech/isodep/mock_transceiver.cpp
)