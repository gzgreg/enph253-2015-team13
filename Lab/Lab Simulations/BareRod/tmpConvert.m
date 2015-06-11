function T = tmpConvert(arduinoOut)
V = (arduinoOut/1023)*5000;%miliV
T = V/10.0;%C
end