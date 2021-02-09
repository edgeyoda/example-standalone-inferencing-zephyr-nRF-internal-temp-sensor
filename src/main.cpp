#include <zephyr.h>
#include "ei_run_classifier.h"
#include "numpy.hpp"
#include <drivers/sensor.h>

float features[8] = {0};

int raw_feature_get_data(size_t offset, size_t length, float *out_ptr) {
    memcpy(out_ptr, features + offset, length * sizeof(float));
    return 0;
}

struct sensor_value temp;
float fahrenheit = 0;


#define TEMP_NODE DT_INST(0, nordic_nrf_temp)
#define TEMP_LABEL DT_LABEL(TEMP_NODE)

const struct device *chip = device_get_binding(TEMP_LABEL);
                
int main() {
    // This is needed so that output of printf is output immediately without buffering
    setvbuf(stdout, NULL, _IONBF, 0);

    printk("Edge Impulse standalone inferencing (Zephyr)\n");

    if (sizeof(features) / sizeof(float) != EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE) {
        printk("The size of your 'features' array is not correct. Expected %d items, but had %u\n",
            EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE, sizeof(features) / sizeof(float));
        return 1;
    }

    ei_impulse_result_t result = { 0 };

  while (1) {
        // the features are stored into flash, and we don't want to load everything into RAM
        signal_t features_signal;

        for (size_t i = 0; i < EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE; i++)
        {
          sensor_sample_fetch(chip);
          sensor_channel_get(chip, SENSOR_CHAN_DIE_TEMP, &temp);
          fahrenheit = (float)(((sensor_value_to_double(&temp) * 9)/5) + 32);

          // debug
          // printk("Chip die Temperature: %.2f F\n", fahrenheit);

          features[i] = fahrenheit;
          k_msleep(500);
        }

        int err = numpy::signal_from_buffer(features, EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE, &features_signal);

        // invoke the impulse
        EI_IMPULSE_ERROR res = run_classifier(&features_signal, &result, true);
        printk("run_classifier returned: %d\n", res);

        if (res != 0) return 1;

        printk("Predictions (DSP: %d ms., Classification: %d ms., Anomaly: %d ms.): \n",
            result.timing.dsp, result.timing.classification, result.timing.anomaly);

#if EI_CLASSIFIER_HAS_ANOMALY == 1
        ei_printf_float(result.anomaly);
#endif
        k_msleep(500);
  }
}