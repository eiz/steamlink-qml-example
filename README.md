# steamlink-qml-example

A minimal Qt Quick application for the Steam Link. To build, set the `STEAMLINK_HOME` environment variable to the Steam Link SDK root and run `./build_steamlink.sh`.

To install, either follow Valve's USB stick install instructions, or enable ssh on your Steam Link and copy `steamapps/apps/steamlink-qml-example` to `/home/steam/apps` on the device and run `kill $(pidof shell)` to restart the Steam Link shell.

