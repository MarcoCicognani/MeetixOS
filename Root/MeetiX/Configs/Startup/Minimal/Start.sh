printf:"Starting time driver"
driver:"/MeetiX/Kernel/Servers/Time.sv"

printf:"Starting ps2 driver"
driver:"/MeetiX/Kernel/Servers/Ps2.sv"

printf:"Starting video driver"
driver:"/MeetiX/Kernel/Servers/VBE.sv"
wait:"vbe"

printf:"Starting zipNET windowserver"
exec:"/MeetiX/Kernel/Servers/ZipNET.sv"
wait:"zipNET::registration"

printf:"Starting MXLogin"
exec:"/Apps/MXLogin/Bin/MXLogin"
