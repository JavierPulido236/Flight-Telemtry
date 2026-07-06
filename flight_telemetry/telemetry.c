#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <curl/curl.h>
#include "telemetry.h"


void init_frame(TelemetryFrame *frame) {
    frame->timestamp      = time(NULL);
    frame->altitude       = 33000.0;
    frame->airspeed       = 500.0;
    frame->heading        = 270.0;
    frame->vertical_speed = 0.0;
    frame->engine_rpm     = 2400.0;
    frame->cabin_pressure = 11.5;
    frame->fuel_level     = 80.0;
    frame->engine_temp    = 65.0;
    frame->failure_timer = 0;

    frame->failure_mode = FAILURE_NONE;
}

void inject_failure(TelemetryFrame *frame) {
    if (frame->failure_timer > 0) {
        frame->failure_timer--;
        return;
    }

    int r = rand() % 100;

    if (r < 5) {
        frame->failure_mode = FAILURE_ENGINE_OVERHEAT;
        frame->failure_timer = 10;
    }
    else if (r < 10) {
        frame->failure_mode = FAILURE_FUEL_LEAK;
        frame->failure_timer = 10;
    }
    else if (r < 15) {
        frame->failure_mode = FAILURE_ALTITUDE_SENSOR_STUCK;
        frame->failure_timer = 10;
    } else {
        frame->failure_mode = FAILURE_NONE;
    }
}

void update_frame(TelemetryFrame *frame) {
    frame->timestamp = time(NULL);

    frame->heading += 2.0;
    if (frame->heading >= 360.0) {
        frame->heading -= 360.0;
    }

    frame->fuel_level -= 0.2;
    if (frame->fuel_level < 0.0) {
        frame->fuel_level = 0.0;
    }

    if (frame->failure_mode == FAILURE_ENGINE_OVERHEAT) {
        frame->engine_temp += 5.0;
    }

    if (frame->failure_mode == FAILURE_FUEL_LEAK) {
        frame->fuel_level -= 2.0;
    }

    if (frame->failure_mode != FAILURE_ALTITUDE_SENSOR_STUCK) {
        frame->altitude += (rand() % 21) - 10;
    }

    frame->engine_temp += ((rand() % 5) - 2) * 0.2;
}

const char *failure_to_string(FailureMode mode) {
    switch (mode) {
        case FAILURE_ENGINE_OVERHEAT:
            return "ENGINE_OVERHEAT";

        case FAILURE_FUEL_LEAK:
            return "FUEL_LEAK";

        case FAILURE_ALTITUDE_SENSOR_STUCK:
            return "ALTITUDE_SENSOR_STUCK";

        default:
            return "NONE";
    }
}

void print_frame(TelemetryFrame frame) {
    printf("---- Telemetry Frame ---\n");
    printf("Timestamp:      %ld\n",      frame.timestamp);
    printf("Altitude:       %.1f ft\n",  frame.altitude);
    printf("Airspeed:       %.1f kts\n", frame.airspeed);
    printf("Heading:        %.1f deg\n", frame.heading);
    printf("Vertical speed: %.1f fpm\n", frame.vertical_speed);
    printf("Engine RPM:     %.1f\n",     frame.engine_rpm);
    printf("Cabin pressure: %.2f psi\n", frame.cabin_pressure);
    printf("Fuel level:     %.1f%%\n",   frame.fuel_level);
    printf("Engine temp:    %.1f C\n",   frame.engine_temp);
    printf("Failure mode:   %s\n",       failure_to_string(frame.failure_mode));
}

void check_anomalies(TelemetryFrame frame) {
    if (frame.fuel_level < 20.0) {
        printf("WARNING: Low fuel level!\n");
    }

    if (frame.engine_temp > 100.0) {
        printf("WARNING: Engine temperature high!\n");
    }

    if (frame.cabin_pressure < 10.0) {
        printf("WARNING: Cabin pressure low!\n");
    }

    if (frame.altitude > 40000.0) {
        printf("WARNING: Altitude exceeds safe limit!\n");
    }
}

void log_frame(TelemetryFrame frame) {
    FILE *file = fopen("telemetry.log", "a+");

    if (file == NULL) {
        printf("ERROR: Could not open log file.\n");
        return;
    }

    fseek(file, 0, SEEK_END);

    if (ftell(file) == 0) {
        fprintf(file,
            "timestamp,altitude,airspeed,heading,vertical_speed,"
            "engine_rpm,cabin_pressure,fuel_level,engine_temp,failure_mode\n");
    }

    fprintf(file,
        "%ld,%.1f,%.1f,%.1f,%.1f,%.1f,%.2f,%.1f,%.1f,%d\n",
        frame.timestamp,
        frame.altitude,
        frame.airspeed,
        frame.heading,
        frame.vertical_speed,
        frame.engine_rpm,
        frame.cabin_pressure,
        frame.fuel_level,
        frame.engine_temp,
        frame.failure_mode
    );

    fclose(file);
}



