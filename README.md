# poolhouse
messuring things and viualising them via lorawan technology making use of the TTN (The Things Network) and cayenne
# note 
see full project on https://github.com/kiwiton/poolhouse/tree/readmeEdits
# components
In this projec I make use of 
-	an arduino uno with an lora shield,
-	a rasberryPi4 with a lora/gps hat,
-	dht11 sensor,
-	ldr,
-	some LED's,
-	a ventilator,
-	and a button,

# rasberryPi
We're using our Pi as gateway to the TTN so we can integrate with cayenne.

First let's start with enabeling spi on youre Pi.
type in youre CLI: "sudo raspi-config"
select the 5th option interfacing options.
