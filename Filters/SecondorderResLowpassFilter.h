/*
  ==============================================================================

    SecondorderResLowpassFilter.h
    Created: 24 Apr 2023 6:56:09pm
    Author:  Brian Greene

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include <vector>

// 2nd order IIR resonant lowpass filter
// based on Andrew McPherson tutorial https://www.youtube.com/watch?v=XVOdqJy-Rfg&t=1117s

class SecondorderResLowpassFilter
{

public:
    void setCutoffFrequency(float cutoff);
    void setQFacter(float q);
    void calculateCoefficients(float sampleRate, float frequency, float q);
    void setSamplingRate(float sampleRate);
    void processBlock(juce::AudioBuffer<float> &);
    
    
private:
    float cutoff;
    float q;
    float sampleRate;
    std::vector<float> gLastX1 {0}, gLastX2 {0}, gLastY1 {0}, gLastY2 {0};
    float gA1 = 0, gA2 = 0;
    float gB0 = 1, gB1 = 0, gB2 = 0;
    
};
