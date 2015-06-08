function T = tmpConvert(arduinoOut)
V = (arduinoOut/1023)*5;%volts
T = 3*V +1000;%this formula isn't real and needs to be calibrated
end