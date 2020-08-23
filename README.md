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
![alt text](https://github.com/kiwiton/poolhouse/blob/readmeEdits/pics/InterfOptionsSelect.png)

Second select option 4 "SPI"
![alt text](https://github.com/kiwiton/poolhouse/blob/readmeEdits/pics/selectOption4.png)
After that choose "yes" and press enter and select "ok", now press the right arrow key and enter to finish in this menu an go back to your CLI.

Next up, installing wiringpi:
In the CLI enter "sudo apt-get install wiringpi"
![alt text](https://github.com/kiwiton/poolhouse/blob/readmeEdits/pics/wiringpiCommand.png)

Now downloadind the single channel paket forwarder by entering the following in the CLI:
"Sudo https://github.com/tftelkamp/single_chan_pkt_fwd/archive/master.zip"
This will download a ".zip3 file, we're going to unzip it by typing "unzip master.zip" in the CLI.



