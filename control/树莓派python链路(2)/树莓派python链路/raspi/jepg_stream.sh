#!/bin/bash
cd /usr/local/bin/
mjpg_streamer -i "/usr/local/lib/mjpg-streamer/input_uvc.so -n -f 24 -r 640x480" -o "/usr/local/lib/mjpg-streamer/output_http.so -p 8080 -w /usr/local/share/mjpg-streamer/www"
