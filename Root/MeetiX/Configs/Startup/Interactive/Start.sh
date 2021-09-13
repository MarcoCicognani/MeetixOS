printf:"Starting video driver"
driver:"/MeetiX/Kernel/Servers/VBE.sv"
wait:"vbedriver"

printf:"Starting zipNET windowserver"
exec:"/MeetiX/Kernel/Servers/ZipNET.sv"
wait:"zipNET/registration"

printf:"Starting CandyGui"
exec:"/Apps/OsmosUI/Bin/OsmosUI"
