printf:"Starting time driver"
driver:"/sys/eva/server/time.sv"

printf:"Starting ps2 driver"
driver:"/sys/eva/server/ps2.sv"

printf:"Starting video driver"
driver:"/sys/eva/server/vbe.sv"
wait:"vbe"

printf:"Starting zipNET windowserver"
exec:"/sys/eva/server/zipNET.sv"
wait:"zipNET::registration"

printf:"Starting MXLogin"
exec:"/app/MXLogin/bin/MXLogin"
