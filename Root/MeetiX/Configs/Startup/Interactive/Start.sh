printf:"Starting video driver"
driver:"/sys/eva/server/vbedriver.sv"
wait:"vbedriver"

printf:"Starting zipNET windowserver"
exec:"/sys/eva/server/zipNET"
wait:"zipNET/registration"

printf:"Starting CandyGui"
exec:"/app/OsmosUI/bin/OsmosUI"
