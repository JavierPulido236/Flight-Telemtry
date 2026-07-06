#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "telemetry.h"
#include "reporter.h"


int counter = 0;

int main() {
    TelemetryFrame frame;

    srand(time(NULL));

    init_frame(&frame);

    while (1) {
        inject_failure(&frame);
        update_frame(&frame);
        print_frame(frame);
        check_anomalies(frame);
        log_frame(frame);

        counter++;

        if (counter >= 5) {
            send_telemetry_report(frame);
            counter = 0;
        }

        sleep(1);
    }

     return 0;
}