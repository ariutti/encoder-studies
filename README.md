This repository is a collection of sketches created while experimenting with an **Bourns** ENA1D-B28-L00128L (Rs code: 789-9504) optical encoder and Arduino UNO.

## Processing

* ```Processing_oscilloscope```: this sketch is useful to figure out how the encoder quadrature works. To be used with the ```encoder_2``` Arduino sketch;
* ```Processing_encoder_fb```: this sketch can be used (w/ the necessary modifications) with both the encoder_3/4 Arduino sketches

## Arduino

* ```encoder_2```: to be used with the Processing oscilloscope in order to figure out how the quadrature works;
* ```encoder_3_optical``` : first test with an **alpha** encoder;
* ```encoder_4_optical``` : first test with the **Bourns** optical encoder;