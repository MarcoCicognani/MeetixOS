printf:"Starting time driver"
driver:"/MeetiX/Kernel/Servers/Time.sv"

printf:"Starting ps2 driver"
driver:"/MeetiX/Kernel/Servers/Ps2.sv"

printf:"Starting video driver"
driver:"/MeetiX/Kernel/Servers/VBE.sv"
wait:"vbe"

printf:"Starting zipNET WindowServer"
exec:"/MeetiX/Kernel/Servers/ZipNET.sv"
wait:"ZipNET::registration"

printf:"Starting MXLogin"
exec:"/Apps/MXLogin/Bin/MXLogin"
