#ifndef TELEMETRY_H
#define TELEMETRY_H

typedef enum {
    FAILURE_NONE,
    FAILURE_ENGINE_OVERHEAT,
    FAILURE_FUEL_LEAK,
    FAILURE_ALTITUDE_SENSOR_STUCK
} FailureMode;

typedef struct {
    double altitude;
    double airspeed;
    double heading;
    double vertical_speed;
    double engine_rpm;
    double cabin_pressure;
    double fuel_level;
    double engine_temp;
    long   timestamp;
    int failure_timer;
    FailureMode failure_mode;
} TelemetryFrame;

void init_frame(TelemetryFrame *frame);
void update_frame(TelemetryFrame *frame);
void inject_failure(TelemetryFrame *frame);
void print_frame(TelemetryFrame frame);
void check_anomalies(TelemetryFrame frame);
void log_frame(TelemetryFrame frame);

#endif