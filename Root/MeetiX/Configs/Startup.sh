log:"Starting Input Driver"
spawn_driver:"/MeetiX/Kernel/Servers/Ps2.sv"

log:"Starting Video Driver"
spawn_driver:"/MeetiX/Kernel/Servers/VBE.sv"
wait_for_id:"vbe"

log:"Starting MeetiX Terminal"
spawn_app:"/Apps/Terminal/Bin/Terminal" args:"--headless=Gui"
