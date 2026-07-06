# Flight Telemetry Simulator

A C program that simulates real-time aircraft telemetry: altitude, airspeed, heading, fuel level, engine temp, plus randomized fault injection (engine overheat, fuel leaks, stuck altitude sensor).

## What it does

- Generates a telemetry frame once a second and updates it over time
- Randomly injects failures to simulate real sensor/engine problems
- Logs every frame to `telemetry.log` as CSV
- Checks each frame for anomalies (low fuel, high engine temp, low cabin pressure, unsafe altitude) and prints warnings
- Every 5 frames, sends a summary out over HTTP using libcurl

## Files

- `main.c`, main loop
- `telemetry.c` / `telemetry.h`, frame generation, fault injection, logging, anomaly checks
- `reporter.c` / `reporter.h`, sends telemetry summaries out over HTTP

## Building and running

Needs `libcurl` installed.

```bash
make        # builds the "flight" binary
make run    # builds and runs it
make clean  # removes the binary and log file
```

It runs forever (Ctrl+C to stop), printing a new frame every second.
