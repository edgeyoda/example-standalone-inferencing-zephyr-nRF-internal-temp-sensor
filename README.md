# Edge Impulse Example: stand-alone inferencing (Zephyr) with nRF internal (die) temp sensor support

This is a modified version of the Edge Impulse stand-alone inferencing (Zephyr) example with nRF internal (die) temp sensor support. See the documentation at [Running your impulse locally (Zephyr)](https://docs.edgeimpulse.com/docs/running-your-impulse-locally-zephyr).

## Tested on

This example has been tested on the nRF52840dk but should work on all the following boards:

* [nrf52dk_nrf52832](https://docs.zephyrproject.org/latest/boards/arm/nrf52dk_nrf52832/doc/index.html)
* [nRF52840dk_nrf52840](https://docs.zephyrproject.org/latest/boards/arm/nrf52840dk_nrf52840/doc/index.html)
* [nRF5340dk_nrf5340](https://docs.zephyrproject.org/latest/boards/arm/nrf5340dk_nrf5340/doc/index.html)
* [nrf9160dk_nrf9160](https://docs.zephyrproject.org/latest/boards/arm/nrf9160dk_nrf9160/doc/index.html)

But should run as-is on most development boards. If you run this example on a non-Arm development board you'll want to disable the CMSIS-DSP and CMSIS-NN macros in [CMakeLists.txt](CMakeLists.txt).
