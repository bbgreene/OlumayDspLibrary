/*
  ==============================================================================

    SecondorderResLowpassFilter.cpp
    Created: 24 Apr 2023 6:56:09pm
    Author:  Brian Greene

  ==============================================================================
*/

#include "SecondorderResLowpassFilter.h"

void SecondorderResLowpassFilter::setCutoffFrequency(float cutoff)
{
    this->cutoff = cutoff;
}
void SecondorderResLowpassFilter::setQFacter(float q)
{
    this->q = q;
}
void SecondorderResLowpassFilter::setSamplingRate(float sampleRate)
{
    this->sampleRate = sampleRate;
}


void SecondorderResLowpassFilter::calculateCoefficients(float sampleRate, float frequency, float q)
{
    float k = std::tan(M_PI * frequency / sampleRate);
    float norm = 1.0 / (1 + k / q + k * k);
    
    gB0 = k * k * norm;
    gB1 = 2.0 * gB0;
    gB2 = gB0;
    gA1 = 2 * (k * k - 1) * norm;
    gA2 = (1 - k / q + k * k) * norm;
}

void SecondorderResLowpassFilter::processBlock(juce::AudioBuffer<float> & buffer)
{
    calculateCoefficients(sampleRate, cutoff, q);
    
    gLastX1.resize(buffer.getNumChannels(), 0.0f);
    gLastX2.resize(buffer.getNumChannels(), 0.0f);
    gLastY1.resize(buffer.getNumChannels(), 0.0f);
    gLastY2.resize(buffer.getNumChannels(), 0.0f);
    
    for(auto channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        auto channelData = buffer.getWritePointer(channel);
        
        for(auto i = 0; i < buffer.getNumSamples(); ++i)
        {
            auto in = channelData[i];
            
            float out = gB0*in + gB1*gLastX1[channel] + gB2*gLastX2[channel] - gA1*gLastY1[channel] - gA2*gLastY2[channel];
            
            gLastX2[channel] = gLastX1[channel];
            gLastX1[channel] = in;
            gLastY2[channel] = gLastY1[channel];
            gLastY1[channel] = out;
            
            channelData[i] = out;
        }
    }
    
}
