# Wayland menu sizing regression

Build this test with qmake6 and make in a separate directory. Run with
`QT_QPA_PLATFORM=wayland` in a live Wayland session. To test a local DTK build,
set `LD_PRELOAD` to that build's `libdtk2widget.so.6.0.1`.

The test requires menu polishing to leave the native window uncreated. It then
checks an unparented menu's first popup, repeated popup and size after adding an
action. Each popup must match sizeHint in both QWidget and QWindow geometry.
The test closes its own windows automatically. Exit 77 means Wayland is absent.
