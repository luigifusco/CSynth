#include "output.h"

#include <sys/soundcard.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>

int output_open_default_device(struct SoundIo *soundio, struct SoundIoOutStream *out) {
    soundio = soundio_create();

    if  (!soundio) return -1;
    if  (soundio_connect(soundio)) return -1;

    soundio_flush_events(soundio);

    int default_out = soundio_default_output_device_index(soundio);
    if (default_out < 0) return -1;

    struct SoundIoDevice *device = soundio_get_output_device(soundio, default_out);
    if (!device) return -1;

    out = soundio_outstream_create(device);
    out->format = SoundIoFormatFloat32NE;
    
    return 0;
}

void output_generator_noise(struct SoundIoOutStream *out, int frame_min, int frame_max) {
    const struct SoundIoChannelLayout *layout = &out->layout;
    float sr = out->sample_rate;
    float secs_per_frame = 1.0f / sr;
    struct SoundIoChannelArea *areas;
    int frames_left = frame_max;

    while (frames_left > 0) {
        int frame_count = frames_left;
        if (soundio_outstream_begin_write(out, &areas, &frame_count)) exit(EXIT_FAILURE);

        if (!frame_count) break;

        for (int frame = 0; frame < frame_count; ++frame) {
            float sample = (float)rand()/(float)RAND_MAX;
            for (int channel = 0; channel < layout->channel_count; ++channel) {
                float *ptr = (float*)(areas[channel].ptr + areas[channel].step * frame);
                *ptr = sample;
            }
        }

        if(soundio_outstream_end_write(out)) exit(EXIT_FAILURE);

        frames_left -= frame_count;
    }

}
