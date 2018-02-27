This repository is a collection of sketches created while experimenting with and Arduino UNO and:
1. a **Bourns** ENA1D-B28-L00128L (Rs code: 789-9504) optical encoder
2. an **Alpha** mechanical encoder;

## Processing

* ```Processing_oscilloscope```: this sketch is useful to figure out how the encoder quadrature works. To be used with the ```encoder_2``` Arduino sketch;
* ```Processing_encoder_fb```: this sketch can be used (w/ the necessary modifications) with both the encoder_3/4 Arduino sketches
* ```Processing_osc_A_B```: this sketch can be used to plot two signals simultaneously. It is tought to be used with the Arduino _encoder_5_avago_heds_ sketch;

## Arduino

* ```encoder_2```: to be used with the Processing oscilloscope in order to figure out how the quadrature works;
* ```encoder_3_optical``` : first test with an **Alpha** encoder;
* ```encoder_4_optical``` : first test with the **Bourns** optical encoder;
* ```encoder_5_avago_heds```:
* ```encoder_6_eran```: this sketch is meant to be used with the **Alpha** mechanical encoder. It outputs the step count;
