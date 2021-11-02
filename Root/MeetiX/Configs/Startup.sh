log:"Starting Input Driver"
spawn_driver:"/MeetiX/Kernel/Servers/Input.sv"

log:"Starting Video Driver"
spawn_driver:"/MeetiX/Kernel/Servers/Video.sv"
wait_for_id:"Video"

log:"Starting MeetiX Terminal"
spawn_app:"/Apps/Terminal/Bin/Terminal" args:"--headless=Gui"
