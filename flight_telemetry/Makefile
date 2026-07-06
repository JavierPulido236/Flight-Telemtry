flight: main.c telemetry.c reporter.c
	cc main.c telemetry.c reporter.c -lcurl -o flight

run: flight
	./flight

clean:
	rm -f flight telemetry.log