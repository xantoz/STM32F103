This is a sender for a generic controller with up to 16 buttons
connected from pin A0 up to A15. The buttons are active high; the pins
are internally connected to pull-downs and need to be pulled high by
the button to constitute a button press.

In this configuration the nRF24L01 is connected to SPI2 (NSS/CSN =
B12, SCK = B13, MISO = B14, MOSI = B15). Auxilliary pins: CE = B1, IRQ = B0
