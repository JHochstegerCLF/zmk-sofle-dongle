# Central Dongle specific source files
if (CONFIG_SHIELD_EYELASH_SOFLE_CENTRAL_DONGLE)
    zephyr_library_sources(src/dongle/custom_status_screen.c)
    zephyr_library_sources(src/dongle/widgets/heartbeat.c)
    zephyr_library_sources(src/dongle/widgets/profile_status.c)
    zephyr_library_sources(src/dongle/widgets/peripheral_battery.c)
endif()
