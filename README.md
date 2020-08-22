# 	poolhouse
messuring things and viualising them via lorawan technology 
# 	components
In this projec I make use of 
	an arduino uno with an lora shield,
	a rasberryPi4 with a lora/gps hat,
	dht11 sensor,
	ldr,
	some LED's,
	a ventilator,
	and a button,

# 	rasberryPi

We're using our Pi as gateway to the TTN so we can integrate with cayenne.

First let's start with enabeling spi on youre Pi. type in youre CLI: "sudo raspi-config" select the 5th option interfacing options.
![alt text](https://github.com/kiwiton/poolhouse/tree/readmeEdits/pics/InterfOptionsSelect.png?raw=true)

